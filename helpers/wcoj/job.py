import os
import subprocess
from pathlib import Path
from typing import Final

import pandas as pd

from helpers.ablations import NO_ABLATION, apply_ablation
from helpers.constants import Algo, JOB_TIMINGS_DIR, SCRIPTS_DIR
from helpers.wcoj.shared import DTYPES, write_results_frame

WCOJ_DIR: Final[str] = os.path.abspath(os.path.join(JOB_TIMINGS_DIR, "wcoj"))


def read_job_result(algo: Algo, ablation: int = NO_ABLATION) -> pd.DataFrame:
    if algo == Algo.GJ and ablation != NO_ABLATION:
        raise ValueError("Ablation not supported for GJ")

    job_ablation_dir = os.path.join(WCOJ_DIR, f"O{ablation}")
    job_data_dir = os.path.join(job_ablation_dir, f"{algo.value}_results")
    job_results: Final[str] = os.path.join(
        job_ablation_dir, f"{algo.value}_results.csv"
    )

    if not Path(job_data_dir).is_dir():
        apply_ablation(ablation)
        subprocess.call(f"./codegen.sh job/{algo.value} 5", shell=True, cwd=SCRIPTS_DIR)
        subprocess.call(f"./compile.sh job/{algo.value}", shell=True, cwd=SCRIPTS_DIR)
        subprocess.call(
            f"./run.sh job O{ablation} {algo.value}", shell=True, cwd=SCRIPTS_DIR
        )

    if not Path(job_results).is_file():
        write_results_frame(job_data_dir, job_results)

    return pd.read_csv(job_results, dtype=DTYPES)
