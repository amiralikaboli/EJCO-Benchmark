import statistics
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL


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
    plt.rcParams["font.size"] = 14
    plt.rcParams["font.family"] = "Times New Roman"
    plt.rcParams["pdf.fonttype"] = 42
    plt.rcParams["ps.fonttype"] = 42
    plt.figure(figsize=(5, 5))

    vec_str = str()
    if algo == Algo.GJ:
        legend_str = "Generic Join"
        xlabel_str = "Free Join's Generic Join"
    else:
        legend_str = "Free Join"
        xlabel_str = "Free Join"
        vec_str = f" w/{'o' if not vectorised else ''} vectorization"

    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values

    ratio = 1
    all_values = np.array(list(x_values) + list(y_values)) / 1000
    eye_line = [min(all_values) / ratio, max(all_values) * ratio]

    plt.plot(eye_line, eye_line, color="gray")
    plt.scatter(x_values / 1000, y_values / 1000, color="black", s=10, label=legend_str)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"{xlabel_str}{vec_str} (s)")
    plt.ylabel(f"Our System (s)")
    plt.legend()
    plt.savefig(
        Path(PLOTS_DIR) / f"{algo.value}-{int(vectorised)}.pdf", bbox_inches="tight"
    )


def violin_plot(df: pd.DataFrame) -> None:
    plt.rcParams["font.size"] = 14
    plt.rcParams["font.family"] = "Times New Roman"
    plt.rcParams["pdf.fonttype"] = 42
    plt.rcParams["ps.fonttype"] = 42
    plt.figure(figsize=(10, 4))

    mat = df[["O0", "O1", "O2", "O3", "O4", "FJ", "FJ (vector)"]].to_numpy()
    mat = 1 / (mat[:, :-1] / mat[:, -1][:, np.newaxis])
    dfmat = pd.DataFrame(mat, columns=["Naive", "O1", "O2", "O3", "O4", "O5"])

    sns.violinplot(data=dfmat, color="0.8")
    sns.stripplot(data=dfmat, jitter=True, zorder=1)

    plt.ylabel("Performance Improvement")
    plt.grid(axis="y", linestyle="dotted")

    plt.savefig(Path(PLOTS_DIR) / "violin.pdf", bbox_inches="tight")
