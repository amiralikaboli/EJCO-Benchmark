from functools import reduce
from typing import Iterable

import pandas as pd

from helpers.constants import QUERY_COL, RUNTIME_COL


def join_frames(names: Iterable[str], frames: Iterable[pd.DataFrame]) -> pd.DataFrame:
    df = reduce(
        lambda l, r: pd.merge(l, r, on=QUERY_COL, how="outer"),
        (df.rename(columns={RUNTIME_COL: name}) for name, df in zip(names, frames)),
    )
    df.set_index(QUERY_COL, inplace=True)
    # undo float casting while supporting NaNs
    df = df.astype(pd.Int64Dtype())
    return df
