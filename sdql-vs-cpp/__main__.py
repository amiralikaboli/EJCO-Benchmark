import os
import statistics
from pathlib import Path
from typing import Final

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from helpers.constants import Algo, QUERY_COL, RUNTIME_COL, SECS_TO_MS, Sorting
from helpers.plots.shared import RATIO, get_colnames
from helpers.wcoj import read_job_result
from helpers.wcoj.shared import get_query_names_and_times

FILE_DIR: Final[str] = os.path.dirname(os.path.realpath(__file__))

CASES: Final[tuple[tuple[Algo, Sorting], ...]] = (
    (Algo.FJ, Sorting.SORTING),
    (Algo.FJ, Sorting.HYBRID),
    (Algo.GJ, Sorting.HYBRID),
)


def get_data_dir(algo: Algo, sorting: Sorting) -> str:
    return os.path.join(FILE_DIR, sorting.value.upper(), f"{algo.value}_results")


def plot(df: pd.DataFrame, algo: Algo, sorting: Sorting, legend: str):
    plt.figure()
    df /= SECS_TO_MS
    df.drop(df.columns[-1], axis=1, inplace=True)
    eye_line = [np.min(df) / RATIO, np.max(df) * RATIO]
    plt.plot(eye_line, eye_line, color="gray", lw=0.5)
    x_values = df[df.columns[0]].values
    y_values = df[df.columns[1]].values
    plt.scatter(x_values, y_values, color="black", s=10, label=legend, zorder=3)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel(f"C++")
    plt.ylabel(f"SDQL")
    plt.xlim(eye_line)
    plt.ylim(eye_line)
    plt.legend(loc="upper center", ncol=1, bbox_to_anchor=(0.5, 1.25))
    path = Path(FILE_DIR) / f"{algo.value}_{sorting.value}_sdql-vs_cpp.pdf"
    plt.savefig(path, bbox_inches="tight")
    plt.close()


if __name__ == "__main__":
    for algo, sorting in CASES:
        # assume results in main codebase were generated in C++
        cpp = read_job_result(algo=algo, sorting=sorting)
        cpp.rename(columns={RUNTIME_COL: "C++"}, inplace=True)

        # and results in the local directory were generated in SDQL
        data_dir = get_data_dir(algo, sorting)
        times = get_query_names_and_times(data_dir)
        sdql = pd.DataFrame(times, columns=[QUERY_COL, RUNTIME_COL])
        sdql.rename(columns={RUNTIME_COL: "SDQL"}, inplace=True)

        _, legend = get_colnames(False, algo, sorting)
        print(f"\n{legend}")

        df = pd.merge(cpp, sdql, on=QUERY_COL, how="inner")
        df.set_index(QUERY_COL, inplace=True)
        df["Performance Improvement"] = (df["C++"] / df["SDQL"]).round(2)
        df.sort_values(by="Performance Improvement", ascending=False, inplace=True)
        geometric_mean = statistics.geometric_mean(df["Performance Improvement"])
        geometric_mean = round(geometric_mean, 2)
        print("Geometric Mean", geometric_mean)
        print(df)

        plot(df, algo=algo, sorting=sorting, legend=legend)
