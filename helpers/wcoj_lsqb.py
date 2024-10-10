import os
import subprocess
from pathlib import Path
from typing import Final

import pandas as pd

from helpers.constants import (
    Algo,
    LSQB_TIMINGS_DIR,
    SCRIPTS_DIR,
)
from helpers.scaling_factors import link_dataset, sf_dir_fmt, unlink_dataset
from helpers.wcoj import DTYPES, write_results_frame

WCOJ_DIR: Final[str] = os.path.abspath(os.path.join(LSQB_TIMINGS_DIR, "wcoj"))


def read_lsqb_result(algo: Algo, sf: float) -> pd.DataFrame:
    sf_dir = sf_dir_fmt(sf)
    wcoj_ablation_dir = os.path.join(WCOJ_DIR, sf_dir)
    wcoj_data_dir = os.path.join(wcoj_ablation_dir, f"{algo.value}_results")
    wcoj_results: Final[str] = os.path.join(
        wcoj_ablation_dir, f"{algo.value}_results.csv"
    )

    if not Path(wcoj_data_dir).is_dir():
        link_dataset(sf)
        subprocess.call(
            f"./codegen.sh lsqb/{algo.value} 5", shell=True, cwd=SCRIPTS_DIR
        )
        subprocess.call(f"./compile.sh lsqb/{algo.value}", shell=True, cwd=SCRIPTS_DIR)
        subprocess.call(
            f"./run.sh lsqb {sf_dir} {algo.value}", shell=True, cwd=SCRIPTS_DIR
        )

        # TODO context handler that unlinks
        unlink_dataset()

    if not Path(wcoj_results).is_file():
        write_results_frame(wcoj_data_dir, wcoj_results)

    return pd.read_csv(wcoj_results, dtype=DTYPES)
