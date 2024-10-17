import os
from enum import Enum
from typing import Final

SECS_TO_MS: Final[int] = 1_000

QUERY_COL: Final[str] = "Query"
RUNTIME_COL: Final[str] = "Runtime (ms)"
SF_COL: Final[str] = "SF"

FILE_DIR: Final[str] = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR: Final[str] = os.path.abspath(os.path.join(FILE_DIR, ".."))
ABLATIONS_DIR: Final[str] = os.path.join(ROOT_DIR, "ablations")
TIMINGS_DIR: Final[str] = os.path.join(ROOT_DIR, "timings")
JOB_TIMINGS_DIR: Final[str] = os.path.join(TIMINGS_DIR, "job")
LSQB_TIMINGS_DIR: Final[str] = os.path.join(TIMINGS_DIR, "lsqb")
SCRIPTS_DIR: Final[str] = os.path.join(ROOT_DIR, "scripts")
PLOTS_DIR: Final[str] = os.path.join(ROOT_DIR, "plots")
PROGS_SYM: Final[str] = os.path.join(ROOT_DIR, "progs")
PROGS_DIR: Final[str] = os.path.realpath(PROGS_SYM)
JOB_GJ_DIR: Final[str] = os.path.join(PROGS_DIR, "job", "gj")
JOB_FJ_DIR: Final[str] = os.path.join(PROGS_DIR, "job", "fj")
SDQL_ROOT: Final[str] = os.path.abspath(os.path.join(PROGS_DIR, ".."))
DATASETS_DIR: Final[str] = os.path.realpath(os.path.join(SDQL_ROOT, "datasets"))
GENERATED_DIR: Final[str] = os.path.join(SDQL_ROOT, "generated")


class Source(Enum):
    JOB = "JOB"
    LSQB = "LSQB"


class Algo(Enum):
    FJ = "fj"
    GJ = "gj"
