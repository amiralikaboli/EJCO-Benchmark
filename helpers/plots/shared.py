from typing import Final

import matplotlib.pyplot as plt
import pandas as pd

from helpers.constants import Algo

FIG_SIZE: Final[tuple[int, int]] = (5, 5)
RATIO: Final[float] = 1.2

plt.rcParams["font.size"] = 14
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["pdf.fonttype"] = 42
plt.rcParams["ps.fonttype"] = 42
plt.rcParams["axes.spines.right"] = False
plt.rcParams["axes.spines.top"] = False


def build_frame(df: pd.DataFrame, algo: Algo, vectorised: bool) -> pd.DataFrame:
    baseline_col, ours_col = get_colnames(algo, vectorised)
    df = df[[baseline_col, ours_col]].copy()
    df["Performance Improvement"] = (df[baseline_col] / df[ours_col]).round(2)
    return df.sort_values(by="Performance Improvement", ascending=False)


def get_colnames(algo: Algo, vectorised: bool) -> tuple[str, str]:
    if algo == Algo.GJ:
        assert not vectorised
        return "GJ (free-join)", "GJ"
    else:
        return "FJ (vector)" if vectorised else "FJ (scalar)", "FJ"


def pdf_filename(algo: Algo, vectorised: bool) -> str:
    return f"{algo.value}_w{'o' if not vectorised else ''}.pdf"
