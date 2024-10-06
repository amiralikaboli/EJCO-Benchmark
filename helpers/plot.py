import statistics

import matplotlib.pyplot as plt
import pandas as pd

from helpers.constants import Algo, QUERY_COL


def plot(df: pd.DataFrame, algo: Algo, vectorised: bool = False) -> None:
    df = build_frame(df, algo, vectorised)
    geometric_mean = round(statistics.geometric_mean(df["Performance Improvement"]), 2)
    print("Geometric Mean", geometric_mean)
    print()
    print(df)
    plot_frame(df, algo, vectorised)


def build_frame(df: pd.DataFrame, algo: Algo, vectorised: bool) -> pd.DataFrame:
    baseline_col, ours_col = get_colnames(algo, vectorised)
    df = df[[QUERY_COL, baseline_col, ours_col]].set_index(QUERY_COL)
    df["Performance Improvement"] = (df[baseline_col] / df[ours_col]).round(2)
    return df.sort_values(by="Performance Improvement", ascending=False)


def get_colnames(algo: Algo, vectorised: bool) -> tuple[str, str]:
    if algo == Algo.GJ:
        assert not vectorised
        return "GJ (free-join)", "GJ"
    else:
        return "FJ (vector)" if vectorised else "FJ (scalar)", "FJ"


def plot_frame(df: pd.DataFrame, algo: Algo, vectorised: bool) -> None:
    plt.plot(df[df.columns[0]], df[df.columns[0]], color="gray")
    plt.scatter(df[df.columns[0]], df[df.columns[1]], color="black", s=5)
    plt.xscale("log")
    plt.yscale("log")
    label = (
        " "
        if algo.value == Algo.GJ.value
        else (" with vectorisation " if vectorised else " without vectorisation ")
    )
    plt.xlabel(f"free-join - {algo.value.upper()}{label}(ms)")
    plt.ylabel(f"WCOJ - {algo.value.upper()} (ms)")
    plt.show()
