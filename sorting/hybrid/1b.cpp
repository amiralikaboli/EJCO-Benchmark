#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document T_CSV("../datasets/job/1b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* production_year */ T_CSV.GetColumn<int>(4), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX_CSV("../datasets/job/movie_info_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx = tuple(/* movie_id */ MI_IDX_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_IDX_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/1b/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* note */ MC_CSV.GetColumn<string>(4), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/1b/it.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/1b/ct.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));

int main() {
vector<int> mi_idx_offsets(get<2>(mi_idx));
iota(mi_idx_offsets.begin(), mi_idx_offsets.end(), 0);
sort(mi_idx_offsets.begin(), mi_idx_offsets.end(), [&](const int i, const int j) { return get<0>(mi_idx)[i] < get<0>(mi_idx)[j]; });
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> mi_idx_trie0(get<2>(mi_idx));
for (int i = 0; i < get<2>(mi_idx); ++i)
mi_idx_trie0[get<0>(mi_idx)[mi_idx_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<string, int, int>, 4>>(min(1000000, get<3>(t)));
for (auto t_off = 0; t_off < get<3>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto mi_idx_it = mi_idx_trie0.find(x0);
if (mi_idx_it != mi_idx_trie0.end()) {
const auto &mi_idx_trie1 = mi_idx_it->second;
for (auto mi_idx_i = mi_idx_trie1.left(); mi_idx_i < mi_idx_trie1.right(); ++mi_idx_i) {
const auto mi_idx_off = mi_idx_offsets[mi_idx_i];
interm0_trie0[get<0>(t)[t_off]][tuple<string, int, int>(get<1>(t)[t_off], get<2>(t)[t_off], get<1>(mi_idx)[mi_idx_off])] += 1;
}
}
}
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
auto res = tuple<string, string, int>(STRING_MAX, STRING_MAX, INT_MAX);
for (auto mc_off = 0; mc_off < get<3>(mc); ++mc_off) {
const auto x0 = get<0>(mc)[mc_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (const auto &interm0_tuple: interm0_trie1) {
const auto x1 = get<2>(interm0_tuple);
const auto it_it = it_trie0.find(x1);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
const auto x2 = get<1>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x2);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
auto mn_mc = tuple<string>(get<2>(mc)[mc_off]);
auto mn_interm0 = tuple<string, int>(get<0>(interm0_tuple), get<1>(interm0_tuple));
min_inplace(res, {get<0>(mn_mc), get<0>(mn_interm0), get<1>(mn_interm0)});
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}