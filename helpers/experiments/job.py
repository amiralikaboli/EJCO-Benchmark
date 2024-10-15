from pathlib import Path

import pandas as pd

from helpers import free_join, wcoj
from helpers.ablations import ABLATIONS, NO_ABLATION
from helpers.constants import Algo, JOB_TIMINGS_DIR, QUERY_COL
from helpers.experiments.shared import join_frames
from helpers.plots import ablation_plot, job_plot, violin_plot
from helpers.wcoj.job import read_job_result_cached


def job_plots() -> None:
    df = job_overview()
    print(df)
    df.to_csv(Path(JOB_TIMINGS_DIR) / "overview.csv", index=False)
    job_plot(df, Algo.GJ)
    job_plot(df, Algo.FJ, vectorised=False)
    job_plot(df, Algo.FJ, vectorised=True)
    violin_plot(df)
    ablation_plot(df, ["9d", "12b", "16b", "19d"])


def job_overview() -> pd.DataFrame:
    names = (
        [
            "GJ (free-join)",
            "FJ (scalar)",
            "FJ (vector)",
            "GJ",
        ]
        + [f"O{ablation}" for ablation in ABLATIONS if ablation != NO_ABLATION]
        + [
            "FJ",
            "FJ sorting (pure)",
            "FJ sorting (hybrid)",
        ]
    )
    results = (
        [
            free_join.read_job_result(algo=Algo.GJ),
            free_join.read_job_result(algo=Algo.FJ, vectorised=False),
            free_join.read_job_result(algo=Algo.FJ, vectorised=True),
            wcoj.read_job_result(algo=Algo.GJ),
        ]
        + [
            wcoj.read_job_result(algo=Algo.FJ, ablation=ablation)
            for ablation in ABLATIONS
            if ablation != NO_ABLATION
        ]
        + [
            wcoj.read_job_result(algo=Algo.FJ, ablation=NO_ABLATION),
            read_job_result_cached(algo=Algo.FJ, dir_name="FJ_SORTING_PURE"),
            read_job_result_cached(algo=Algo.FJ, dir_name="FJ_SORTING"),
        ]
    )
    return join_frames(names, results).set_index(QUERY_COL)
