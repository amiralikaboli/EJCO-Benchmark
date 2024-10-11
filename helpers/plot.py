import statistics
from pathlib import Path
from typing import List

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL

plt.rcParams["font.size"] = 14
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["pdf.fonttype"] = 42
plt.rcParams["ps.fonttype"] = 42
plt.rcParams["axes.spines.right"] = False
plt.rcParams["axes.spines.top"] = False


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
    plt.figure(figsize=(5, 5))

    if algo == Algo.GJ:
        legend_str = "Generic Join"
    else:
        legend_str = f"Free Join w/{'o' if not vectorised else ''} vectorization"

    x_values = df[df.columns[0]].values / 1000
    y_values = df[df.columns[1]].values / 1000

    ratio = 1.2
    all_values = np.array(list(x_values) + list(y_values))
    eye_line = [min(all_values) / ratio, max(all_values) * ratio]

    plt.plot(eye_line, eye_line, color="gray")
    plt.scatter(x_values, y_values, color="black", s=10, label=legend_str)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"Our System (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend()
    plt.savefig(
        Path(PLOTS_DIR) / f"{algo.value}_w{'o' if not vectorised else ''}.pdf",
        bbox_inches="tight",
    )


def violin_plot(df: pd.DataFrame) -> None:
    plt.figure(figsize=(10, 4))

    mat = df[["O0", "O1", "O2", "O3", "O4", "FJ", "FJ (vector)"]].to_numpy()
    mat = 1 / (mat[:, :-1] / mat[:, -1][:, np.newaxis])
    dfmat = pd.DataFrame(mat, columns=["Naive", "O1", "O2", "O3", "O4", "O5"])

    sns.violinplot(data=dfmat, color="0.8")
    sns.stripplot(data=dfmat, jitter=True, zorder=1, s=4)

    plt.ylabel("Performance Improvement")
    plt.grid(axis="y", linestyle="dotted")

    plt.savefig(Path(PLOTS_DIR) / "violin.pdf", bbox_inches="tight")


def ablation_plot(tdf: pd.DataFrame, queries: List[str]) -> None:
    fig, axes = plt.subplots(nrows=1, ncols=len(queries))
    fig.set_size_inches(11, 3)
    for query, ax in zip(queries, axes):
        df = tdf[tdf[QUERY_COL] == query]
        df = df[[QUERY_COL, "O0", "O1", "O2", "O3", "O4", "FJ", "FJ (vector)"]]
        df.columns = [QUERY_COL, "Naive", "O1", "O2", "O3", "O4", "O5", "Free Join"]
        df[df.columns[1:]] = df[df.columns[1:]] / 1000
        df.plot.bar(ax=ax, legend=None)
        lines, labels = ax.get_legend_handles_labels()
        ax.set_xticklabels([query], rotation=0)

    plt.figlegend(
        lines,
        labels,
        loc="upper center",
        ncol=7,
        bbox_to_anchor=(0.5, 1.1),
        frameon=False,
        prop={"size": 12},
    )
    plt.setp(axes[0], ylabel="Runtime (s)")
    plt.tight_layout()
    plt.savefig(Path(PLOTS_DIR) / "ablation.pdf", bbox_inches="tight")
