import os
import subprocess
from pathlib import Path
from typing import Final

import pandas as pd

from helpers.constants import Algo, LSQB_TIMINGS_DIR, SCRIPTS_DIR
from helpers.scaling_factors import DatasetLinking, sf_dir_fmt
from helpers.wcoj.shared import DTYPES, write_results_frame

WCOJ_DIR: Final[str] = os.path.abspath(os.path.join(LSQB_TIMINGS_DIR, "wcoj"))


def read_lsqb_result(algo: Algo, sf: float, timeout: int = 0) -> pd.DataFrame:
    sf_dir = sf_dir_fmt(sf)
    lsqb_sf_dir = os.path.join(WCOJ_DIR, sf_dir)
    lsqb_data_dir = os.path.join(lsqb_sf_dir, f"{algo.value}_results")
    lsqb_results: Final[str] = os.path.join(lsqb_sf_dir, f"{algo.value}_results.csv")

    if not Path(lsqb_data_dir).is_dir():
        with DatasetLinking(sf):
            subprocess.call(
                f"./codegen.sh lsqb/{algo.value} 5", shell=True, cwd=SCRIPTS_DIR
            )
            subprocess.call(
                f"./compile.sh lsqb/{algo.value}", shell=True, cwd=SCRIPTS_DIR
            )
            run = f"./run.sh {timeout} lsqb {sf_dir} {algo.value}"
            subprocess.call(run, shell=True, cwd=SCRIPTS_DIR)

    if not Path(lsqb_results).is_file():
        write_results_frame(lsqb_data_dir, lsqb_results)

    return pd.read_csv(lsqb_results, dtype=DTYPES)
