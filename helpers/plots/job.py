from pathlib import Path
from typing import Final, List

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scipy
import seaborn as sns
from matplotlib.ticker import MaxNLocator

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL, SECS_TO_MS
from helpers.plots.shared import RATIO, pdf_filename, showing_speedup

JOB_PLOTS_PATH: Final[Path] = Path(PLOTS_DIR) / "job"


@showing_speedup
def job_plot(df: pd.DataFrame, algo: Algo, vectorised: bool) -> None:
    plt.figure(figsize=(3, 3))

    if algo == Algo.GJ:
        legend_str = "Generic Join"
    else:
        legend_str = f"Free Join{'\nw/o vectorization' if not vectorised else ''}"

    df /= SECS_TO_MS
    df.drop(df.columns[-1], axis=1, inplace=True)
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="gray", lw=0.5)
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values
    plt.scatter(x_values, y_values, color="black", s=10, label=legend_str, zorder=3)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"Our System (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend(loc="upper center", ncol=1, bbox_to_anchor=(0.5, 1.25))

    path = JOB_PLOTS_PATH / f"job_{pdf_filename(algo, vectorised)}"
    plt.savefig(path, bbox_inches="tight")


def job_fj_plot(df: pd.DataFrame) -> None:
    plt.figure(figsize=(3, 3))
    df = df[["FJ (scalar)", "FJ (vector)", "FJ"]]
    df /= SECS_TO_MS
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="gray", lw=0.5)
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[2]].values
    plt.scatter(
        x_values,
        y_values,
        color="lightgray",
        s=10,
        label="Free Join w/o vectorization",
        zorder=3,
    )
    x2_values = df[df.columns[1]].values
    plt.scatter(x2_values, y_values, color="black", s=10, label="Free Join", zorder=3)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"Our System (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend()

    path = JOB_PLOTS_PATH / f"job_fj_both.pdf"
    plt.savefig(path, bbox_inches="tight")


def violin_plot(df: pd.DataFrame) -> None:
    plt.figure(figsize=(6, 3))

    mat = df[["O0", "O1", "O2", "O3", "O4", "FJ", "FJ (vector)"]].to_numpy()
    mat = 1 / (mat[:, :-1] / mat[:, -1][:, np.newaxis])
    dfmat = pd.DataFrame(mat, columns=["Naive", "O1", "O2", "O3", "O4", "O5"])
    gmeans = np.array([scipy.stats.gmean(dfmat[col]) for col in dfmat.columns])

    sns.violinplot(data=dfmat, color="0.8")
    sns.stripplot(data=dfmat, jitter=True, zorder=1, s=4)
    plt.plot(dfmat.columns, gmeans, color="dimgray")
    plt.scatter(dfmat.columns, gmeans, color="lightgray", zorder=3, s=20)

    plt.ylabel("Performance Improvement (×)")
    plt.grid(axis="y", linestyle="dotted")

    plt.savefig(JOB_PLOTS_PATH / "violin.pdf", bbox_inches="tight")


def ablation_plot(tdf: pd.DataFrame, queries: List[str]) -> None:
    fig, axes = plt.subplots(nrows=2, ncols=2)
    fig.set_size_inches(5, 4)
    axes = axes.ravel()
    assert len(queries) == len(axes)
    tdf = tdf.reset_index()
    for query, ax in zip(queries, axes):
        df = tdf[tdf[QUERY_COL] == query]
        df = df[[QUERY_COL, "O0", "O1", "O2", "O3", "O4", "FJ"]]
        df.columns = [QUERY_COL, "Naive", "O1", "O2", "O3", "O4", "O5"]
        df.plot.bar(ax=ax, legend=None, zorder=3)
        lines, labels = ax.get_legend_handles_labels()
        ax.set_xticklabels([query], rotation=0)
        ax.grid(axis="y", linestyle="dotted")
        ax.yaxis.set_major_locator(MaxNLocator(nbins=5))

    plt.figlegend(
        lines,
        labels,
        loc="upper center",
        ncol=3,
        bbox_to_anchor=(0.5, 1.125),
        frameon=False,
        prop={"size": 12},
    )
    plt.setp(axes[0], ylabel="Runtime (ms)")
    plt.setp(axes[2], ylabel="Runtime (ms)")
    plt.tight_layout()
    plt.savefig(JOB_PLOTS_PATH / "ablation.pdf", bbox_inches="tight")


def job_sorting_plot(df: pd.DataFrame, algo: Algo) -> None:
    plt.figure(figsize=(3, 3))
    if algo == Algo.SORTING:
        column = "FJ sorting (pure)"
        label = "Sort-based"
    else:
        column = "FJ sorting (hybrid)"
        label = "Hybrid"
    df = df[["FJ (vector)", column]]
    df /= SECS_TO_MS
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="gray", lw=0.5)
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values
    plt.scatter(x_values, y_values, color="black", label=label, s=10, zorder=3)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"Our System (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend(loc="upper center", ncol=1, bbox_to_anchor=(0.5, 1.25))

    path = JOB_PLOTS_PATH / f"job_fj_{algo.value}.pdf"
    plt.savefig(path, bbox_inches="tight")


def alternatives_plot(tdf: pd.DataFrame, queries: List[str]) -> None:
    # old ones ["#e377c2", "#7f7f7f", "#bcbd22", "#17becf"]
    # new ones https://coolors.co/palette/335c67-fff3b0-e09f3e-9e2a2b-540b0e
    color = ["#335C67", "#E09F3E", "#9E2A2B", "#540B0E"]
    assert len(queries) == len(color)
    fig, axes = plt.subplots(nrows=2, ncols=2)
    fig.set_size_inches(5, 4)
    axes = axes.ravel()
    assert len(queries) == len(axes)
    tdf = tdf.reset_index()
    for query, ax in zip(queries, axes):
        df = tdf[tdf[QUERY_COL] == query]
        df = df[
            [QUERY_COL, "FJ (vector)", "FJ", "FJ sorting (pure)", "FJ sorting (hybrid)"]
        ]
        df.columns = [QUERY_COL, "Free Join", "Hash-based", "Sort-based", "Hybrid"]
        df.plot.bar(ax=ax, legend=None, zorder=3, color=color)
        lines, labels = ax.get_legend_handles_labels()
        ax.set_xticklabels([query], rotation=0)
        ax.grid(axis="y", linestyle="dotted")
        ax.yaxis.set_major_locator(MaxNLocator(nbins=5))

    plt.figlegend(
        lines,
        labels,
        loc="upper center",
        ncol=2,
        bbox_to_anchor=(0.5, 1.125),
        frameon=False,
    )
    plt.setp(axes[0], ylabel="Runtime (ms)")
    plt.setp(axes[2], ylabel="Runtime (ms)")
    plt.tight_layout()
    plt.savefig(JOB_PLOTS_PATH / "alternatives.pdf", bbox_inches="tight")
