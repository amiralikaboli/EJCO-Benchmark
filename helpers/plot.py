import statistics
from pathlib import Path
from typing import Final, List

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL, SECS_TO_MS

JOB_PLOTS_PATH: Final[Path] = Path(PLOTS_DIR) / "job"
LSQB_PLOTS_PATH: Final[Path] = Path(PLOTS_DIR) / "lsqb"

# Same as free-join paper's
# https://github.com/remysucre/gj-vs-binary/blob/91d8b6791ea56dd38805a432a86b361daed18ada/scripts/lsqb.py#L37-L43
LINE_STYLES: Final[dict[str, str]] = {
    "q1": "-",
    "q2": "--",
    "q3": "-.",  # wasn't reproducible
    "q4": ":",
    "q5": (5, (10, 3)),
}

SHORT_TO_LONG: Final[dict[str, str]] = {
    "GJ (free-join)": "Generic Join",
    "FJ (scalar)": "Free Join w/o vectorisation",
    "FJ (vector)": "Free Join w/ vectorisation",
    "GJ": "Generic Join (our system)",
    "FJ": "Free Join (our system)",
}

FIG_SIZE: Final[tuple[int, int]] = (5, 5)
RATIO: Final[float] = 1.2

plt.rcParams["font.size"] = 14
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams["pdf.fonttype"] = 42
plt.rcParams["ps.fonttype"] = 42
plt.rcParams["axes.spines.right"] = False
plt.rcParams["axes.spines.top"] = False


# TODO calculate geometric mean and print performance improvement
def lsqb_plot(df: pd.DataFrame, algo: Algo, vectorised: bool = False) -> None:
    _lsqb_plot(df, algo, vectorised)


def _lsqb_plot(df: pd.DataFrame, algo: Algo, vectorised: bool = False) -> None:
    plt.figure(figsize=FIG_SIZE)

    colnames = list(get_colnames(algo, vectorised))
    df = df[colnames]
    df /= SECS_TO_MS
    print()
    print(df)
    print()
    queries = {
        query: df.xs(query, level="Query")
        for query in df.index.get_level_values(QUERY_COL).unique()
    }

    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="gray")
    for q, q_df in queries.items():
        x_values = q_df.iloc[:, 0]
        y_values = q_df.iloc[:, 1]
        plt.plot(x_values, y_values, linestyle=LINE_STYLES[q], color="black", label=q)

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(SHORT_TO_LONG[df.columns[0]])
    plt.ylabel(SHORT_TO_LONG[df.columns[1]])
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend()

    # TODO switch back
    plt.show()
    # path = LSQB_PLOTS_PATH / pdf_filename(algo, vectorised)
    # plt.savefig(path, bbox_inches="tight")


def job_plot(df: pd.DataFrame, algo: Algo, vectorised: bool = False) -> None:
    df = build_frame(df, algo, vectorised)
    geometric_mean = round(statistics.geometric_mean(df["Performance Improvement"]), 2)
    print("Geometric Mean", geometric_mean)
    print()
    print(df)
    _job_plot(df, algo, vectorised)


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


def pdf_filename(algo: Algo, vectorised: bool) -> str:
    return f"{algo.value}_w{'o' if not vectorised else ''}.pdf"


def _job_plot(df: pd.DataFrame, algo: Algo, vectorised: bool) -> None:
    plt.figure(figsize=FIG_SIZE)

    if algo == Algo.GJ:
        legend_str = "Generic Join"
    else:
        legend_str = f"Free Join w/{'o' if not vectorised else ''} vectorization"

    df /= SECS_TO_MS
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
