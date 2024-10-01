import os
import sys
from pathlib import Path
from typing import Final

from helpers.constants import ROOT_DIR

PROGS: Final[Path] = Path(os.path.join(ROOT_DIR, "progs"))

MSG: Final[str] = (
    "Follow instructions in README to create a symlink to the 'sdql/progs' directory"
)


def check_progs():
    if not (PROGS.is_symlink() and PROGS.exists() and PROGS.is_dir()):
        sys.exit(MSG)
