import statistics

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
        baseline, ours, how="outer", on="Query", suffixes=(" baseline", " ours")
    ).set_index("Query")
    df["Performance Improvement"] = (df[df.columns[0]] / df[df.columns[1]]).round(2)
    geometric_mean = round(statistics.geometric_mean(df["Performance Improvement"]), 2)
    print("Geometric Mean", geometric_mean)
    print()
    print(df.sort_values(by=df.columns[2], ascending=False))
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
