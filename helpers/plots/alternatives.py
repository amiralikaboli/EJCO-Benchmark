import pandas as pd

from helpers.constants import Algo, Sorting
from helpers.plots.shared import get_colnames


def fj_hash_vs_sort_vs_hybrid(
    df: pd.DataFrame,
    sortings: tuple[Sorting | None, Sorting | None, Sorting | None],
    include_counter_examples: bool = False,
    speedup_cutoff: float = 1.10,
    query_ms_cutoff: float = 50,
) -> pd.DataFrame:
    assert len(sortings) == len(set(sortings)), "should be distinct"
    colnames = [get_colnames(False, algo=Algo.FJ, sorting=s)[1] for s in sortings]
    df = df[colnames].copy()

    lhs_speedup = (df.iloc[:, 1] / df.iloc[:, 0]).round(2)
    df.insert(1, "<- speedup", lhs_speedup)
    rhs_speedup = (df.iloc[:, 3] / df.iloc[:, 2]).round(2)
    df.insert(3, "speedup <-", rhs_speedup)

    # ALL query runtimes have to be above a minimum threshold
    df = df[(df.iloc[:, 0] >= query_ms_cutoff) & (df.iloc[:, 2] >= query_ms_cutoff)]
    # SOME query speedup has to be above the minimum threshold
    df = df[(df.iloc[:, 1] >= speedup_cutoff) | (df.iloc[:, 3] >= speedup_cutoff)]

    df["lhs_speedup > 1"] = lhs_speedup > 1
    df["rhs_speedup > 1"] = rhs_speedup > 1
    df.sort_values(by=df.columns[[5, 6, 1, 3]].tolist(), ascending=False, inplace=True)
    if not include_counter_examples:
        df = df[df.iloc[:, 5] & df.iloc[:, 6]]
    df.drop(columns=df.columns[-2:], inplace=True)

    return df


def find_variants(
    lhs: pd.Index, rhs: pd.Index
) -> dict[int, tuple[list[str], list[str]]]:
    lhs_queries = set(lhs)
    rhs_queries = set(rhs)
    lhs_n = set(int(q[:-1]) for q in lhs_queries)
    rhs_n = set(int(q[:-1]) for q in rhs_queries)
    intersect = lhs_n & rhs_n

    variants = dict()
    for n in intersect:
        lhs_overlap = sorted(q for q in lhs_queries if q.startswith(str(n)))
        rhs_overlap = sorted(q for q in rhs_queries if q.startswith(str(n)))
        variants[n] = (lhs_overlap, rhs_overlap)

    return variants


def pick_queries(df: pd.DataFrame) -> tuple[str, str, str, str]:
    # queries 12b: hybrid > hash > sorting
    hybrid_hash_sort = fj_hash_vs_sort_vs_hybrid(
        df, (Sorting.HYBRID, None, Sorting.SORTING)
    )
    # 17f is valid, but we won't use it because 17b was incorrect
    assert "17f" in hybrid_hash_sort.index
    # keep 12b also because it appears in Fig 16 (this is mentioned in the paper)
    assert "12b" in hybrid_hash_sort.index
    # assert paper claim that it has the "highest speedups among all JOB queries"
    assert "12b" == hybrid_hash_sort.index[0]

    # new query 6d: sorting > hybrid > hash
    sort_hybrid_hash = fj_hash_vs_sort_vs_hybrid(
        df, (Sorting.SORTING, Sorting.HYBRID, None)
    )
    # 8a was an old incorrect query
    assert "8a" not in sort_hybrid_hash.index
    # 6d shows good speedups for both sorting > hybrid and hybrid > hash
    assert "6d" in sort_hybrid_hash.index

    # hash > sorting > hybrid
    hash_hybrid_sort = fj_hash_vs_sort_vs_hybrid(
        df, (None, Sorting.HYBRID, Sorting.SORTING)
    )
    # old incorrect query
    assert "17b" not in hash_hybrid_sort
    # find two variants of the same query (like 17b / 17f previously)
    variants = find_variants(hybrid_hash_sort.index, hash_hybrid_sort.index)
    is_variant = lambda idx: [any(x.startswith(str(n)) for n in variants) for x in idx]
    hybrid_hash_sort = hybrid_hash_sort[is_variant(hybrid_hash_sort.index)]
    hybrid_hash_sort.sort_index(inplace=True)
    hash_hybrid_sort = hash_hybrid_sort[is_variant(hash_hybrid_sort.index)]
    hash_hybrid_sort.sort_index(inplace=True)
    # 7a shows good speedups for both hybrid > hash and hash > sorting
    assert "7a" in hybrid_hash_sort.index
    # 7c shows good speedups for both hash > hybrid and hybrid > sorting
    assert "7c" in hash_hybrid_sort.index

    return "6d", "12b", "7a", "7c"
