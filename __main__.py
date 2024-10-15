from helpers.checks import check_progs
from helpers.constants import Source
from helpers.experiments import job_plots, lsqb_plots

# TOGGLE THIS HERE TO PRODUCE THE DESIRED PLOTS
SOURCE = Source.JOB


if __name__ == "__main__":
    check_progs()
    match SOURCE:
        case Source.JOB:
            job_plots()
        case Source.LSQB:
            lsqb_plots()
        case _:
            raise ValueError(f"Unknown source: {SOURCE}")
