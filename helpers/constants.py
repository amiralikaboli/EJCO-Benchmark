import os
from typing import Final


FILE_DIR: Final[str] = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR: Final[str] = os.path.abspath(os.path.join(FILE_DIR, ".."))
TIMINGS_DIR: Final[str] = os.path.join(ROOT_DIR, "timings")
SCRIPTS_DIR: Final[str] = os.path.join(ROOT_DIR, "scripts")
PROGS_SYM: Final[str] = os.path.join(ROOT_DIR, "progs")
PROGS_DIR: Final[str] = os.path.realpath(PROGS_SYM)

QUERY_COL = "Query"
RUNTIME_COL = "Runtime (ms)"

SECS_TO_MS: Final[int] = 1_000
