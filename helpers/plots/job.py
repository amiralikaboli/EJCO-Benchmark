from pathlib import Path
from typing import Final, List

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scipy
import seaborn as sns
from matplotlib.ticker import MaxNLocator

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL, SECS_TO_MS, Sorting
from helpers.plots.shared import RATIO, pdf_filename, showing_speedup

JOB_PLOTS_PATH: Final[Path] = Path(PLOTS_DIR) / "job"


@showing_speedup
def job_plot(df: pd.DataFrame, algo: Algo, vectorised: bool) -> None:
    plt.figure(figsize=(3, 3))

    if algo == Algo.GJ:
        legend_str = "Generic Join"
    else:
        legend_str = f"Hash-based"

    df /= SECS_TO_MS
    df.drop(df.columns[-1], axis=1, inplace=True)
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="lightgray", lw=0.5)
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values
    plt.scatter(x_values, y_values, color="black", s=10, label=legend_str, zorder=3)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"EJCO (s)")
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
    plt.ylabel(f"EJCO (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend()

    path = JOB_PLOTS_PATH / f"job_fj_both.pdf"
    plt.savefig(path, bbox_inches="tight")


def violin_plot(df: pd.DataFrame) -> None:
    plt.figure(figsize=(6, 3.5))

    mat = df[["O0", "O1", "O2", "O3", "FJ", "FJ (vector)"]].astype(int).to_numpy()
    mat = 1 / (mat[:, :-1] / mat[:, -1][:, np.newaxis])
    dfmat = pd.DataFrame(mat, columns=["Naive", "O1", "O2", "O3", "O4"])
    gmeans = np.array([scipy.stats.gmean(dfmat[col]) for col in dfmat.columns])

    sns.violinplot(data=dfmat, color="0.8")
    sns.stripplot(data=dfmat, jitter=True, zorder=1, s=4)
    plt.plot(dfmat.columns, gmeans, color="dimgray")
    plt.scatter(dfmat.columns, gmeans, color="lightgray", zorder=3, s=20)

    plt.ylabel("Speedup (×)")
    plt.grid(axis="y", linestyle="dotted")

    plt.savefig(JOB_PLOTS_PATH / "violin.pdf", bbox_inches="tight")


def ablation_plot(tdf: pd.DataFrame, queries: List[str]) -> None:
    fig, axes = plt.subplots(nrows=2, ncols=2)
    fig.set_size_inches(5, 4)
    axes = axes.ravel()
    assert len(queries) == len(axes) - 1
    tdf = tdf.reset_index()
    for query, ax in zip(queries, axes):
        df = tdf[tdf[QUERY_COL] == query]
        df = df[[QUERY_COL, "O0", "O1", "O2", "O3", "FJ"]]
        df.columns = [QUERY_COL, "Naive", "O1", "O2", "O3", "O4"]
        df.plot.bar(ax=ax, legend=None, zorder=3)
        ax.set_xticklabels([query], rotation=0)
        ax.grid(axis="y", linestyle="dotted")
        ax.yaxis.set_major_locator(MaxNLocator(nbins=5))

    ax = axes[-1]
    df = pd.DataFrame([[38041.6, 4553.2, 4491, 4488.4, 343.6]])
    df.columns = ["Naive", "O1", "O2", "O3", "O4"]
    df.plot.bar(logy=True, ax=ax, legend=None, zorder=3)
    lines, labels = ax.get_legend_handles_labels()
    ax.set_xticklabels(["LSQB Q1"], rotation=0)
    ax.grid(axis="y", linestyle="dotted")

    plt.figlegend(
        lines,
        labels,
        loc="upper center",
        ncol=3,
        bbox_to_anchor=(0.5, 1.125),
        frameon=False,
        prop={"size": 12},
    )
    plt.setp(axes[0], ylabel="Run Time (ms)")
    plt.setp(axes[2], ylabel="Run Time (ms)")
    plt.tight_layout()
    plt.savefig(JOB_PLOTS_PATH / "ablation.pdf", bbox_inches="tight")


def job_sorting_plot(
        df: pd.DataFrame, sorting: Sorting, algo: Algo, vectorized: bool = False
) -> None:
    if algo == Algo.GJ:
        if vectorized:
            raise ValueError("Vectorization supported for GJ")
        if sorting == Sorting.SORTING:
            raise ValueError("Pure sorting not supported for GJ")

    # if sorting == sorting.SORTING and vectorized:
    #     raise ValueError()

    plt.figure(figsize=(3, 3))

    vector_or_scalar = "vector" if vectorized else "scalar"
    theirs = "GJ (free-join)" if algo == Algo.GJ else f"FJ ({vector_or_scalar})"

    ours = (
        f"FJ sorting (pure)"
        if sorting == sorting.SORTING
        else f"{algo.value.upper()} sorting (hybrid)"
    )

    if sorting == sorting.SORTING:
        label = "Sort-based"
    elif sorting == sorting.HYBRID:
        if algo == Algo.FJ:
            label = f"Free Join{' w/o vectorization' if not vectorized else ''}"
        else:
            label = "Generic Join"

    df = df[[theirs, ours]]
    df /= SECS_TO_MS
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="lightgray", lw=0.5)
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values
    plt.scatter(x_values, y_values, color="black", label=label, s=10, zorder=3)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"EJCO (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend(loc="upper center", ncol=1, bbox_to_anchor=(0.5, 1.25))
    suffix = "_wo" if algo == Algo.FJ and not vectorized else ""
    path = JOB_PLOTS_PATH / f"job_{algo.value}_{sorting.value}{suffix}.pdf"
    plt.savefig(path, bbox_inches="tight")


def alternatives_plot(tdf: pd.DataFrame, queries: List[str]) -> None:
    color = ["#e377c2", "#7f7f7f", "#bcbd22", "#17becf"]
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
    plt.setp(axes[0], ylabel="Run Time (ms)")
    plt.setp(axes[2], ylabel="Run Time (ms)")
    plt.tight_layout()
    plt.savefig(JOB_PLOTS_PATH / "alternatives.pdf", bbox_inches="tight")
