import matplotlib.pyplot as plt
import pandas as pd

from helpers.free_join import read_free_join_results
from helpers.wcoj import Algo, read_wcoj_results


def plot(algo: Algo, vectorised: bool = False) -> None:
    wcoj_gj, scalar_fj, vector_fj = read_free_join_results()
    baseline = (
        wcoj_gj
        if algo.value == Algo.GJ.value
        else (vector_fj if vectorised else scalar_fj)
    )
    ours = read_wcoj_results(algo=algo)
    df = pd.merge(
        baseline, ours, how="outer", on="Query", suffixes=(" expected", " actual")
    ).set_index("Query")
    df["Diff (ms)"] = df[df.columns[1]] - df[df.columns[0]]
    df["Diff (%)"] = (100 * df["Diff (ms)"] / df[df.columns[0]]).round().astype(int)
    pct_diff_summary = pd.DataFrame(df[df.columns[3]].describe().round().astype(int)).T
    print(pct_diff_summary)
    print()
    print(df.sort_values(by=df.columns[3], ascending=False))
    plot_(df, algo, vectorised)


def plot_(df: pd.DataFrame, algo: Algo, vectorised: bool) -> None:
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
