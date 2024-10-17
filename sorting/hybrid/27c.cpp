#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document ML_CSV("../datasets/job/movie_link.csv", NO_HEADERS, SEPARATOR);
auto ml = tuple(/* movie_id */ ML_CSV.GetColumn<int>(1), /* link_type_id */ ML_CSV.GetColumn<int>(3), /* size */ static_cast<int>(ML_CSV.GetRowCount()));
const rapidcsv::Document LT_CSV("../datasets/job/27c/lt.csv", NO_HEADERS, SEPARATOR);
auto lt = tuple(/* id */ LT_CSV.GetColumn<int>(0), /* link */ LT_CSV.GetColumn<string>(1), /* size */ static_cast<int>(LT_CSV.GetRowCount()));
const rapidcsv::Document CC_CSV("../datasets/job/complete_cast.csv", NO_HEADERS, SEPARATOR);
auto cc = tuple(/* movie_id */ CC_CSV.GetColumn<int>(1), /* subject_id */ CC_CSV.GetColumn<int>(2), /* status_id */ CC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(CC_CSV.GetRowCount()));
const rapidcsv::Document CCT1_CSV("../datasets/job/27c/cct1.csv", NO_HEADERS, SEPARATOR);
auto cct1 = tuple(/* id */ CCT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT1_CSV.GetRowCount()));
const rapidcsv::Document CCT2_CSV("../datasets/job/27c/cct2.csv", NO_HEADERS, SEPARATOR);
auto cct2 = tuple(/* id */ CCT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CCT2_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/27c/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/27c/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/27c/ct.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/27c/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* name */ CN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/27c/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/27c/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* size */ static_cast<int>(MI_CSV.GetRowCount()));

int main() {
vector<int> lt_offsets(get<2>(lt));
iota(lt_offsets.begin(), lt_offsets.end(), 0);
sort(lt_offsets.begin(), lt_offsets.end(), [&](const int i, const int j) { return get<0>(lt)[i] < get<0>(lt)[j]; });
vector<int> cct1_offsets(get<1>(cct1));
iota(cct1_offsets.begin(), cct1_offsets.end(), 0);
sort(cct1_offsets.begin(), cct1_offsets.end(), [&](const int i, const int j) { return get<0>(cct1)[i] < get<0>(cct1)[j]; });
vector<int> cct2_offsets(get<1>(cct2));
iota(cct2_offsets.begin(), cct2_offsets.end(), 0);
sort(cct2_offsets.begin(), cct2_offsets.end(), [&](const int i, const int j) { return get<0>(cct2)[i] < get<0>(cct2)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<2>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> lt_trie0(get<2>(lt));
for (int i = 0; i < get<2>(lt); ++i)
lt_trie0[get<0>(lt)[lt_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string>, 4>>(min(1000000, get<2>(ml)));
for (auto ml_off = 0; ml_off < get<2>(ml); ++ml_off) {
const auto x0 = get<1>(ml)[ml_off];
const auto lt_it = lt_trie0.find(x0);
if (lt_it != lt_trie0.end()) {
const auto &lt_trie1 = lt_it->second;
for (auto lt_i = lt_trie1.left(); lt_i < lt_trie1.right(); ++lt_i) {
const auto lt_off = lt_offsets[lt_i];
interm0_trie0[get<0>(ml)[ml_off]][tuple<int, string>(get<1>(ml)[ml_off], get<1>(lt)[lt_off])] += 1;
}
}
}
SortedDict<int, int> cct1_trie0(get<1>(cct1));
for (int i = 0; i < get<1>(cct1); ++i)
cct1_trie0[get<0>(cct1)[cct1_offsets[i]]] += 1;
SortedDict<int, int> cct2_trie0(get<1>(cct2));
for (int i = 0; i < get<1>(cct2); ++i)
cct2_trie0[get<0>(cct2)[cct2_offsets[i]]] += 1;
auto interm1_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string>, 4>>(min(1000000, get<3>(cc)));
for (auto cc_off = 0; cc_off < get<3>(cc); ++cc_off) {
const auto x0 = get<1>(cc)[cc_off];
const auto cct1_it = cct1_trie0.find(x0);
if (cct1_it != cct1_trie0.end()) {
const auto &cct1_trie1 = cct1_it->second;
const auto x1 = get<2>(cc)[cc_off];
const auto cct2_it = cct2_trie0.find(x1);
if (cct2_it != cct2_trie0.end()) {
const auto &cct2_trie1 = cct2_it->second;
const auto x2 = get<0>(cc)[cc_off];
const auto interm0_it = interm0_trie0.find(x2);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (const auto &interm0_tuple: interm0_trie1) {
interm1_trie0[get<0>(cc)[cc_off]][tuple<int, int, string>(get<1>(cc)[cc_off], get<2>(cc)[cc_off], get<1>(interm0_tuple))] += 1;
}
}
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
interm2_trie0[get<0>(t)[t_off]][tuple<string, string>(get<1>(t)[t_off], get<2>(interm1_tuple))] += 1;
}
}
}
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
SortedDict<int, Range> cn_trie0(get<2>(cn));
for (int i = 0; i < get<2>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]][i] += 1;
auto interm3_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string, string, string>, 4>>(min(1000000, get<3>(mc)));
for (auto mc_off = 0; mc_off < get<3>(mc); ++mc_off) {
const auto x0 = get<2>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x0);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
const auto x1 = get<0>(mc)[mc_off];
const auto interm2_it = interm2_trie0.find(x1);
if (interm2_it != interm2_trie0.end()) {
auto &interm2_trie1 = interm2_it->second;
const auto x2 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x2);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
for (auto cn_i = cn_trie1.left(); cn_i < cn_trie1.right(); ++cn_i) {
const auto cn_off = cn_offsets[cn_i];
for (const auto &interm2_tuple: interm2_trie1) {
interm3_trie0[get<0>(mc)[mc_off]][tuple<int, int, string, string, string>(get<2>(mc)[mc_off], get<1>(mc)[mc_off], get<1>(cn)[cn_off], get<0>(interm2_tuple), get<1>(interm2_tuple))] += 1;
}
}
}
}
}
}
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
auto interm4_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string, string, string>, 4>>(min(1000000, get<2>(mk)));
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x0);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x1 = get<0>(mk)[mk_off];
const auto interm3_it = interm3_trie0.find(x1);
if (interm3_it != interm3_trie0.end()) {
auto &interm3_trie1 = interm3_it->second;
for (const auto &interm3_tuple: interm3_trie1) {
interm4_trie0[get<0>(mk)[mk_off]][tuple<int, string, string, string>(get<1>(mk)[mk_off], get<2>(interm3_tuple), get<3>(interm3_tuple), get<4>(interm3_tuple))] += 1;
}
}
}
}
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<1>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto interm4_it = interm4_trie0.find(x0);
if (interm4_it != interm4_trie0.end()) {
auto &interm4_trie1 = interm4_it->second;
auto mn_interm4 = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (const auto &interm4_tuple: interm4_trie1) {
min_inplace(mn_interm4, {get<1>(interm4_tuple), get<2>(interm4_tuple), get<3>(interm4_tuple)});
}
min_inplace(res, {get<0>(mn_interm4), get<1>(mn_interm4), get<2>(mn_interm4)});
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}