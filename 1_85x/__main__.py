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

from helpers.benchmarks.shared import join_frames
from helpers.constants import SECS_TO_MS
from helpers.free_join.job import Record, SPECS_FIELDS, SpecsGJ
from helpers.free_join.shared import RecordMean, categorise, mapper, mean_ms, to_frame

FILE_DIR: Final[str] = os.path.dirname(os.path.realpath(__file__))

RE_QUERY: Final[re.Pattern] = re.compile(r"^(\d+)([a-z])$")

BASELINE_COL: Final[str] = "free-join (on server)"
OURS_COL: Final[str] = "wcoj (on server)"


def to_ms(record: RecordMean) -> RecordMean:
    return RecordMean(query=record.query, time=SECS_TO_MS * record.time)


def split_query(record: RecordMean) -> tuple[int, str]:
    n, az = RE_QUERY.match(record.query).groups()
    return int(n), az


if __name__ == "__main__":
    with open(os.path.join(FILE_DIR, "gj.json")) as f:
        data = json.load(f)

    records = [Record.model_validate_json(json.dumps(item)) for item in data["gj"]]
    [baseline] = categorise(SPECS_FIELDS)(records, SpecsGJ)
    baseline_df = to_frame(mapper(mean_ms)(baseline))

    ours = (RecordMean.model_validate_json(json.dumps(item)) for item in data["ours"])
    ours_df = to_frame(sorted(map(to_ms, ours), key=split_query))

    df = join_frames([BASELINE_COL, OURS_COL], [baseline_df, ours_df])
    df["Performance Improvement"] = (df[BASELINE_COL] / df[OURS_COL]).round(2)
    df.sort_values(by="Performance Improvement", ascending=False, inplace=True)

    geometric_mean = round(statistics.geometric_mean(df["Performance Improvement"]), 2)
    assert geometric_mean == 1.85

    print("Geometric Mean", geometric_mean)
    print()
    print(df)
