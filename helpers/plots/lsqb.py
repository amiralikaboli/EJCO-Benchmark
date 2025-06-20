from pathlib import Path
from typing import Final

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from helpers.constants import Algo, PLOTS_DIR, QUERY_COL, SECS_TO_MS
from helpers.plots.shared import (
    FIG_SIZE,
    RATIO,
    get_colnames,
    pdf_filename,
    showing_speedup,
)

LSQB_PLOTS_PATH: Final[Path] = Path(PLOTS_DIR) / "lsqb"

SHORT_TO_LONG: Final[dict[str, str]] = {
    "GJ (free-join)": "Generic Join",
    "FJ (scalar)": "Free Join w/o vectorisation",
    "FJ (vector)": "Free Join w/ vectorisation",
    "GJ": "Generic Join (our system)",
    "FJ": "Free Join (our system)",
}

# Same as free-join paper's
# https://github.com/remysucre/gj-vs-binary/blob/91d8b6791ea56dd38805a432a86b361daed18ada/scripts/lsqb.py#L37-L43
LINE_STYLES: Final[dict[str, str]] = {
    "q1": "-",
    "q2": "--",
    "q3": "-.",  # wasn't reproducible
    "q4": ":",
    "q5": (5, (10, 3)),
}


@showing_speedup
def lsqb_plot(df: pd.DataFrame, algo: Algo, vectorised: bool = False) -> None:
    plt.figure(figsize=(3, 3))

    colnames = list(get_colnames(vectorised, algo))
    df = df[colnames]
    df /= SECS_TO_MS
    queries = {
        query: df.xs(query, level="Query")
        for query in sorted(df.index.get_level_values(QUERY_COL).unique())
    }

    eye_line = [np.min(df) / RATIO / 1.5, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="lightgray", lw=0.5)
    for q, q_df in queries.items():
        x_values = q_df.iloc[:, 0]
        y_values = q_df.iloc[:, 1]
        # sort by y values
        x_values = [x for _, x in sorted(zip(y_values, x_values))]
        y_values = [y for y, _ in sorted(zip(y_values, x_values))]
        plt.plot(
            x_values,
            y_values,
            linestyle=LINE_STYLES[q],
            color="black",
            label=q,
            lw=2,
            zorder=3,
        )

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Free Join framework (s)")
    plt.ylabel("EJCO (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend(loc="upper center", ncol=2, bbox_to_anchor=(0.5, 1.3))

    path = LSQB_PLOTS_PATH / f"lsqb_{pdf_filename(algo, vectorised)}"
    plt.savefig(path, bbox_inches="tight")


def lsqb_all_plot(df: pd.DataFrame) -> None:
    plt.figure(figsize=FIG_SIZE)

    colnames = ["GJ (free-join)", "GJ", "FJ (vector)", "FJ"]
    df = df[colnames]
    df /= SECS_TO_MS
    queries = {
        query: df.xs(query, level="Query")
        for query in sorted(df.index.get_level_values(QUERY_COL).unique())
    }

    eye_line = [np.min(df) / RATIO / 1.5, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="lightgray", lw=0.5)
    for q, q_df in queries.items():
        x2_values = q_df.iloc[:, 0]
        y2_values = q_df.iloc[:, 1]
        x_values = q_df.iloc[:, 2]
        y_values = q_df.iloc[:, 3]
        # sort by y values
        x_values = [x for _, x in sorted(zip(y_values, x_values))]
        y_values = [y for y, _ in sorted(zip(y_values, x_values))]
        plt.plot(
            x_values,
            y_values,
            linestyle=LINE_STYLES[q],
            color="black",
            label=q,
            lw=2,
            zorder=3,
        )
        plt.plot(
            x2_values, y2_values, linestyle=LINE_STYLES[q], color="gray", lw=2, zorder=3
        )

    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Free Join framework (s)")
    plt.ylabel("EJCO (s)")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend()

    path = LSQB_PLOTS_PATH / f"lsqb_all.pdf"
    plt.savefig(path, bbox_inches="tight")
