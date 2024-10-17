#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document CC_CSV("../datasets/job/complete_cast.csv", NO_HEADERS, SEPARATOR);
auto cc = tuple(/* movie_id */ CC_CSV.GetColumn<int>(1), /* subject_id */ CC_CSV.GetColumn<int>(2), /* status_id */ CC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CC_CSV.GetRowCount()));
const rapidcsv::Document CCT1_CSV("../datasets/job/29a/cct1.csv", NO_HEADERS, SEPARATOR);
auto cct1 = tuple(/* id */ CCT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT1_CSV.GetRowCount()));
const rapidcsv::Document CCT2_CSV("../datasets/job/29a/cct2.csv", NO_HEADERS, SEPARATOR);
auto cct2 = tuple(/* id */ CCT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT2_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/29a/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/29a/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/29a/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/29a/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/29a/it.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));
const rapidcsv::Document PI_CSV("../datasets/job/person_info.csv", NO_HEADERS, SEPARATOR);
auto pi = tuple(/* person_id */ PI_CSV.GetColumn<int>(1), /* info_type_id */ PI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(PI_CSV.GetRowCount()));
const rapidcsv::Document AN_CSV("../datasets/job/aka_name.csv", NO_HEADERS, SEPARATOR);
auto an = tuple(/* person_id */ AN_CSV.GetColumn<int>(1), /* size */ static_cast<int>(AN_CSV.GetRowCount()));
const rapidcsv::Document IT3_CSV("../datasets/job/29a/it3.csv", NO_HEADERS, SEPARATOR);
auto it3 = tuple(/* id */ IT3_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT3_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/29a/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* name */ N_CSV.GetColumn<string>(1), /* size */ static_cast<int>(N_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/29a/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* person_role_id */ CI_CSV.GetColumn<int>(3), /* role_id */ CI_CSV.GetColumn<int>(6), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document CHN_CSV("../datasets/job/29a/chn.csv", NO_HEADERS, SEPARATOR);
auto chn = tuple(/* id */ CHN_CSV.GetColumn<int>(0), /* name */ CHN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CHN_CSV.GetRowCount()));
const rapidcsv::Document RT_CSV("../datasets/job/29a/rt.csv", NO_HEADERS, SEPARATOR);
auto rt = tuple(/* id */ RT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(RT_CSV.GetRowCount()));

int main() {
vector<int> cct1_offsets(get<1>(cct1));
iota(cct1_offsets.begin(), cct1_offsets.end(), 0);
sort(cct1_offsets.begin(), cct1_offsets.end(), [&](const int i, const int j) { return get<0>(cct1)[i] < get<0>(cct1)[j]; });
vector<int> cct2_offsets(get<1>(cct2));
iota(cct2_offsets.begin(), cct2_offsets.end(), 0);
sort(cct2_offsets.begin(), cct2_offsets.end(), [&](const int i, const int j) { return get<0>(cct2)[i] < get<0>(cct2)[j]; });
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
vector<int> it3_offsets(get<1>(it3));
iota(it3_offsets.begin(), it3_offsets.end(), 0);
sort(it3_offsets.begin(), it3_offsets.end(), [&](const int i, const int j) { return get<0>(it3)[i] < get<0>(it3)[j]; });
vector<int> an_offsets(get<1>(an));
iota(an_offsets.begin(), an_offsets.end(), 0);
sort(an_offsets.begin(), an_offsets.end(), [&](const int i, const int j) { return get<0>(an)[i] < get<0>(an)[j]; });
vector<int> chn_offsets(get<2>(chn));
iota(chn_offsets.begin(), chn_offsets.end(), 0);
sort(chn_offsets.begin(), chn_offsets.end(), [&](const int i, const int j) { return get<0>(chn)[i] < get<0>(chn)[j]; });
vector<int> rt_offsets(get<1>(rt));
iota(rt_offsets.begin(), rt_offsets.end(), 0);
sort(rt_offsets.begin(), rt_offsets.end(), [&](const int i, const int j) { return get<0>(rt)[i] < get<0>(rt)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> cct1_trie0(get<1>(cct1));
for (int i = 0; i < get<1>(cct1); ++i)
cct1_trie0[get<0>(cct1)[cct1_offsets[i]]] += 1;
SortedDict<int, int> cct2_trie0(get<1>(cct2));
for (int i = 0; i < get<1>(cct2); ++i)
cct2_trie0[get<0>(cct2)[cct2_offsets[i]]] += 1;
vector<tuple<int, int, int>> interm0;
for (auto cc_off = 0; cc_off < get<3>(cc); ++cc_off) {
const auto x0 = get<1>(cc)[cc_off];
const auto cct1_it = cct1_trie0.find(x0);
if (cct1_it != cct1_trie0.end()) {
const auto &cct1_trie1 = cct1_it->second;
const auto x1 = get<2>(cc)[cc_off];
const auto cct2_it = cct2_trie0.find(x1);
if (cct2_it != cct2_trie0.end()) {
const auto &cct2_trie1 = cct2_it->second;
interm0.emplace_back(get<0>(cc)[cc_off], get<1>(cc)[cc_off], get<2>(cc)[cc_off]);}
}
}
sort(interm0.begin(), interm0.end(), [](const tuple<int, int, int> &a, const tuple<int, int, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
vector<tuple<int, int, string>> interm1;
for (auto t_off = 0; t_off < get<2>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
interm1.emplace_back(get<0>(t)[t_off], get<0>(t)[t_off], get<1>(t)[t_off]);}
}
sort(interm1.begin(), interm1.end(), [](const tuple<int, int, string> &a, const tuple<int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm1_trie0 = SortedDict<int, Range>(interm1.size());
for (int i = 0; i < interm1.size(); ++i)
interm1_trie0[get<0>(interm1[i])][i] += 1;
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
vector<tuple<int, int, int, string>> interm2;
for (auto mc_off = 0; mc_off < get<2>(mc); ++mc_off) {
const auto x0 = get<0>(mc)[mc_off];
const auto interm1_it = interm1_trie0.find(x0);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto x1 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x1);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
for (auto interm1_off = interm1_trie1.left(); interm1_off < interm1_trie1.right(); ++interm1_off) {
const auto &interm1_tuple = interm1[interm1_off];
interm2.emplace_back(get<0>(mc)[mc_off], get<1>(mc)[mc_off], get<0>(mc)[mc_off], get<2>(interm1_tuple));}
}
}
}
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
sort(interm2.begin(), interm2.end(), [](const tuple<int, int, int, string> &a, const tuple<int, int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm2_trie0 = SortedDict<int, Range>(interm2.size());
for (int i = 0; i < interm2.size(); ++i)
interm2_trie0[get<0>(interm2[i])][i] += 1;
vector<tuple<int, int, int, string>> interm3;
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
interm3.emplace_back(get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<0>(mk)[mk_off], get<3>(interm2_tuple));}
}
}
}
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
sort(interm3.begin(), interm3.end(), [](const tuple<int, int, int, string> &a, const tuple<int, int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm3_trie0 = SortedDict<int, Range>(interm3.size());
for (int i = 0; i < interm3.size(); ++i)
interm3_trie0[get<0>(interm3[i])][i] += 1;
vector<tuple<int, int, int, string>> interm4;
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<1>(mi)[mi_off];
const auto it_it = it_trie0.find(x0);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
const auto x1 = get<0>(mi)[mi_off];
const auto interm3_it = interm3_trie0.find(x1);
if (interm3_it != interm3_trie0.end()) {
auto &interm3_trie1 = interm3_it->second;
for (auto interm3_off = interm3_trie1.left(); interm3_off < interm3_trie1.right(); ++interm3_off) {
const auto &interm3_tuple = interm3[interm3_off];
interm4.emplace_back(get<0>(mi)[mi_off], get<1>(mi)[mi_off], get<0>(mi)[mi_off], get<3>(interm3_tuple));}
}
}
}
SortedDict<int, int> it3_trie0(get<1>(it3));
for (int i = 0; i < get<1>(it3); ++i)
it3_trie0[get<0>(it3)[it3_offsets[i]]] += 1;
SortedDict<int, int> an_trie0(get<1>(an));
for (int i = 0; i < get<1>(an); ++i)
an_trie0[get<0>(an)[an_offsets[i]]] += 1;
vector<tuple<int, int>> interm5;
for (auto pi_off = 0; pi_off < get<2>(pi); ++pi_off) {
const auto x0 = get<1>(pi)[pi_off];
const auto it3_it = it3_trie0.find(x0);
if (it3_it != it3_trie0.end()) {
const auto &it3_trie1 = it3_it->second;
const auto x1 = get<0>(pi)[pi_off];
const auto an_it = an_trie0.find(x1);
if (an_it != an_trie0.end()) {
const auto &an_trie1 = an_it->second;
interm5.emplace_back(get<0>(pi)[pi_off], get<1>(pi)[pi_off]);}
}
}
sort(interm5.begin(), interm5.end(), [](const tuple<int, int> &a, const tuple<int, int> &b) { return get<0>(a) < get<0>(b); });
auto interm5_trie0 = SortedDict<int, Range>(interm5.size());
for (int i = 0; i < interm5.size(); ++i)
interm5_trie0[get<0>(interm5[i])][i] += 1;
vector<tuple<int, int, string>> interm6;
for (auto n_off = 0; n_off < get<2>(n); ++n_off) {
const auto x0 = get<0>(n)[n_off];
const auto interm5_it = interm5_trie0.find(x0);
if (interm5_it != interm5_trie0.end()) {
auto &interm5_trie1 = interm5_it->second;
interm6.emplace_back(get<0>(n)[n_off], get<0>(n)[n_off], get<1>(n)[n_off]);}
}
SortedDict<int, Range> chn_trie0(get<2>(chn));
for (int i = 0; i < get<2>(chn); ++i)
chn_trie0[get<0>(chn)[chn_offsets[i]]][i] += 1;
SortedDict<int, int> rt_trie0(get<1>(rt));
for (int i = 0; i < get<1>(rt); ++i)
rt_trie0[get<0>(rt)[rt_offsets[i]]] += 1;
sort(interm4.begin(), interm4.end(), [](const tuple<int, int, int, string> &a, const tuple<int, int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm4_trie0 = SortedDict<int, Range>(interm4.size());
for (int i = 0; i < interm4.size(); ++i)
interm4_trie0[get<0>(interm4[i])][i] += 1;
sort(interm6.begin(), interm6.end(), [](const tuple<int, int, string> &a, const tuple<int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm6_trie0 = SortedDict<int, Range>(interm6.size());
for (int i = 0; i < interm6.size(); ++i)
interm6_trie0[get<0>(interm6[i])][i] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<4>(ci); ++ci_off) {
const auto x0 = get<2>(ci)[ci_off];
const auto chn_it = chn_trie0.find(x0);
if (chn_it != chn_trie0.end()) {
const auto &chn_trie1 = chn_it->second;
const auto x1 = get<3>(ci)[ci_off];
const auto rt_it = rt_trie0.find(x1);
if (rt_it != rt_trie0.end()) {
const auto &rt_trie1 = rt_it->second;
const auto x2 = get<1>(ci)[ci_off];
const auto interm4_it = interm4_trie0.find(x2);
if (interm4_it != interm4_trie0.end()) {
auto &interm4_trie1 = interm4_it->second;
const auto x3 = get<0>(ci)[ci_off];
const auto interm6_it = interm6_trie0.find(x3);
if (interm6_it != interm6_trie0.end()) {
auto &interm6_trie1 = interm6_it->second;
auto mn_chn = tuple<string>(STRING_MAX);
for (auto chn_i = chn_trie1.left(); chn_i < chn_trie1.right(); ++chn_i) {
const auto chn_off = chn_offsets[chn_i];
min_inplace(mn_chn, {get<1>(chn)[chn_off]});
}
auto mn_interm6 = tuple<string>(STRING_MAX);
for (auto interm6_off = interm6_trie1.left(); interm6_off < interm6_trie1.right(); ++interm6_off) {
const auto &interm6_tuple = interm6[interm6_off];
min_inplace(mn_interm6, {get<2>(interm6_tuple)});
}
auto mn_interm4 = tuple<string>(STRING_MAX);
for (auto interm4_off = interm4_trie1.left(); interm4_off < interm4_trie1.right(); ++interm4_off) {
const auto &interm4_tuple = interm4[interm4_off];
min_inplace(mn_interm4, {get<3>(interm4_tuple)});
}
min_inplace(res, {get<0>(mn_chn), get<0>(mn_interm6), get<0>(mn_interm4)});
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}