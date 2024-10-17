#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/4b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX_CSV("../datasets/job/4b/mi_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx = tuple(/* movie_id */ MI_IDX_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX_CSV.GetColumn<int>(2), /* info */ MI_IDX_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/4b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/4b/it.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));

int main() {
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> mi_idx_offsets(get<3>(mi_idx));
iota(mi_idx_offsets.begin(), mi_idx_offsets.end(), 0);
sort(mi_idx_offsets.begin(), mi_idx_offsets.end(), [&](const int i, const int j) { return get<0>(mi_idx)[i] < get<0>(mi_idx)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
SortedDict<int, Range> mi_idx_trie0(get<3>(mi_idx));
for (int i = 0; i < get<3>(mi_idx); ++i)
mi_idx_trie0[get<0>(mi_idx)[mi_idx_offsets[i]]][i] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x0);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x1 = get<0>(mk)[mk_off];
const auto mi_idx_it = mi_idx_trie0.find(x1);
if (mi_idx_it != mi_idx_trie0.end()) {
const auto &mi_idx_trie1 = mi_idx_it->second;
const auto t_it = t_trie0.find(x1);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto mi_idx_i = mi_idx_trie1.left(); mi_idx_i < mi_idx_trie1.right(); ++mi_idx_i) {
const auto mi_idx_off = mi_idx_offsets[mi_idx_i];
const auto x2 = get<1>(mi_idx)[mi_idx_off];
const auto it_it = it_trie0.find(x2);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
auto mn_t = tuple<string>(STRING_MAX);
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
min_inplace(mn_t, {get<1>(t)[t_off]});
}
auto mn_mi_idx = tuple<string>(get<2>(mi_idx)[mi_idx_off]);
min_inplace(res, {get<0>(mn_t), get<0>(mn_mi_idx)});
}
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}