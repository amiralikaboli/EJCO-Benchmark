from pathlib import Path
from typing import Final, List

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL, SECS_TO_MS
from helpers.plots.shared import FIG_SIZE, RATIO, pdf_filename, showing_speedup

JOB_PLOTS_PATH: Final[Path] = Path(PLOTS_DIR) / "job"


@showing_speedup
def job_plot(df: pd.DataFrame, algo: Algo, vectorised: bool) -> None:
    plt.figure(figsize=FIG_SIZE)

    if algo == Algo.GJ:
        legend_str = "Generic Join"
    else:
        legend_str = f"Free Join w/{'o' if not vectorised else ''} vectorization"

    df /= SECS_TO_MS
    df.drop(df.columns[-1], axis=1, inplace=True)
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="gray")
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values
    plt.scatter(x_values, y_values, color="black", s=10, label=legend_str)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"Free Join framework (s)")
    plt.ylabel(f"Our System (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend()

    path = JOB_PLOTS_PATH / pdf_filename(algo, vectorised)
    plt.savefig(path, bbox_inches="tight")


def violin_plot(df: pd.DataFrame) -> None:
    plt.figure(figsize=(10, 4))

    mat = df[["O0", "O1", "O2", "O3", "O4", "FJ", "FJ (vector)"]].to_numpy()
    mat = 1 / (mat[:, :-1] / mat[:, -1][:, np.newaxis])
    dfmat = pd.DataFrame(mat, columns=["Naive", "O1", "O2", "O3", "O4", "O5"])

    sns.violinplot(data=dfmat, color="0.8")
    sns.stripplot(data=dfmat, jitter=True, zorder=1, s=4)

    plt.ylabel("Performance Improvement")
    plt.grid(axis="y", linestyle="dotted")

    plt.savefig(JOB_PLOTS_PATH / "violin.pdf", bbox_inches="tight")


def ablation_plot(tdf: pd.DataFrame, queries: List[str]) -> None:
    fig, axes = plt.subplots(nrows=1, ncols=len(queries))
    fig.set_size_inches(11, 3)
    tdf = tdf.reset_index()
    for query, ax in zip(queries, axes):
        df = tdf[tdf[QUERY_COL] == query]
        df = df[[QUERY_COL, "O0", "O1", "O2", "O3", "O4", "FJ", "FJ (vector)"]]
        df.columns = [QUERY_COL, "Naive", "O1", "O2", "O3", "O4", "O5", "Free Join"]
        df[df.columns[1:]] = df[df.columns[1:]] / SECS_TO_MS
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
    plt.savefig(JOB_PLOTS_PATH / "ablation.pdf", bbox_inches="tight")
