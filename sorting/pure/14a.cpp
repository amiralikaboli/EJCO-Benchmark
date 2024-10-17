#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/14a/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* kind_id */ T_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX_CSV("../datasets/job/14a/mi_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx = tuple(/* movie_id */ MI_IDX_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX_CSV.GetColumn<int>(2), /* info */ MI_IDX_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/14a/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document KT_CSV("../datasets/job/14a/kt.csv", NO_HEADERS, SEPARATOR);
auto kt = tuple(/* id */ KT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/14a/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document IT2_CSV("../datasets/job/14a/it2.csv", NO_HEADERS, SEPARATOR);
auto it2 = tuple(/* id */ IT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT2_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/14a/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));

int main() {
vector<int> t_offsets(get<3>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> mi_idx_offsets(get<3>(mi_idx));
iota(mi_idx_offsets.begin(), mi_idx_offsets.end(), 0);
sort(mi_idx_offsets.begin(), mi_idx_offsets.end(), [&](const int i, const int j) { return get<0>(mi_idx)[i] < get<0>(mi_idx)[j]; });
vector<int> kt_offsets(get<1>(kt));
iota(kt_offsets.begin(), kt_offsets.end(), 0);
sort(kt_offsets.begin(), kt_offsets.end(), [&](const int i, const int j) { return get<0>(kt)[i] < get<0>(kt)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> it2_offsets(get<1>(it2));
iota(it2_offsets.begin(), it2_offsets.end(), 0);
sort(it2_offsets.begin(), it2_offsets.end(), [&](const int i, const int j) { return get<0>(it2)[i] < get<0>(it2)[j]; });
vector<int> it1_offsets(get<1>(it1));
iota(it1_offsets.begin(), it1_offsets.end(), 0);
sort(it1_offsets.begin(), it1_offsets.end(), [&](const int i, const int j) { return get<0>(it1)[i] < get<0>(it1)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> t_trie0(get<3>(t));
for (int i = 0; i < get<3>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, Range> mi_idx_trie0(get<3>(mi_idx));
for (int i = 0; i < get<3>(mi_idx); ++i)
mi_idx_trie0[get<0>(mi_idx)[mi_idx_offsets[i]]][i] += 1;
vector<tuple<int, int, string, int, int, string>> interm0;
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto t_it = t_trie0.find(x0);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
const auto mi_idx_it = mi_idx_trie0.find(x0);
if (mi_idx_it != mi_idx_trie0.end()) {
const auto &mi_idx_trie1 = mi_idx_it->second;
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
for (auto mi_idx_i = mi_idx_trie1.left(); mi_idx_i < mi_idx_trie1.right(); ++mi_idx_i) {
const auto mi_idx_off = mi_idx_offsets[mi_idx_i];
interm0.emplace_back(get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<1>(t)[t_off], get<2>(t)[t_off], get<1>(mi_idx)[mi_idx_off], get<2>(mi_idx)[mi_idx_off]);}
}
}
}
}
sort(interm0.begin(), interm0.end(), [](const tuple<int, int, string, int, int, string> &a, const tuple<int, int, string, int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, int> kt_trie0(get<1>(kt));
for (int i = 0; i < get<1>(kt); ++i)
kt_trie0[get<0>(kt)[kt_offsets[i]]] += 1;
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
SortedDict<int, int> it2_trie0(get<1>(it2));
for (int i = 0; i < get<1>(it2); ++i)
it2_trie0[get<0>(it2)[it2_offsets[i]]] += 1;
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (auto interm0_off = interm0_trie1.left(); interm0_off < interm0_trie1.right(); ++interm0_off) {
const auto &interm0_tuple = interm0[interm0_off];
const auto x1 = get<3>(interm0_tuple);
const auto kt_it = kt_trie0.find(x1);
if (kt_it != kt_trie0.end()) {
const auto &kt_trie1 = kt_it->second;
const auto &interm0_tuple = interm0[interm0_off];
const auto x2 = get<1>(interm0_tuple);
const auto k_it = k_trie0.find(x2);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto &interm0_tuple = interm0[interm0_off];
const auto x3 = get<4>(interm0_tuple);
const auto it2_it = it2_trie0.find(x3);
if (it2_it != it2_trie0.end()) {
const auto &it2_trie1 = it2_it->second;
const auto x4 = get<1>(mi)[mi_off];
const auto it1_it = it1_trie0.find(x4);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
auto mn_interm0 = tuple<string, string>(get<2>(interm0_tuple), get<5>(interm0_tuple));
min_inplace(res, {get<0>(mn_interm0), get<1>(mn_interm0)});
}
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