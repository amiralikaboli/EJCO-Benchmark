import os
from enum import Enum
from typing import Final

import pandas as pd
from pydantic import BaseModel

from helpers.constants import Algo, JOB_TIMINGS_DIR
from helpers.free_join.shared import categorise, mapper, mean_ms, read_records, to_frame

FREE_JOIN_DIR: Final[str] = os.path.join(JOB_TIMINGS_DIR, "free-join")


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
    time: tuple[float, float, float, float, float]


SPECS_FIELDS: Final[set[str]] = set(RecordSpecs.model_fields.keys())
SpecsGJ = RecordSpecs(vectorize=1, optimize=0, strategy=BuildStrategy.Full)
SpecsScalarFJ = RecordSpecs(vectorize=1, optimize=1, strategy=BuildStrategy.COLT)
SpecsVectorFJ = RecordSpecs(vectorize=1_000, optimize=1, strategy=BuildStrategy.COLT)


def get_specs(algo: Algo, vectorised: bool) -> RecordSpecs:
    assert algo == Algo.FJ or not vectorised
    if algo.value == Algo.GJ.value:
        return SpecsGJ
    elif vectorised:
        return SpecsVectorFJ
    else:
        return SpecsScalarFJ


# Both generic join and free join were ran for 5 iterations:
# https://github.com/SIGMOD23p561/free-join/blob/c020bbc7964ba17594299a1910ad6b65eebdf0e0/Makefile#L51
# For generic join we ran this code:
# https://github.com/SIGMOD23p561/free-join/blob/c020bbc7964ba17594299a1910ad6b65eebdf0e0/gj/src/main.rs#L106-L114
# As per https://arxiv.org/abs/2301.10841 – 5.1 Setup:
# "We therefore implement a Generic Join baseline ourselves,
#  by modifying Free Join to fully construct all tries, and removing vectorization."
def read_job_result(algo: Algo, vectorised: bool = False) -> pd.DataFrame:
    records = read_records(Record)(FREE_JOIN_DIR)
    [categorised] = categorise(SPECS_FIELDS)(records, get_specs(algo, vectorised))
    return to_frame(mapper(mean_ms)(categorised))
