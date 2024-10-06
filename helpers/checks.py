import subprocess
import sys
from pathlib import Path
from typing import Final

from helpers.constants import PROGS_DIR, PROGS_SYM

PROGS_PATH: Final[Path] = Path(PROGS_SYM)


def check_progs() -> None:
    if not (PROGS_PATH.is_symlink() and PROGS_PATH.exists() and PROGS_PATH.is_dir()):
        sys.exit(
            (
                "Follow instructions in README "
                "to create a symlink to the 'sdql/progs' directory"
            )
        )


def check_progs_diffs() -> bool:
    kwargs = dict(shell=True, stdout=subprocess.PIPE, text=True, cwd=PROGS_DIR)
    stdout = subprocess.run("git diff --name-only job/fj", **kwargs).stdout
    return bool(stdout)
