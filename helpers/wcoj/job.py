import os
import subprocess
from pathlib import Path
from typing import Final

import pandas as pd

from helpers.ablations import Ablation, NO_ABLATION, apply_ablation
from helpers.constants import (
    Algo,
    GENERATED_DIR,
    JOB_TIMINGS_DIR,
    ROOT_DIR,
    SCRIPTS_DIR,
)
from helpers.wcoj.shared import DTYPES, write_results_frame

WCOJ_DIR: Final[str] = os.path.abspath(os.path.join(JOB_TIMINGS_DIR, "wcoj"))


def read_job_result(
    algo: Algo, ablation: Ablation = NO_ABLATION, cpp_dir: str = "", timeout: int = 0
) -> pd.DataFrame:
    if ablation != NO_ABLATION:
        if algo == Algo.GJ:
            raise ValueError("Ablation not supported for GJ")
        if cpp_dir and ablation != NO_ABLATION:
            raise ValueError("Skipping C++ codegen is for sorting, not ablations")

    data_dir = cpp_dir.replace(os.path.sep, "_").upper() if cpp_dir else f"O{ablation}"
    data_path = os.path.join(WCOJ_DIR, data_dir)

    if cpp_dir:
        results_path = os.path.join(data_path, f"{algo.value}_results")
        results_csv = os.path.join(data_path, f"{algo.value}_results.csv")
    else:
        data_path = os.path.join(WCOJ_DIR, data_dir)
        results_path = os.path.join(data_path, f"{algo.value}_results")
        results_csv = os.path.join(data_path, f"{algo.value}_results.csv")

    if not Path(results_path).is_dir():
        if cpp_dir:
            copy_cpp(cpp_dir)
        else:
            apply_ablation(ablation)
            subprocess.call(
                f"./codegen.sh job/{algo.value} 5", shell=True, cwd=SCRIPTS_DIR
            )
        subprocess.call(f"./compile.sh job/{algo.value}", shell=True, cwd=SCRIPTS_DIR)
        run = f"./run.sh {timeout} job {data_dir} {algo.value}"
        subprocess.call(run, shell=True, cwd=SCRIPTS_DIR)

    if not Path(results_csv).is_file():
        write_results_frame(results_path, results_csv)

    return pd.read_csv(results_csv, dtype=DTYPES)


def copy_cpp(cpp_dir: str) -> None:
    bash = f"cp -f {cpp_dir}/*.cpp {GENERATED_DIR}"
    subprocess.call(bash, shell=True, cwd=ROOT_DIR)
