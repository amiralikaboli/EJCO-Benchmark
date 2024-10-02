"""
1.85x GJ improvements:
  - ran on server
  - using https://github.com/edin-dal/sdql/tree/fbd1792642f394e06d9a26f08b027782fc38042f
"""

import json
import os
import re
import statistics
from typing import Final

from helpers.constants import SECS_TO_MS
from helpers.free_join import (
    Record,
    RecordMean,
    SpecsGJ,
    categorise,
    mapper,
    mean_ms,
    to_frame,
)
from helpers.plot import merge

FILE_DIR: Final[str] = os.path.dirname(os.path.realpath(__file__))

RE_QUERY: Final[re.Pattern] = re.compile(r"^(\d+)([a-z])$")


def to_ms(record: RecordMean) -> RecordMean:
    return RecordMean(query=record.query, time=SECS_TO_MS * record.time)


def split_query(record: RecordMean) -> tuple[int, str]:
    n, az = RE_QUERY.match(record.query).groups()
    return int(n), az


if __name__ == "__main__":
    with open(os.path.join(FILE_DIR, "gj.json")) as f:
        data = json.load(f)

    records = [Record.model_validate_json(json.dumps(item)) for item in data["gj"]]
    [baseline] = categorise(records, SpecsGJ)
    baseline_df = to_frame(mapper(mean_ms)(baseline))

    ours = (RecordMean.model_validate_json(json.dumps(item)) for item in data["ours"])
    ours_df = to_frame(sorted(map(to_ms, ours), key=split_query))

    df = merge(baseline=baseline_df, ours=ours_df)
    geometric_mean = round(statistics.geometric_mean(df["Performance Improvement"]), 2)
    assert geometric_mean == 1.85

    print("Geometric Mean", geometric_mean)
    print()
    print(df)
