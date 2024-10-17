#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document T_CSV("../datasets/job/22a/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* kind_id */ T_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX_CSV("../datasets/job/22a/mi_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx = tuple(/* movie_id */ MI_IDX_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX_CSV.GetColumn<int>(2), /* info */ MI_IDX_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX_CSV.GetRowCount()));
const rapidcsv::Document KT_CSV("../datasets/job/22a/kt.csv", NO_HEADERS, SEPARATOR);
auto kt = tuple(/* id */ KT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT_CSV.GetRowCount()));
const rapidcsv::Document IT2_CSV("../datasets/job/22a/it2.csv", NO_HEADERS, SEPARATOR);
auto it2 = tuple(/* id */ IT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT2_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/22a/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/22a/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* name */ CN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/company_type.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/22a/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/22a/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/22a/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));

int main() {
vector<int> kt_offsets(get<1>(kt));
iota(kt_offsets.begin(), kt_offsets.end(), 0);
sort(kt_offsets.begin(), kt_offsets.end(), [&](const int i, const int j) { return get<0>(kt)[i] < get<0>(kt)[j]; });
vector<int> mi_idx_offsets(get<3>(mi_idx));
iota(mi_idx_offsets.begin(), mi_idx_offsets.end(), 0);
sort(mi_idx_offsets.begin(), mi_idx_offsets.end(), [&](const int i, const int j) { return get<0>(mi_idx)[i] < get<0>(mi_idx)[j]; });
vector<int> it2_offsets(get<1>(it2));
iota(it2_offsets.begin(), it2_offsets.end(), 0);
sort(it2_offsets.begin(), it2_offsets.end(), [&](const int i, const int j) { return get<0>(it2)[i] < get<0>(it2)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<2>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> it1_offsets(get<1>(it1));
iota(it1_offsets.begin(), it1_offsets.end(), 0);
sort(it1_offsets.begin(), it1_offsets.end(), [&](const int i, const int j) { return get<0>(it1)[i] < get<0>(it1)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> kt_trie0(get<1>(kt));
for (int i = 0; i < get<1>(kt); ++i)
kt_trie0[get<0>(kt)[kt_offsets[i]]] += 1;
SortedDict<int, Range> mi_idx_trie0(get<3>(mi_idx));
for (int i = 0; i < get<3>(mi_idx); ++i)
mi_idx_trie0[get<0>(mi_idx)[mi_idx_offsets[i]]][i] += 1;
SortedDict<int, int> it2_trie0(get<1>(it2));
for (int i = 0; i < get<1>(it2); ++i)
it2_trie0[get<0>(it2)[it2_offsets[i]]] += 1;
vector<tuple<int, int, string, int, string>> interm0;
for (auto t_off = 0; t_off < get<3>(t); ++t_off) {
const auto x0 = get<2>(t)[t_off];
const auto kt_it = kt_trie0.find(x0);
if (kt_it != kt_trie0.end()) {
const auto &kt_trie1 = kt_it->second;
const auto x1 = get<0>(t)[t_off];
const auto mi_idx_it = mi_idx_trie0.find(x1);
if (mi_idx_it != mi_idx_trie0.end()) {
const auto &mi_idx_trie1 = mi_idx_it->second;
for (auto mi_idx_i = mi_idx_trie1.left(); mi_idx_i < mi_idx_trie1.right(); ++mi_idx_i) {
const auto mi_idx_off = mi_idx_offsets[mi_idx_i];
const auto x2 = get<1>(mi_idx)[mi_idx_off];
const auto it2_it = it2_trie0.find(x2);
if (it2_it != it2_trie0.end()) {
const auto &it2_trie1 = it2_it->second;
interm0.emplace_back(get<0>(t)[t_off], get<2>(t)[t_off], get<1>(t)[t_off], get<1>(mi_idx)[mi_idx_off], get<2>(mi_idx)[mi_idx_off]);}
}
}
}
}
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int, string, int, string> &a, const tuple<int, int, string, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, Range> cn_trie0(get<2>(cn));
for (int i = 0; i < get<2>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]][i] += 1;
vector<tuple<int, int, int, int, string, string, string>> interm1;
for (auto mc_off = 0; mc_off < get<3>(mc); ++mc_off) {
const auto x0 = get<2>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x0);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
const auto x1 = get<0>(mc)[mc_off];
const auto interm0_it = interm0_trie0.find(x1);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto x2 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x2);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
for (auto cn_i = cn_trie1.left(); cn_i < cn_trie1.right(); ++cn_i) {
const auto cn_off = cn_offsets[cn_i];
for (auto interm0_off = interm0_trie1.left(); interm0_off < interm0_trie1.right(); ++interm0_off) {
const auto &interm0_tuple = interm0[interm0_off];
interm1.emplace_back(get<0>(mc)[mc_off], get<1>(mc)[mc_off], get<2>(mc)[mc_off], get<0>(mc)[mc_off], get<1>(cn)[cn_off], get<2>(interm0_tuple), get<4>(interm0_tuple));}
}
}
}
}
}
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
sort(interm1.begin(), interm1.end(), [](const tuple<int, int, int, int, string, string, string> &a, const tuple<int, int, int, int, string, string, string> &b) { return get<0>(a) < get<0>(b); });
auto interm1_trie0 = SortedDict<int, Range>(interm1.size());
for (int i = 0; i < interm1.size(); ++i)
interm1_trie0[get<0>(interm1[i])][i] += 1;
vector<tuple<int, int, int, string, string, string>> interm2;
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x0);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x1 = get<0>(mk)[mk_off];
const auto interm1_it = interm1_trie0.find(x1);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
for (auto interm1_off = interm1_trie1.left(); interm1_off < interm1_trie1.right(); ++interm1_off) {
const auto &interm1_tuple = interm1[interm1_off];
interm2.emplace_back(get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<0>(mk)[mk_off], get<4>(interm1_tuple), get<5>(interm1_tuple), get<6>(interm1_tuple));}
}
}
}
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
sort(interm2.begin(), interm2.end(), [](const tuple<int, int, int, string, string, string> &a, const tuple<int, int, int, string, string, string> &b) { return get<0>(a) < get<0>(b); });
auto interm2_trie0 = SortedDict<int, Range>(interm2.size());
for (int i = 0; i < interm2.size(); ++i)
interm2_trie0[get<0>(interm2[i])][i] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<1>(mi)[mi_off];
const auto it1_it = it1_trie0.find(x0);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
const auto x1 = get<0>(mi)[mi_off];
const auto interm2_it = interm2_trie0.find(x1);
if (interm2_it != interm2_trie0.end()) {
auto &interm2_trie1 = interm2_it->second;
auto mn_interm2 = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto interm2_off = interm2_trie1.left(); interm2_off < interm2_trie1.right(); ++interm2_off) {
const auto &interm2_tuple = interm2[interm2_off];
min_inplace(mn_interm2, {get<3>(interm2_tuple), get<4>(interm2_tuple), get<5>(interm2_tuple)});
}
min_inplace(res, {get<0>(mn_interm2), get<1>(mn_interm2), get<2>(mn_interm2)});
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}