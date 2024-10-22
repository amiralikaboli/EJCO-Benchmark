from typing import Annotated

import typer

from helpers.checks import check_progs
from helpers.constants import Experiment
from helpers.experiments import job_plots, lsqb_plots


def main(
    experiment: Annotated[
        Experiment, typer.Option(case_sensitive=False)
    ] = Experiment.JOB
):
    check_progs()
    match experiment:
        case Experiment.JOB:
            job_plots()
        case Experiment.LSQB:
            lsqb_plots()
        case _:
            raise ValueError(f"Unknown experiment: {experiment}")


if __name__ == "__main__":
    typer.run(main)
