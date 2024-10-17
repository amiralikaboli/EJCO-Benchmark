#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document CC_CSV("../datasets/job/complete_cast.csv", NO_HEADERS, SEPARATOR);
auto cc = tuple(/* movie_id */ CC_CSV.GetColumn<int>(1), /* subject_id */ CC_CSV.GetColumn<int>(2), /* status_id */ CC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CC_CSV.GetRowCount()));
const rapidcsv::Document CCT1_CSV("../datasets/job/20c/cct1.csv", NO_HEADERS, SEPARATOR);
auto cct1 = tuple(/* id */ CCT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT1_CSV.GetRowCount()));
const rapidcsv::Document CCT2_CSV("../datasets/job/20c/cct2.csv", NO_HEADERS, SEPARATOR);
auto cct2 = tuple(/* id */ CCT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT2_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/20c/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* kind_id */ T_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document KT_CSV("../datasets/job/20c/kt.csv", NO_HEADERS, SEPARATOR);
auto kt = tuple(/* id */ KT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/20c/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/cast_info.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* person_role_id */ CI_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/name.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* name */ N_CSV.GetColumn<string>(1), /* size */ static_cast<int>(N_CSV.GetRowCount()));
const rapidcsv::Document CHN_CSV("../datasets/job/20c/chn.csv", NO_HEADERS, SEPARATOR);
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
vector<int> chn_offsets(get<1>(chn));
iota(chn_offsets.begin(), chn_offsets.end(), 0);
sort(chn_offsets.begin(), chn_offsets.end(), [&](const int i, const int j) { return get<0>(chn)[i] < get<0>(chn)[j]; });
vector<int> n_offsets(get<2>(n));
iota(n_offsets.begin(), n_offsets.end(), 0);
sort(n_offsets.begin(), n_offsets.end(), [&](const int i, const int j) { return get<0>(n)[i] < get<0>(n)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> cct1_trie0(get<1>(cct1));
for (int i = 0; i < get<1>(cct1); ++i)
cct1_trie0[get<0>(cct1)[cct1_offsets[i]]] += 1;
SortedDict<int, int> cct2_trie0(get<1>(cct2));
for (int i = 0; i < get<1>(cct2); ++i)
cct2_trie0[get<0>(cct2)[cct2_offsets[i]]] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int>, 4>>(min(1000000, get<3>(cc)));
for (auto cc_off = 0; cc_off < get<3>(cc); ++cc_off) {
const auto x0 = get<1>(cc)[cc_off];
const auto cct1_it = cct1_trie0.find(x0);
if (cct1_it != cct1_trie0.end()) {
const auto &cct1_trie1 = cct1_it->second;
const auto x1 = get<2>(cc)[cc_off];
const auto cct2_it = cct2_trie0.find(x1);
if (cct2_it != cct2_trie0.end()) {
const auto &cct2_trie1 = cct2_it->second;
interm0_trie0[get<0>(cc)[cc_off]][tuple<int, int>(get<1>(cc)[cc_off], get<2>(cc)[cc_off])] += 1;
}
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
auto interm1_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string>, 4>>(min(1000000, get<2>(mk)));
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
interm1_trie0[get<0>(mk)[mk_off]][tuple<int, int, string>(get<1>(mk)[mk_off], get<2>(t)[t_off], get<1>(t)[t_off])] += 1;
}
}
}
}
}
SortedDict<int, int> chn_trie0(get<1>(chn));
for (int i = 0; i < get<1>(chn); ++i)
chn_trie0[get<0>(chn)[chn_offsets[i]]] += 1;
SortedDict<int, Range> n_trie0(get<2>(n));
for (int i = 0; i < get<2>(n); ++i)
n_trie0[get<0>(n)[n_offsets[i]]][i] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<3>(ci); ++ci_off) {
const auto x0 = get<1>(ci)[ci_off];
const auto interm1_it = interm1_trie0.find(x0);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto x1 = get<2>(ci)[ci_off];
const auto chn_it = chn_trie0.find(x1);
if (chn_it != chn_trie0.end()) {
const auto &chn_trie1 = chn_it->second;
const auto x2 = get<0>(ci)[ci_off];
const auto n_it = n_trie0.find(x2);
if (n_it != n_trie0.end()) {
const auto &n_trie1 = n_it->second;
auto mn_n = tuple<string>(STRING_MAX);
for (auto n_i = n_trie1.left(); n_i < n_trie1.right(); ++n_i) {
const auto n_off = n_offsets[n_i];
min_inplace(mn_n, {get<1>(n)[n_off]});
}
auto mn_interm1 = tuple<string>(STRING_MAX);
for (const auto &interm1_tuple: interm1_trie1) {
min_inplace(mn_interm1, {get<2>(interm1_tuple)});
}
min_inplace(res, {get<0>(mn_n), get<0>(mn_interm1)});
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}