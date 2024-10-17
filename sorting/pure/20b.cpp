#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document CC_CSV("../datasets/job/complete_cast.csv", NO_HEADERS, SEPARATOR);
auto cc = tuple(/* movie_id */ CC_CSV.GetColumn<int>(1), /* subject_id */ CC_CSV.GetColumn<int>(2), /* status_id */ CC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CC_CSV.GetRowCount()));
const rapidcsv::Document CCT1_CSV("../datasets/job/20b/cct1.csv", NO_HEADERS, SEPARATOR);
auto cct1 = tuple(/* id */ CCT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT1_CSV.GetRowCount()));
const rapidcsv::Document CCT2_CSV("../datasets/job/20b/cct2.csv", NO_HEADERS, SEPARATOR);
auto cct2 = tuple(/* id */ CCT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT2_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/20b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* kind_id */ T_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document KT_CSV("../datasets/job/20b/kt.csv", NO_HEADERS, SEPARATOR);
auto kt = tuple(/* id */ KT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/20b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/cast_info.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* person_role_id */ CI_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/20b/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* size */ static_cast<int>(N_CSV.GetRowCount()));
const rapidcsv::Document CHN_CSV("../datasets/job/20b/chn.csv", NO_HEADERS, SEPARATOR);
auto chn = tuple(/* id */ CHN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CHN_CSV.GetRowCount()));

int main() {
vector<int> cct1_offsets(get<1>(cct1));
iota(cct1_offsets.begin(), cct1_offsets.end(), 0);
sort(cct1_offsets.begin(), cct1_offsets.end(), [&](const int i, const int j) { return get<0>(cct1)[i] < get<0>(cct1)[j]; });
vector<int> cct2_offsets(get<1>(cct2));
iota(cct2_offsets.begin(), cct2_offsets.end(), 0);
sort(cct2_offsets.begin(), cct2_offsets.end(), [&](const int i, const int j) { return get<0>(cct2)[i] < get<0>(cct2)[j]; });
vector<int> t_offsets(get<3>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> kt_offsets(get<1>(kt));
iota(kt_offsets.begin(), kt_offsets.end(), 0);
sort(kt_offsets.begin(), kt_offsets.end(), [&](const int i, const int j) { return get<0>(kt)[i] < get<0>(kt)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> n_offsets(get<1>(n));
iota(n_offsets.begin(), n_offsets.end(), 0);
sort(n_offsets.begin(), n_offsets.end(), [&](const int i, const int j) { return get<0>(n)[i] < get<0>(n)[j]; });
vector<int> chn_offsets(get<1>(chn));
iota(chn_offsets.begin(), chn_offsets.end(), 0);
sort(chn_offsets.begin(), chn_offsets.end(), [&](const int i, const int j) { return get<0>(chn)[i] < get<0>(chn)[j]; });
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
SortedDict<int, Range> t_trie0(get<3>(t));
for (int i = 0; i < get<3>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, int> kt_trie0(get<1>(kt));
for (int i = 0; i < get<1>(kt); ++i)
kt_trie0[get<0>(kt)[kt_offsets[i]]] += 1;
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
vector<tuple<int, int, int, string>> interm1;
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto t_it = t_trie0.find(x0);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
const auto x1 = get<2>(t)[t_off];
const auto kt_it = kt_trie0.find(x1);
if (kt_it != kt_trie0.end()) {
const auto &kt_trie1 = kt_it->second;
const auto x2 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x2);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
interm1.emplace_back(get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<2>(t)[t_off], get<1>(t)[t_off]);}
}
}
}
}
SortedDict<int, int> n_trie0(get<1>(n));
for (int i = 0; i < get<1>(n); ++i)
n_trie0[get<0>(n)[n_offsets[i]]] += 1;
SortedDict<int, int> chn_trie0(get<1>(chn));
for (int i = 0; i < get<1>(chn); ++i)
chn_trie0[get<0>(chn)[chn_offsets[i]]] += 1;
sort(interm1.begin(), interm1.end(), [](const tuple<int, int, int, string> &a, const tuple<int, int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm1_trie0 = SortedDict<int, Range>(interm1.size());
for (int i = 0; i < interm1.size(); ++i)
interm1_trie0[get<0>(interm1[i])][i] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int, int> &a, const tuple<int, int, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
auto res = tuple<string>(STRING_MAX);
for (auto ci_off = 0; ci_off < get<3>(ci); ++ci_off) {
const auto x0 = get<0>(ci)[ci_off];
const auto n_it = n_trie0.find(x0);
if (n_it != n_trie0.end()) {
const auto &n_trie1 = n_it->second;
const auto x1 = get<2>(ci)[ci_off];
const auto chn_it = chn_trie0.find(x1);
if (chn_it != chn_trie0.end()) {
const auto &chn_trie1 = chn_it->second;
const auto x2 = get<1>(ci)[ci_off];
const auto interm1_it = interm1_trie0.find(x2);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto interm0_it = interm0_trie0.find(x2);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
auto mn_interm1 = tuple<string>(STRING_MAX);
for (auto interm1_off = interm1_trie1.left(); interm1_off < interm1_trie1.right(); ++interm1_off) {
const auto &interm1_tuple = interm1[interm1_off];
min_inplace(mn_interm1, {get<3>(interm1_tuple)});
}
min_inplace(res, {get<0>(mn_interm1)});
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}