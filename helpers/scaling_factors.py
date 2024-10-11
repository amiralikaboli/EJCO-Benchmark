import os
import subprocess
import sys
from pathlib import Path
from typing import Final

from helpers.constants import DATASETS_DIR

SCALING_FACTORS: Final[tuple[float, ...]] = (0.1, 0.3, 1, 3)

LSQB_DATASETS_DIR: Final[str] = "lsqb_no_headers"  # replace this directory or set it up
LSQB_DATASETS_PATH: Final[Path] = Path(DATASETS_DIR) / LSQB_DATASETS_DIR


def link_dataset(sf: float) -> None:
    check_datasets()
    sf_dir = os.path.join(LSQB_DATASETS_DIR, sf_dir_fmt(sf))
    check_dataset(sf_dir)
    bash = f"ln -s {sf_dir} lsqb"
    subprocess.call(bash, shell=True, cwd=DATASETS_DIR)


def unlink_dataset() -> None:
    # TODO check is symlink before deleting
    bash = f"rm lsqb"
    subprocess.call(bash, shell=True, cwd=DATASETS_DIR)


def sf_dir_fmt(sf: float) -> str:
    return f"SF_{str(int(sf) if sf == int(sf) else sf)}"


def check_dataset(sf_dir: str) -> None:
    sf_path = Path(DATASETS_DIR) / sf_dir
    if not (sf_path.exists() and sf_path.is_dir()):
        sys.exit(f"Missing directory: {sf_dir}")


def check_datasets() -> None:
    if not (LSQB_DATASETS_PATH.exists() and LSQB_DATASETS_PATH.is_dir()):
        sys.exit("Set up LSQB datasets directory (all scaling factors, no csv headers)")
