from pathlib import Path

import pandas as pd

from helpers import free_join, wcoj
from helpers.ablations import ABLATIONS, NO_ABLATION
from helpers.benchmarks.shared import join_frames
from helpers.constants import Algo, JOB_TIMINGS_DIR, Sorting
from helpers.plots import (
    ablation_plot,
    alternatives_plot,
    job_fj_plot,
    job_plot,
    job_sorting_plot,
    violin_plot,
)
from helpers.plots.alternatives import pick_queries


def job_plots() -> None:
    df = job_overview()
    print(df)
    df.to_csv(Path(JOB_TIMINGS_DIR) / "overview.csv", index=True)

    job_plot(df, Algo.GJ)
    job_plot(df, Algo.FJ, vectorised=False)
    job_plot(df, Algo.FJ, vectorised=True)

    job_fj_plot(df)
    violin_plot(df)
    ablation_plot(df, ["9d", "12b", "16b", "19d"])

    job_sorting_plot(df, Sorting.SORTING, Algo.FJ)
    job_sorting_plot(df, Sorting.HYBRID, Algo.FJ)
    job_sorting_plot(df, Sorting.HYBRID, Algo.FJ, vectorized=True)
    job_sorting_plot(df, Sorting.HYBRID, Algo.GJ)

    alternatives_queries = ["7a", "7b", "17b", "17f"]
    alternatives_plot(df, list(alternatives_queries))

    revised_plans_df = revised_plans_table(df)
    print(f"\n{revised_plans_df}")
    revised_plans_df.to_csv(Path(JOB_TIMINGS_DIR) / "revised_plans.csv", index=True)


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
            "GJ sorting (hybrid)",
            "FJ revised plans",
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
            wcoj.read_job_result(algo=Algo.FJ, sorting=Sorting.SORTING),
            wcoj.read_job_result(algo=Algo.FJ, sorting=Sorting.HYBRID),
            wcoj.read_job_result(algo=Algo.GJ, sorting=Sorting.HYBRID),
            wcoj.read_job_result(algo=Algo.FJ, revised_plans=True),
        ]
    )
    return join_frames(names, results)


def revised_plans_table(df: pd.DataFrame) -> pd.DataFrame:
    df = df.loc[["3b", "17b", "15b", "3a", "14a", "15a"]]
    df = df[["FJ (scalar)", "FJ sorting (hybrid)", "FJ revised plans"]]
    df = df.copy()
    df.insert(2, "Speedup (default)", (df.iloc[:, 0] / df.iloc[:, 1]).round(2))
    df["Speedup (rev)"] = (df.iloc[:, 0] / df.iloc[:, -1]).round(2)
    return df
