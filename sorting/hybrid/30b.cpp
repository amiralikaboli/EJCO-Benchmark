#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document CC_CSV("../datasets/job/complete_cast.csv", NO_HEADERS, SEPARATOR);
auto cc = tuple(/* movie_id */ CC_CSV.GetColumn<int>(1), /* subject_id */ CC_CSV.GetColumn<int>(2), /* status_id */ CC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CC_CSV.GetRowCount()));
const rapidcsv::Document CCT1_CSV("../datasets/job/30b/cct1.csv", NO_HEADERS, SEPARATOR);
auto cct1 = tuple(/* id */ CCT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT1_CSV.GetRowCount()));
const rapidcsv::Document CCT2_CSV("../datasets/job/30b/cct2.csv", NO_HEADERS, SEPARATOR);
auto cct2 = tuple(/* id */ CCT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT2_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX_CSV("../datasets/job/movie_info_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx = tuple(/* movie_id */ MI_IDX_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX_CSV.GetColumn<int>(2), /* info */ MI_IDX_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX_CSV.GetRowCount()));
const rapidcsv::Document IT2_CSV("../datasets/job/30b/it2.csv", NO_HEADERS, SEPARATOR);
auto it2 = tuple(/* id */ IT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT2_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/30b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/30b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/30b/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* info */ MI_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/30b/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/30b/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/30b/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* name */ N_CSV.GetColumn<string>(1), /* size */ static_cast<int>(N_CSV.GetRowCount()));

int main() {
vector<int> cct2_offsets(get<1>(cct2));
iota(cct2_offsets.begin(), cct2_offsets.end(), 0);
sort(cct2_offsets.begin(), cct2_offsets.end(), [&](const int i, const int j) { return get<0>(cct2)[i] < get<0>(cct2)[j]; });
vector<int> cct1_offsets(get<1>(cct1));
iota(cct1_offsets.begin(), cct1_offsets.end(), 0);
sort(cct1_offsets.begin(), cct1_offsets.end(), [&](const int i, const int j) { return get<0>(cct1)[i] < get<0>(cct1)[j]; });
vector<int> it2_offsets(get<1>(it2));
iota(it2_offsets.begin(), it2_offsets.end(), 0);
sort(it2_offsets.begin(), it2_offsets.end(), [&](const int i, const int j) { return get<0>(it2)[i] < get<0>(it2)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> it1_offsets(get<1>(it1));
iota(it1_offsets.begin(), it1_offsets.end(), 0);
sort(it1_offsets.begin(), it1_offsets.end(), [&](const int i, const int j) { return get<0>(it1)[i] < get<0>(it1)[j]; });
vector<int> n_offsets(get<2>(n));
iota(n_offsets.begin(), n_offsets.end(), 0);
sort(n_offsets.begin(), n_offsets.end(), [&](const int i, const int j) { return get<0>(n)[i] < get<0>(n)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> cct2_trie0(get<1>(cct2));
for (int i = 0; i < get<1>(cct2); ++i)
cct2_trie0[get<0>(cct2)[cct2_offsets[i]]] += 1;
SortedDict<int, int> cct1_trie0(get<1>(cct1));
for (int i = 0; i < get<1>(cct1); ++i)
cct1_trie0[get<0>(cct1)[cct1_offsets[i]]] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int>, 4>>(min(1000000, get<3>(cc)));
for (auto cc_off = 0; cc_off < get<3>(cc); ++cc_off) {
const auto x0 = get<2>(cc)[cc_off];
const auto cct2_it = cct2_trie0.find(x0);
if (cct2_it != cct2_trie0.end()) {
const auto &cct2_trie1 = cct2_it->second;
const auto x1 = get<1>(cc)[cc_off];
const auto cct1_it = cct1_trie0.find(x1);
if (cct1_it != cct1_trie0.end()) {
const auto &cct1_trie1 = cct1_it->second;
interm0_trie0[get<0>(cc)[cc_off]][tuple<int, int>(get<1>(cc)[cc_off], get<2>(cc)[cc_off])] += 1;
}
}
}
SortedDict<int, int> it2_trie0(get<1>(it2));
for (int i = 0; i < get<1>(it2); ++i)
it2_trie0[get<0>(it2)[it2_offsets[i]]] += 1;
auto interm1_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string>, 4>>(min(1000000, get<3>(mi_idx)));
for (auto mi_idx_off = 0; mi_idx_off < get<3>(mi_idx); ++mi_idx_off) {
const auto x0 = get<1>(mi_idx)[mi_idx_off];
const auto it2_it = it2_trie0.find(x0);
if (it2_it != it2_trie0.end()) {
const auto &it2_trie1 = it2_it->second;
const auto x1 = get<0>(mi_idx)[mi_idx_off];
const auto interm0_it = interm0_trie0.find(x1);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
interm1_trie0[get<0>(mi_idx)[mi_idx_off]][tuple<int, string>(get<1>(mi_idx)[mi_idx_off], get<2>(mi_idx)[mi_idx_off])] += 1;
}
}
}
auto interm2_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<string, string>, 4>>(min(1000000, get<2>(t)));
for (auto t_off = 0; t_off < get<2>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto interm1_it = interm1_trie0.find(x0);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
for (const auto &interm1_tuple: interm1_trie1) {
interm2_trie0[get<0>(t)[t_off]][tuple<string, string>(get<1>(t)[t_off], get<1>(interm1_tuple))] += 1;
}
}
}
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
auto interm3_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string, string>, 4>>(min(1000000, get<2>(mk)));
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto interm2_it = interm2_trie0.find(x0);
if (interm2_it != interm2_trie0.end()) {
auto &interm2_trie1 = interm2_it->second;
const auto x1 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x1);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
for (const auto &interm2_tuple: interm2_trie1) {
interm3_trie0[get<0>(mk)[mk_off]][tuple<int, string, string>(get<1>(mk)[mk_off], get<0>(interm2_tuple), get<1>(interm2_tuple))] += 1;
}
}
}
}
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
auto interm4_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string, string, string>, 4>>(min(1000000, get<3>(mi)));
for (auto mi_off = 0; mi_off < get<3>(mi); ++mi_off) {
const auto x0 = get<1>(mi)[mi_off];
const auto it1_it = it1_trie0.find(x0);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
const auto x1 = get<0>(mi)[mi_off];
const auto interm3_it = interm3_trie0.find(x1);
if (interm3_it != interm3_trie0.end()) {
auto &interm3_trie1 = interm3_it->second;
for (const auto &interm3_tuple: interm3_trie1) {
interm4_trie0[get<0>(mi)[mi_off]][tuple<int, string, string, string>(get<1>(mi)[mi_off], get<2>(mi)[mi_off], get<1>(interm3_tuple), get<2>(interm3_tuple))] += 1;
}
}
}
}
SortedDict<int, Range> n_trie0(get<2>(n));
for (int i = 0; i < get<2>(n); ++i)
n_trie0[get<0>(n)[n_offsets[i]]][i] += 1;
auto res = tuple<string, string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<2>(ci); ++ci_off) {
const auto x0 = get<1>(ci)[ci_off];
const auto interm4_it = interm4_trie0.find(x0);
if (interm4_it != interm4_trie0.end()) {
auto &interm4_trie1 = interm4_it->second;
const auto x1 = get<0>(ci)[ci_off];
const auto n_it = n_trie0.find(x1);
if (n_it != n_trie0.end()) {
const auto &n_trie1 = n_it->second;
auto mn_n = tuple<string>(STRING_MAX);
for (auto n_i = n_trie1.left(); n_i < n_trie1.right(); ++n_i) {
const auto n_off = n_offsets[n_i];
min_inplace(mn_n, {get<1>(n)[n_off]});
}
auto mn_interm4 = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (const auto &interm4_tuple: interm4_trie1) {
min_inplace(mn_interm4, {get<1>(interm4_tuple), get<2>(interm4_tuple), get<3>(interm4_tuple)});
}
min_inplace(res, {get<0>(mn_n), get<0>(mn_interm4), get<1>(mn_interm4), get<2>(mn_interm4)});
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << " | " << get<3>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}