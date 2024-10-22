from typing import Annotated

import typer

from helpers.benchmarks import job_plots, lsqb_plots
from helpers.checks import check_progs
from helpers.constants import Benchmark


def main(
    benchmark: Annotated[Benchmark, typer.Option(case_sensitive=False)] = Benchmark.JOB
):
    check_progs()
    match benchmark:
        case Benchmark.JOB:
            job_plots()
        case Benchmark.LSQB:
            lsqb_plots()
        case _:
            raise ValueError(f"Unknown experiment: {benchmark}")


if __name__ == "__main__":
    typer.run(main)
