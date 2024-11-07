import os
import re
from collections.abc import Callable
from pathlib import Path
from typing import Any, Final

import pandas as pd

from helpers.constants import QUERY_COL, RUNTIME_COL

DTYPES: Final[dict[str, Any]] = {QUERY_COL: "string", RUNTIME_COL: int}

RE_RUNTIME: Final[re.Pattern] = re.compile(r"(\d+\.?\d*]?) ms")


def write_results_frame(data_dir: str, output_csv: str) -> None:
    df = pd.DataFrame(
        get_query_names_and_times(data_dir), columns=[QUERY_COL, RUNTIME_COL]
    )
    df.to_csv(output_csv, index=False)


def get_query_names_and_times(
    data_dir: str, file_filter: Callable[[str], bool] = lambda _: True
) -> list[tuple[str, int]]:
    files = get_files(data_dir, file_filter)
    query_names = get_query_names(files)
    times = [
        get_ms(Path(os.path.join(data_dir, f)).read_text(), RE_RUNTIME) for f in files
    ]
    return list(zip(query_names, times))


def get_query_names(files: list[str]) -> list[str]:
    return [f.split(".", 1)[0] for f in files]


def get_files(
    data_dir: str, file_filter: Callable[[str], bool] = lambda _: True
) -> list[str]:
    return sorted(
        filter(
            file_filter,
            (f for f in next(os.walk(data_dir))[2] if f.endswith(".result")),
        ),
        key=lambda f: "0" + f if len(f) == len("__.result") else f,
    )


def get_ms(s: str, regex: re.Pattern) -> int:
    return round(float(regex.search(s).group(1)))
