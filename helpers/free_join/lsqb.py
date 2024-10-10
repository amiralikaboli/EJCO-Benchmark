import json
import os
from enum import Enum
from typing import Callable, Final

import pandas as pd
from pydantic import BaseModel

from helpers.constants import Algo, LSQB_TIMINGS_DIR
from helpers.free_join.shared import categorise, mapper, mean_ms, to_frame

SCALING_FACTORS: Final[tuple[float]] = (0.1, 0.3, 1, 3)

FREE_JOIN_DIR: Final[str] = os.path.abspath(os.path.join(LSQB_TIMINGS_DIR, "free-join"))


class AlgoFreeJoin(Enum):
    FJ = "fj"
    FJ_SCALAR = "fj_scalar"
    GJ = "gj"


class RecordSpecs(BaseModel):
    algo: AlgoFreeJoin
    sf: float


class Record(RecordSpecs):
    query: str
    time: tuple[float, float, float, float, float]


SPECS_FIELDS: Final[set[str]] = set(RecordSpecs.model_fields.keys())
SpecsGJ = lambda sf: RecordSpecs(algo=AlgoFreeJoin.GJ, sf=sf)
SpecsScalarFJ = lambda sf: RecordSpecs(algo=AlgoFreeJoin.FJ_SCALAR, sf=sf)
SpecsVectorFJ = lambda sf: RecordSpecs(algo=AlgoFreeJoin.FJ, sf=sf)


def get_specs(algo: Algo, vectorised: bool) -> Callable[[float], RecordSpecs]:
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
def read_lsqb_result(sf: float, algo: Algo, vectorised: bool = False) -> pd.DataFrame:
    assert algo == Algo.FJ or not vectorised
    with open(os.path.join(FREE_JOIN_DIR, "gj.json")) as f:
        data = json.load(f)

    records = [Record.model_validate_json(json.dumps(item)) for item in data["gj"]]
    [categorised] = categorise(SPECS_FIELDS)(records, get_specs(algo, vectorised)(sf))
    return to_frame(mapper(mean_ms)(categorised))
