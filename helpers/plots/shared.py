import statistics
from functools import wraps
from typing import Callable, Final

import matplotlib.pyplot as plt
import pandas as pd

from helpers.constants import Algo, Sorting

FIG_SIZE: Final[tuple[int, int]] = (5, 5)
RATIO: Final[float] = 1.2

plt.rcParams["font.size"] = 14
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["pdf.fonttype"] = 42
plt.rcParams["ps.fonttype"] = 42
plt.rcParams["axes.spines.right"] = False
plt.rcParams["axes.spines.top"] = False


def showing_speedup(
    plot: Callable[[pd.DataFrame, Algo, bool], None]
) -> Callable[[pd.DataFrame, Algo, bool], None]:

    @wraps(plot)
    def _plot(df: pd.DataFrame, algo: Algo, vectorised: bool = False):
        df = build_frame(df, vectorised, algo)
        geometric_mean = round(
            statistics.geometric_mean(df["Performance Improvement"]), 2
        )
        print("Geometric Mean", geometric_mean)
        print()
        print(df)
        plot(df, algo, vectorised)

    return _plot


def build_frame(
    df: pd.DataFrame, vectorised: bool, algo: Algo, sorting: Sorting | None = None
) -> pd.DataFrame:
    baseline_col, ours_col = get_colnames(vectorised, algo, sorting)
    df = df[[baseline_col, ours_col]].copy()
    df["Performance Improvement"] = (df[baseline_col] / df[ours_col]).round(2)
    return df.sort_values(by="Performance Improvement", ascending=False)


def get_colnames(
    vectorised: bool, algo: Algo, sorting: Sorting | None = None
) -> tuple[str, str]:
    if algo == Algo.GJ:
        assert not vectorised
        if sorting == Sorting.SORTING:
            raise NotImplementedError()
        elif sorting == Sorting.HYBRID:
            ours = "GJ sorting (hybrid)"
        else:
            ours = "GJ"
        return "GJ (free-join)", ours
    else:
        if sorting == Sorting.HYBRID:
            ours = "FJ sorting (hybrid)"
        elif sorting == Sorting.SORTING:
            ours = "FJ sorting (pure)"
        else:
            ours = "FJ"
        return "FJ (vector)" if vectorised else "FJ (scalar)", ours


def pdf_filename(algo: Algo, vectorised: bool) -> str:
    return f"{algo.value}_w{'o' if not vectorised else ''}.pdf"
