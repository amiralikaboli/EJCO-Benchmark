import os
import re
import subprocess
from typing import Final

from helpers.checks import check_progs_diffs
from helpers.constants import ABLATIONS_DIR, JOB_FJ_DIR, ROOT_DIR

RE_ABLATION: Final[re.Pattern] = re.compile(r"^O(\d+)$")
ABLATIONS: Final[list[int]] = sorted(
    map(
        lambda x: int(RE_ABLATION.match(x).group(1)),
        (f.name for f in os.scandir(ABLATIONS_DIR) if f.is_dir()),
    )
)
NO_ABLATION: Final[int] = max(ABLATIONS)


def apply_ablation(ablation: int) -> None:
    copy_ablation(ablation)
    if not check_progs_diffs() == (ablation != NO_ABLATION):
        raise RuntimeError("sdql/progs unexpected git diff status")


def copy_ablation(ablation: int) -> None:
    bash = f"cp ablations/O{ablation}/*.sdql {JOB_FJ_DIR}"
    subprocess.call(bash, shell=True, cwd=ROOT_DIR)
