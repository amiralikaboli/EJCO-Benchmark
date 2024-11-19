import os
import statistics
from pathlib import Path
from typing import Final

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from helpers import free_join
from helpers.constants import Algo, QUERY_COL, RUNTIME_COL, SECS_TO_MS
from helpers.plots.shared import RATIO
from helpers.wcoj.shared import get_query_names_and_times

FILE_DIR: Final[str] = os.path.dirname(os.path.realpath(__file__))
DATA_DIR: Final[str] = os.path.join(FILE_DIR, "gj_times")

GJ_HYBRID_COL: Final[str] = "GJ sorting (hybrid)"
GJ_FREE_JOIN_COL: Final[str] = "GJ (free-join)"

if __name__ == "__main__":
    gj_free_join = free_join.read_job_result(algo=Algo.GJ)
    gj_free_join.rename(columns={RUNTIME_COL: GJ_FREE_JOIN_COL}, inplace=True)

    times = get_query_names_and_times(DATA_DIR)
    gj_hybrid = pd.DataFrame(times, columns=[QUERY_COL, RUNTIME_COL])
    gj_hybrid.rename(columns={RUNTIME_COL: GJ_HYBRID_COL}, inplace=True)

    df = pd.merge(gj_free_join, gj_hybrid, on=QUERY_COL, how="inner")
    df.set_index(QUERY_COL, inplace=True)

    df["Performance Improvement"] = (df[GJ_FREE_JOIN_COL] / df[GJ_HYBRID_COL]).round(2)
    df.sort_values(by="Performance Improvement", ascending=False, inplace=True)

    geometric_mean = round(statistics.geometric_mean(df["Performance Improvement"]), 2)
    print("Geometric Mean", geometric_mean)

    print(df)
    legend_str = "Generic Join"

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

    path = Path(FILE_DIR) / f"job_gj_hybrid.pdf"
    plt.savefig(path, bbox_inches="tight")
