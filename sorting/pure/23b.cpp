#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document CC_CSV("../datasets/job/complete_cast.csv", NO_HEADERS, SEPARATOR);
auto cc = tuple(/* movie_id */ CC_CSV.GetColumn<int>(1), /* status_id */ CC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CC_CSV.GetRowCount()));
const rapidcsv::Document CCT1_CSV("../datasets/job/23b/cct1.csv", NO_HEADERS, SEPARATOR);
auto cct1 = tuple(/* id */ CCT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT1_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/23b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* kind_id */ T_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document KT_CSV("../datasets/job/23b/kt.csv", NO_HEADERS, SEPARATOR);
auto kt = tuple(/* id */ KT_CSV.GetColumn<int>(0), /* kind */ KT_CSV.GetColumn<string>(1), /* size */ static_cast<int>(KT_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/company_type.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/23b/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/23b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/23b/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/23b/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));

int main() {
vector<int> cct1_offsets(get<1>(cct1));
iota(cct1_offsets.begin(), cct1_offsets.end(), 0);
sort(cct1_offsets.begin(), cct1_offsets.end(), [&](const int i, const int j) { return get<0>(cct1)[i] < get<0>(cct1)[j]; });
vector<int> kt_offsets(get<2>(kt));
iota(kt_offsets.begin(), kt_offsets.end(), 0);
sort(kt_offsets.begin(), kt_offsets.end(), [&](const int i, const int j) { return get<0>(kt)[i] < get<0>(kt)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<1>(cn));
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
SortedDict<int, int> cct1_trie0(get<1>(cct1));
for (int i = 0; i < get<1>(cct1); ++i)
cct1_trie0[get<0>(cct1)[cct1_offsets[i]]] += 1;
vector<tuple<int, int>> interm0;
for (auto cc_off = 0; cc_off < get<2>(cc); ++cc_off) {
const auto x0 = get<1>(cc)[cc_off];
const auto cct1_it = cct1_trie0.find(x0);
if (cct1_it != cct1_trie0.end()) {
const auto &cct1_trie1 = cct1_it->second;
interm0.emplace_back(get<0>(cc)[cc_off], get<1>(cc)[cc_off]);}
}
SortedDict<int, Range> kt_trie0(get<2>(kt));
for (int i = 0; i < get<2>(kt); ++i)
kt_trie0[get<0>(kt)[kt_offsets[i]]][i] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int> &a, const tuple<int, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
vector<tuple<int, int, int, string, string>> interm1;
for (auto t_off = 0; t_off < get<3>(t); ++t_off) {
const auto x0 = get<2>(t)[t_off];
const auto kt_it = kt_trie0.find(x0);
if (kt_it != kt_trie0.end()) {
const auto &kt_trie1 = kt_it->second;
const auto x1 = get<0>(t)[t_off];
const auto interm0_it = interm0_trie0.find(x1);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (auto kt_i = kt_trie1.left(); kt_i < kt_trie1.right(); ++kt_i) {
const auto kt_off = kt_offsets[kt_i];
interm1.emplace_back(get<0>(t)[t_off], get<2>(t)[t_off], get<0>(t)[t_off], get<1>(t)[t_off], get<1>(kt)[kt_off]);}
}
}
}
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
sort(interm1.begin(), interm1.end(), [](const tuple<int, int, int, string, string> &a, const tuple<int, int, int, string, string> &b) { return get<0>(a) < get<0>(b); });
auto interm1_trie0 = SortedDict<int, Range>(interm1.size());
for (int i = 0; i < interm1.size(); ++i)
interm1_trie0[get<0>(interm1[i])][i] += 1;
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
vector<tuple<int, int, int, int, string, string>> interm2;
for (auto mc_off = 0; mc_off < get<3>(mc); ++mc_off) {
const auto x0 = get<2>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x0);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
const auto x1 = get<0>(mc)[mc_off];
const auto interm1_it = interm1_trie0.find(x1);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto x2 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x2);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
for (auto interm1_off = interm1_trie1.left(); interm1_off < interm1_trie1.right(); ++interm1_off) {
const auto &interm1_tuple = interm1[interm1_off];
interm2.emplace_back(get<0>(mc)[mc_off], get<2>(mc)[mc_off], get<1>(mc)[mc_off], get<0>(mc)[mc_off], get<3>(interm1_tuple), get<4>(interm1_tuple));}
}
}
}
}
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
sort(interm2.begin(), interm2.end(), [](const tuple<int, int, int, int, string, string> &a, const tuple<int, int, int, int, string, string> &b) { return get<0>(a) < get<0>(b); });
auto interm2_trie0 = SortedDict<int, Range>(interm2.size());
for (int i = 0; i < interm2.size(); ++i)
interm2_trie0[get<0>(interm2[i])][i] += 1;
vector<tuple<int, int, int, string, string>> interm3;
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x0);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x1 = get<0>(mk)[mk_off];
const auto interm2_it = interm2_trie0.find(x1);
if (interm2_it != interm2_trie0.end()) {
auto &interm2_trie1 = interm2_it->second;
for (auto interm2_off = interm2_trie1.left(); interm2_off < interm2_trie1.right(); ++interm2_off) {
const auto &interm2_tuple = interm2[interm2_off];
interm3.emplace_back(get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<0>(mk)[mk_off], get<4>(interm2_tuple), get<5>(interm2_tuple));}
}
}
}
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
sort(interm3.begin(), interm3.end(), [](const tuple<int, int, int, string, string> &a, const tuple<int, int, int, string, string> &b) { return get<0>(a) < get<0>(b); });
auto interm3_trie0 = SortedDict<int, Range>(interm3.size());
for (int i = 0; i < interm3.size(); ++i)
interm3_trie0[get<0>(interm3[i])][i] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<1>(mi)[mi_off];
const auto it1_it = it1_trie0.find(x0);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
const auto x1 = get<0>(mi)[mi_off];
const auto interm3_it = interm3_trie0.find(x1);
if (interm3_it != interm3_trie0.end()) {
auto &interm3_trie1 = interm3_it->second;
auto mn_interm3 = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto interm3_off = interm3_trie1.left(); interm3_off < interm3_trie1.right(); ++interm3_off) {
const auto &interm3_tuple = interm3[interm3_off];
min_inplace(mn_interm3, {get<3>(interm3_tuple), get<4>(interm3_tuple)});
}
min_inplace(res, {get<0>(mn_interm3), get<1>(mn_interm3)});
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}