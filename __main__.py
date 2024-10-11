from functools import reduce
from pathlib import Path
from typing import Iterable

import pandas as pd

from helpers import free_join, wcoj
from helpers.ablations import ABLATIONS, NO_ABLATION
from helpers.checks import check_progs
from helpers.constants import (
    Algo,
    JOB_TIMINGS_DIR,
    LSQB_TIMINGS_DIR,
    QUERY_COL,
    RUNTIME_COL,
    SF_COL,
    Source,
)
from helpers.plot import ablation_plot, plot, violin_plot
from helpers.scaling_factors import SCALING_FACTORS

# CHANGE THIS HERE
SOURCE = Source.LSQB


def job_plots() -> None:
    names = (
        [
            "GJ (free-join)",
            "FJ (scalar)",
            "FJ (vector)",
            "GJ",
        ]
        + [f"O{ablation}" for ablation in ABLATIONS if ablation != NO_ABLATION]
        + ["FJ"]
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
        + [wcoj.read_job_result(algo=Algo.FJ, ablation=NO_ABLATION)]
    )
    df = join_frames(names, results)
    print(df)
    df.to_csv(Path(JOB_TIMINGS_DIR) / "overview.csv", index=False)
    plot(df, Algo.GJ)
    plot(df, Algo.FJ, vectorised=False)
    plot(df, Algo.FJ, vectorised=True)
    violin_plot(df)
    ablation_plot(df, ["9d", "12b", "16b", "19d"])


def lsqb_plots() -> None:
    dfs = [lsqb_overviews(sf) for sf in SCALING_FACTORS]
    df = (
        pd.concat(dfs, keys=SCALING_FACTORS, names=[SF_COL, QUERY_COL])
        .swaplevel(SF_COL, QUERY_COL)
        .sort_index()
    )
    print(df)
    df.to_csv(Path(LSQB_TIMINGS_DIR) / "overview.csv", index=True)
    # TODO plots


def lsqb_overviews(sf: float) -> pd.DataFrame:
    names = [
        "GJ (free-join)",
        "FJ (scalar)",
        "FJ (vector)",
        "GJ",
        "FJ",
    ]
    results = [
        free_join.read_lsqb_result(algo=Algo.GJ, sf=sf),
        free_join.read_lsqb_result(algo=Algo.FJ, vectorised=False, sf=sf),
        free_join.read_lsqb_result(algo=Algo.FJ, vectorised=True, sf=sf),
        wcoj.read_lsqb_result(algo=Algo.GJ, sf=sf),
        wcoj.read_lsqb_result(algo=Algo.FJ, sf=sf),
    ]
    return join_frames(names, results).set_index(QUERY_COL)


def join_frames(names: Iterable[str], frames: Iterable[pd.DataFrame]) -> pd.DataFrame:
    return reduce(
        lambda l, r: pd.merge(l, r, on=QUERY_COL),
        (df.rename(columns={RUNTIME_COL: name}) for name, df in zip(names, frames)),
    )


if __name__ == "__main__":
    check_progs()
    match SOURCE:
        case Source.JOB:
            job_plots()
        case Source.LSQB:
            lsqb_plots()
        case _:
            raise ValueError(f"Unknown source: {SOURCE}")
