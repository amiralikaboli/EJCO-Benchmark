import os
import re
import subprocess
from enum import Enum
from pathlib import Path
from typing import Any, Final

import pandas as pd

from helpers.checks import check_progs_diffs
from helpers.constants import QUERY_COL, RUNTIME_COL, SCRIPTS_DIR, TIMINGS_DIR

WCOJ_DIR: Final[str] = os.path.abspath(os.path.join(TIMINGS_DIR, "wcoj"))

RE_RUNTIME: Final[re.Pattern] = re.compile(r"(\d+\.?\d*]?) ms")

DTYPES: Final[dict[str, Any]] = {QUERY_COL: "string", RUNTIME_COL: int}


class Algo(Enum):
    FJ = "fj"
    GJ = "gj"


def read_wcoj_results(algo: Algo) -> pd.DataFrame:
    job_data_dir: Final[str] = os.path.join(WCOJ_DIR, f"{algo.value}_results")
    job_results: Final[str] = os.path.join(WCOJ_DIR, f"{algo.value}_results.csv")

    if not Path(job_data_dir).is_dir():
        if check_progs_diffs():
            raise ValueError("sdql/progs has git diffs (restore it after ablations)")

        subprocess.call(f"./codegen.sh {algo.value} 5", shell=True, cwd=SCRIPTS_DIR)
        subprocess.call(f"./compile.sh {algo.value}", shell=True, cwd=SCRIPTS_DIR)
        subprocess.call(f"./run.sh {algo.value}", shell=True, cwd=SCRIPTS_DIR)

    if not Path(job_results).is_file():
        write_results_frame(job_data_dir, job_results)

    return pd.read_csv(job_results, dtype=DTYPES)


def write_results_frame(data_dir: str, output_csv: str) -> None:
    df = pd.DataFrame(
        get_query_names_and_times(data_dir), columns=[QUERY_COL, RUNTIME_COL]
    )
    df.to_csv(output_csv, index=False)


def get_query_names_and_times(data_dir: str) -> list[tuple[str, int]]:
    files = get_files(data_dir)
    query_names = get_query_names(files)
    times = [
        get_ms(Path(os.path.join(data_dir, f)).read_text(), RE_RUNTIME) for f in files
    ]
    return list(zip(query_names, times))


def get_query_names(files: list[str]) -> list[str]:
    return [f.split(".", 1)[0] for f in files]


def get_files(data_dir: str) -> list[str]:
    return sorted(
        (f for f in next(os.walk(data_dir))[2] if f.endswith(".result")),
        key=lambda f: "0" + f if len(f) == len("__.result") else f,
    )


def get_ms(s: str, regex: re.Pattern) -> int:
    return round(float(regex.search(s).group(1)))
