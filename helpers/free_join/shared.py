import json
import os
from functools import wraps
from statistics import mean
from typing import Callable, Type, TypeVar

import pandas as pd
from pydantic import BaseModel

from helpers.constants import QUERY_COL, RUNTIME_COL, SECS_TO_MS

RecSpecType = TypeVar("RecSpecType", bound=BaseModel)
RecType = TypeVar("RecType", bound=BaseModel)
T = TypeVar("T")


class RecordMean(BaseModel):
    query: str
    time: float


def read_records(rec_type: Type[RecType]) -> Callable[[str], list[RecType]]:
    def _read_records(timings_dir: str) -> list[RecType]:
        with open(os.path.join(timings_dir, "gj.json")) as f:
            data = json.load(f)

        return [rec_type.model_validate_json(json.dumps(item)) for item in data["gj"]]

    return _read_records


def compare(specs_fields: set[str]) -> Callable[[RecSpecType, RecSpecType], bool]:
    def _compare(a: RecSpecType, b: RecSpecType) -> bool:
        return a.model_dump(include=specs_fields) == b.model_dump(include=specs_fields)

    return _compare


def categorise(
    specs_fields: set[str],
) -> Callable[[list[RecType], *tuple[RecSpecType, ...]], list[list[RecType]]]:
    def _categorise(records: list[RecType], *specs: RecSpecType) -> list[list[RecType]]:
        categorised = [[] for _ in specs]

        for record in records:
            for i, spec in enumerate(specs):
                if compare(specs_fields)(record, spec):
                    categorised[i].append(record)
                    break

        return categorised

    return _categorise


def mapper(func: Callable[[RecType], T]) -> Callable[[list[RecType]], list[T]]:
    @wraps(func)
    def wrapper(records: list[RecType]) -> list[T]:
        return list(map(func, records))

    return wrapper


def mean_ms(record: RecType) -> RecordMean:
    return RecordMean(query=record.query, time=round(SECS_TO_MS * mean(record.time)))


def to_series(record: RecordMean) -> pd.Series:
    return pd.Series(record.model_dump())


def to_frame(records: list[RecordMean]) -> pd.DataFrame:
    df = pd.DataFrame(to_series(record) for record in records)
    df["time"] = df["time"].astype(int)
    return df.rename(columns={"query": QUERY_COL, "time": RUNTIME_COL})
