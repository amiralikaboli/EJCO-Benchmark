import os
import re
import subprocess
from typing import Final, NewType

from helpers.checks import check_progs_diffs
from helpers.constants import ABLATIONS_DIR, JOB_FJ_DIR, ROOT_DIR

Ablation = NewType("Ablation", int)

RE_ABLATION: Final[re.Pattern] = re.compile(r"^O(\d+)$")
ABLATIONS: Final[list[Ablation]] = sorted(
    map(
        lambda x: Ablation(int(RE_ABLATION.match(x).group(1))),
        (f.name for f in os.scandir(ABLATIONS_DIR) if f.is_dir()),
    )
)
NO_ABLATION: Final[Ablation] = max(ABLATIONS)


def apply_ablation(ablation: Ablation) -> None:
    copy_ablation(ablation)
    if not check_progs_diffs() == (ablation != NO_ABLATION):
        raise RuntimeError("sdql/progs unexpected git diff status")


def copy_ablation(ablation: Ablation) -> None:
    bash = f"cp ablations/O{ablation}/*.sdql {JOB_FJ_DIR}"
    subprocess.call(bash, shell=True, cwd=ROOT_DIR)
