from functools import reduce
from pathlib import Path

import pandas as pd

from helpers.ablations import ABLATIONS, NO_ABLATION
from helpers.checks import check_progs
from helpers.constants import Algo, QUERY_COL, RUNTIME_COL, JOB_TIMINGS_DIR
from helpers.free_join import read_job_result
from helpers.plot import plot, violin_plot, ablation_plot
from helpers.wcoj import read_wcoj_result

if __name__ == "__main__":
    check_progs()
    names = (
        [
            "GJ (free-join)",
            "FJ (scalar)",
            "FJ (vector)",
            "GJ",
        ]
        + [f"O{ablation}" for ablation in ABLATIONS if ablation != NO_ABLATION]
        + ["FJ"]
    )
    results = (
        [
            read_job_result(algo=Algo.GJ),
            read_job_result(algo=Algo.FJ, vectorised=False),
            read_job_result(algo=Algo.FJ, vectorised=True),
            read_wcoj_result(algo=Algo.GJ),
        ]
        + [
            read_wcoj_result(algo=Algo.FJ, ablation=ablation)
            for ablation in ABLATIONS
            if ablation != NO_ABLATION
        ]
        + [read_wcoj_result(algo=Algo.FJ, ablation=NO_ABLATION)]
    )
    df = reduce(
        lambda l, r: pd.merge(l, r, on=QUERY_COL),
        (df.rename(columns={RUNTIME_COL: name}) for name, df in zip(names, results)),
    )
    print(df)
    df.to_csv(Path(JOB_TIMINGS_DIR) / "overview.csv", index=False)
    plot(df, Algo.GJ)
    plot(df, Algo.FJ, vectorised=False)
    plot(df, Algo.FJ, vectorised=True)
    violin_plot(df)
    ablation_plot(df, ["9d", "12b", "16b", "19d"])
