import json
import os
from enum import Enum
from functools import wraps
from statistics import mean
from typing import Callable, Final, TypeVar

import pandas as pd
from pydantic import BaseModel

from helpers.constants import Algo, JOB_TIMINGS_DIR, QUERY_COL, RUNTIME_COL, SECS_TO_MS

T = TypeVar("T")

FREE_JOIN_DIR: Final[str] = os.path.abspath(os.path.join(JOB_TIMINGS_DIR, "free-join"))


class BuildStrategy(Enum):
    Full = 0
    SLT = 1
    COLT = 2


class RecordSpecs(BaseModel):
    vectorize: int
    optimize: int
    strategy: BuildStrategy


class Record(RecordSpecs):
    query: str
    time: tuple[float, ...]


class RecordMean(BaseModel):
    query: str
    time: float


SPECS_FIELDS: Final[set[str]] = set(RecordSpecs.model_fields.keys())
SpecsGJ = RecordSpecs(vectorize=1, optimize=0, strategy=BuildStrategy.Full)
SpecsScalarFJ = RecordSpecs(vectorize=1, optimize=1, strategy=BuildStrategy.COLT)
SpecsVectorFJ = RecordSpecs(vectorize=1_000, optimize=1, strategy=BuildStrategy.COLT)


def compare(a: RecordSpecs, b: RecordSpecs) -> bool:
    return a.model_dump(include=SPECS_FIELDS) == b.model_dump(include=SPECS_FIELDS)


def categorise(records: list[Record], *specs: RecordSpecs) -> list[list[Record]]:
    categorised = [[] for _ in specs]

    for record in records:
        for i, spec in enumerate(specs):
            if compare(record, spec):
                categorised[i].append(record)
                break
        else:
            raise ValueError(f"Record did not match any specs: {record}")

    return categorised


def mapper(func: Callable[[Record], T]) -> Callable[[list[Record]], list[T]]:
    @wraps(func)
    def wrapper(records: list[Record]) -> list[T]:
        return list(map(func, records))

    return wrapper


def mean_ms(record: Record) -> RecordMean:
    return RecordMean(query=record.query, time=round(SECS_TO_MS * mean(record.time)))


def to_series(record: RecordMean) -> pd.Series:
    return pd.Series(record.model_dump())


def to_frame(records: list[RecordMean]) -> pd.DataFrame:
    df = pd.DataFrame(to_series(record) for record in records)
    df["time"] = df["time"].astype(int)
    return df.rename(columns={"query": QUERY_COL, "time": RUNTIME_COL})


# Both generic join and free join were ran for 5 iterations:
# https://github.com/SIGMOD23p561/free-join/blob/c020bbc7964ba17594299a1910ad6b65eebdf0e0/Makefile#L51
# For generic join we ran this code:
# https://github.com/SIGMOD23p561/free-join/blob/c020bbc7964ba17594299a1910ad6b65eebdf0e0/gj/src/main.rs#L106-L114
# As per https://arxiv.org/abs/2301.10841 – 5.1 Setup:
# "We therefore implement a Generic Join baseline ourselves,
#  by modifying Free Join to fully construct all tries, and removing vectorization."
def read_free_join_results() -> tuple[pd.DataFrame, pd.DataFrame, pd.DataFrame]:
    with open(os.path.join(FREE_JOIN_DIR, "gj.json")) as f:
        data = json.load(f)

    records = [Record.model_validate_json(json.dumps(item)) for item in data["gj"]]
    categorised = categorise(records, SpecsGJ, SpecsScalarFJ, SpecsVectorFJ)
    gj, scalar_fj, vector_fj = map(lambda x: to_frame(mapper(mean_ms)(x)), categorised)
    return gj, scalar_fj, vector_fj


def read_free_join_result(algo: Algo, vectorised: bool = False) -> pd.DataFrame:
    assert algo == Algo.FJ or not vectorised
    wcoj_gj, scalar_fj, vector_fj = read_free_join_results()
    return (
        wcoj_gj
        if algo.value == Algo.GJ.value
        else (vector_fj if vectorised else scalar_fj)
    )
