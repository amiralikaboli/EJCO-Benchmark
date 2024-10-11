from functools import reduce
from typing import Iterable

import pandas as pd

from helpers.constants import QUERY_COL, RUNTIME_COL


def join_frames(names: Iterable[str], frames: Iterable[pd.DataFrame]) -> pd.DataFrame:
    return reduce(
        lambda l, r: pd.merge(l, r, on=QUERY_COL),
        (df.rename(columns={RUNTIME_COL: name}) for name, df in zip(names, frames)),
    )
