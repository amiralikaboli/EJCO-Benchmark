#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document T_CSV("../datasets/job/18b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX_CSV("../datasets/job/18b/mi_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx = tuple(/* movie_id */ MI_IDX_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX_CSV.GetColumn<int>(2), /* info */ MI_IDX_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/18b/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* info */ MI_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document IT2_CSV("../datasets/job/18b/it2.csv", NO_HEADERS, SEPARATOR);
auto it2 = tuple(/* id */ IT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT2_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/18b/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/18b/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/18b/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* size */ static_cast<int>(N_CSV.GetRowCount()));

int main() {
vector<int> mi_idx_offsets(get<3>(mi_idx));
iota(mi_idx_offsets.begin(), mi_idx_offsets.end(), 0);
sort(mi_idx_offsets.begin(), mi_idx_offsets.end(), [&](const int i, const int j) { return get<0>(mi_idx)[i] < get<0>(mi_idx)[j]; });
vector<int> it2_offsets(get<1>(it2));
iota(it2_offsets.begin(), it2_offsets.end(), 0);
sort(it2_offsets.begin(), it2_offsets.end(), [&](const int i, const int j) { return get<0>(it2)[i] < get<0>(it2)[j]; });
vector<int> it1_offsets(get<1>(it1));
iota(it1_offsets.begin(), it1_offsets.end(), 0);
sort(it1_offsets.begin(), it1_offsets.end(), [&](const int i, const int j) { return get<0>(it1)[i] < get<0>(it1)[j]; });
vector<int> n_offsets(get<1>(n));
iota(n_offsets.begin(), n_offsets.end(), 0);
sort(n_offsets.begin(), n_offsets.end(), [&](const int i, const int j) { return get<0>(n)[i] < get<0>(n)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> mi_idx_trie0(get<3>(mi_idx));
for (int i = 0; i < get<3>(mi_idx); ++i)
mi_idx_trie0[get<0>(mi_idx)[mi_idx_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<string, int, string>, 4>>(min(1000000, get<2>(t)));
for (auto t_off = 0; t_off < get<2>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto mi_idx_it = mi_idx_trie0.find(x0);
if (mi_idx_it != mi_idx_trie0.end()) {
const auto &mi_idx_trie1 = mi_idx_it->second;
for (auto mi_idx_i = mi_idx_trie1.left(); mi_idx_i < mi_idx_trie1.right(); ++mi_idx_i) {
const auto mi_idx_off = mi_idx_offsets[mi_idx_i];
interm0_trie0[get<0>(t)[t_off]][tuple<string, int, string>(get<1>(t)[t_off], get<1>(mi_idx)[mi_idx_off], get<2>(mi_idx)[mi_idx_off])] += 1;
}
}
}
SortedDict<int, int> it2_trie0(get<1>(it2));
for (int i = 0; i < get<1>(it2); ++i)
it2_trie0[get<0>(it2)[it2_offsets[i]]] += 1;
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
auto interm1_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string, int, string, string>, 4>>(min(1000000, get<3>(mi)));
for (auto mi_off = 0; mi_off < get<3>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (const auto &interm0_tuple: interm0_trie1) {
const auto x1 = get<1>(interm0_tuple);
const auto it2_it = it2_trie0.find(x1);
if (it2_it != it2_trie0.end()) {
const auto &it2_trie1 = it2_it->second;
const auto x2 = get<1>(mi)[mi_off];
const auto it1_it = it1_trie0.find(x2);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
interm1_trie0[get<0>(mi)[mi_off]][tuple<int, string, int, string, string>(get<1>(mi)[mi_off], get<2>(mi)[mi_off], get<1>(interm0_tuple), get<0>(interm0_tuple), get<2>(interm0_tuple))] += 1;
}
}
}
}
}
SortedDict<int, int> n_trie0(get<1>(n));
for (int i = 0; i < get<1>(n); ++i)
n_trie0[get<0>(n)[n_offsets[i]]] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<2>(ci); ++ci_off) {
const auto x0 = get<1>(ci)[ci_off];
const auto interm1_it = interm1_trie0.find(x0);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto x1 = get<0>(ci)[ci_off];
const auto n_it = n_trie0.find(x1);
if (n_it != n_trie0.end()) {
const auto &n_trie1 = n_it->second;
auto mn_interm1 = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (const auto &interm1_tuple: interm1_trie1) {
min_inplace(mn_interm1, {get<1>(interm1_tuple), get<3>(interm1_tuple), get<4>(interm1_tuple)});
}
min_inplace(res, {get<0>(mn_interm1), get<1>(mn_interm1), get<2>(mn_interm1)});
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}