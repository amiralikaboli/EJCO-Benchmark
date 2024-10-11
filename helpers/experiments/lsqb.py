from pathlib import Path

import pandas as pd

from helpers import free_join, wcoj
from helpers.constants import Algo, LSQB_TIMINGS_DIR, QUERY_COL, SF_COL
from helpers.experiments.shared import join_frames
from helpers.plots import lsqb_plot
from helpers.scaling_factors import SCALING_FACTORS


def lsqb_plots() -> None:
    df = lsqb_overview()
    print(df)
    df.to_csv(Path(LSQB_TIMINGS_DIR) / "overview.csv", index=True)
    lsqb_plot(df, Algo.GJ)
    lsqb_plot(df, Algo.FJ, vectorised=False)
    lsqb_plot(df, Algo.FJ, vectorised=True)


def lsqb_overview() -> pd.DataFrame:
    dfs = [lsqb_overviews(sf) for sf in SCALING_FACTORS]
    return (
        pd.concat(dfs, keys=SCALING_FACTORS, names=[SF_COL, QUERY_COL])
        .swaplevel(SF_COL, QUERY_COL)
        .sort_index()
    )


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
