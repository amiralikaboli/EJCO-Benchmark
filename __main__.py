from functools import reduce
from pathlib import Path
from typing import Iterable

import pandas as pd

from helpers.ablations import ABLATIONS, NO_ABLATION
from helpers.checks import check_progs
from helpers.constants import (
    Algo,
    JOB_TIMINGS_DIR,
    LSQB_TIMINGS_DIR,
    QUERY_COL,
    RUNTIME_COL,
    SCALING_FACTORS,
    Source,
)
from helpers.free_join import read_job_result
from helpers.free_join import read_lsqb_result as free_join_lsqb_result
from helpers.plot import ablation_plot, plot, violin_plot
from helpers.scaling_factors import sf_dir_fmt
from helpers.wcoj import read_wcoj_result
from helpers.wcoj_lsqb import read_lsqb_result as wcoj_read_lsqb_result

# CHANGE THIS HERE
SOURCE = Source.LSQB


def job_plots():
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
            read_job_result(algo=Algo.GJ),
            read_job_result(algo=Algo.FJ, vectorised=False),
            read_job_result(algo=Algo.FJ, vectorised=True),
            read_wcoj_result(algo=Algo.GJ),
        ]
        + [
            read_wcoj_result(algo=Algo.FJ, ablation=ablation)
            for ablation in ABLATIONS
            if ablation != NO_ABLATION
        ]
        + [read_wcoj_result(algo=Algo.FJ, ablation=NO_ABLATION)]
    )
    df = join_frames(names, results)
    print(df)
    df.to_csv(Path(JOB_TIMINGS_DIR) / "overview.csv", index=False)
    plot(df, Algo.GJ)
    plot(df, Algo.FJ, vectorised=False)
    plot(df, Algo.FJ, vectorised=True)
    violin_plot(df)
    ablation_plot(df, ["9d", "12b", "16b", "19d"])


def lsqb_overview(sf: float):
    sf_dir = sf_dir_fmt(sf)
    print(f"SF={sf}")
    names = [
        "GJ (free-join)",
        "FJ (scalar)",
        "FJ (vector)",
        "GJ",
        "FJ",
    ]
    results = [
        free_join_lsqb_result(algo=Algo.GJ, sf=sf),
        free_join_lsqb_result(algo=Algo.FJ, vectorised=False, sf=sf),
        free_join_lsqb_result(algo=Algo.FJ, vectorised=True, sf=sf),
        wcoj_read_lsqb_result(algo=Algo.GJ, sf=sf),
        wcoj_read_lsqb_result(algo=Algo.FJ, sf=sf),
    ]
    df = join_frames(names, results)
    print(df)
    df.to_csv(Path(LSQB_TIMINGS_DIR) / f"overview_{sf_dir}.csv", index=False)
    plot(df, Algo.GJ)
    plot(df, Algo.FJ, vectorised=False)
    plot(df, Algo.FJ, vectorised=True)


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
            for sf in SCALING_FACTORS:
                lsqb_overview(sf)
        case _:
            raise ValueError(f"Unknown source: {SOURCE}")
