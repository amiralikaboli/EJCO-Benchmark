import os
import subprocess
from pathlib import Path
from typing import Final

import pandas as pd

from helpers.ablations import Ablation, NO_ABLATION, apply_ablation
from helpers.constants import Algo, JOB_TIMINGS_DIR, SCRIPTS_DIR, Sorting
from helpers.wcoj.shared import DTYPES, write_results_frame

WCOJ_DIR: Final[str] = os.path.abspath(os.path.join(JOB_TIMINGS_DIR, "wcoj"))


def read_job_result(
    algo: Algo,
    ablation: Ablation = NO_ABLATION,
    sorting: Sorting | None = None,
    timeout: int = 0,
) -> pd.DataFrame:
    if algo == Algo.GJ:
        if ablation != NO_ABLATION:
            raise ValueError("Ablation not supported for GJ")
        if sorting == Sorting.SORTING:
            raise ValueError("Pure sorting not supported for GJ")

    data_dir = f"O{ablation}" if sorting is None else sorting.value.upper()
    data_path = os.path.join(WCOJ_DIR, data_dir)
    results_path = os.path.join(data_path, f"{algo.value}_results")
    results_csv = os.path.join(data_path, f"{algo.value}_results.csv")

    if not Path(results_path).is_dir():
        if sorting is None:
            apply_ablation(ablation)

        progs_path = (
            f"job/{algo.value}"
            if sorting is None
            else f"job/{sorting.value}/{algo.value}"
        )
        subprocess.call(f"./codegen.sh {progs_path} 5", shell=True, cwd=SCRIPTS_DIR)
        subprocess.call(f"./compile.sh {progs_path}", shell=True, cwd=SCRIPTS_DIR)
        run = f"./run.sh {timeout} job {data_dir} {algo.value}"
        subprocess.call(run, shell=True, cwd=SCRIPTS_DIR)

    if not Path(results_csv).is_file():
        write_results_frame(results_path, results_csv)

    return pd.read_csv(results_csv, dtype=DTYPES)
