#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document T_CSV("../datasets/job/21b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document ML_CSV("../datasets/job/movie_link.csv", NO_HEADERS, SEPARATOR);
auto ml = tuple(/* movie_id */ ML_CSV.GetColumn<int>(1), /* link_type_id */ ML_CSV.GetColumn<int>(3), /* size */ static_cast<int>(ML_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/21b/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/21b/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document LT_CSV("../datasets/job/21b/lt.csv", NO_HEADERS, SEPARATOR);
auto lt = tuple(/* id */ LT_CSV.GetColumn<int>(0), /* link */ LT_CSV.GetColumn<string>(1), /* size */ static_cast<int>(LT_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/21b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/21b/ct.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/21b/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* name */ CN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CN_CSV.GetRowCount()));

int main() {
vector<int> ml_offsets(get<2>(ml));
iota(ml_offsets.begin(), ml_offsets.end(), 0);
sort(ml_offsets.begin(), ml_offsets.end(), [&](const int i, const int j) { return get<0>(ml)[i] < get<0>(ml)[j]; });
vector<int> mc_offsets(get<3>(mc));
iota(mc_offsets.begin(), mc_offsets.end(), 0);
sort(mc_offsets.begin(), mc_offsets.end(), [&](const int i, const int j) { return get<0>(mc)[i] < get<0>(mc)[j]; });
vector<int> lt_offsets(get<2>(lt));
iota(lt_offsets.begin(), lt_offsets.end(), 0);
sort(lt_offsets.begin(), lt_offsets.end(), [&](const int i, const int j) { return get<0>(lt)[i] < get<0>(lt)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<2>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> ml_trie0(get<2>(ml));
for (int i = 0; i < get<2>(ml); ++i)
ml_trie0[get<0>(ml)[ml_offsets[i]]][i] += 1;
vector<tuple<int, string, int>> interm0;
for (auto t_off = 0; t_off < get<2>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto ml_it = ml_trie0.find(x0);
if (ml_it != ml_trie0.end()) {
const auto &ml_trie1 = ml_it->second;
for (auto ml_i = ml_trie1.left(); ml_i < ml_trie1.right(); ++ml_i) {
const auto ml_off = ml_offsets[ml_i];
interm0.emplace_back(get<0>(t)[t_off], get<1>(t)[t_off], get<1>(ml)[ml_off]);}
}
}
sort(interm0.begin(), interm0.end(), [](const tuple<int, string, int> &a, const tuple<int, string, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
vector<tuple<int, int, int, string, int>> interm1;
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (auto interm0_off = interm0_trie1.left(); interm0_off < interm0_trie1.right(); ++interm0_off) {
const auto &interm0_tuple = interm0[interm0_off];
interm1.emplace_back(get<0>(mk)[mk_off], get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<1>(interm0_tuple), get<2>(interm0_tuple));}
}
}
sort(interm1.begin(), interm1.end(), [](const tuple<int, int, int, string, int> &a, const tuple<int, int, int, string, int> &b) { return get<0>(a) < get<0>(b); });
auto interm1_trie0 = SortedDict<int, Range>(interm1.size());
for (int i = 0; i < interm1.size(); ++i)
interm1_trie0[get<0>(interm1[i])][i] += 1;
SortedDict<int, Range> mc_trie0(get<3>(mc));
for (int i = 0; i < get<3>(mc); ++i)
mc_trie0[get<0>(mc)[mc_offsets[i]]][i] += 1;
SortedDict<int, Range> lt_trie0(get<2>(lt));
for (int i = 0; i < get<2>(lt); ++i)
lt_trie0[get<0>(lt)[lt_offsets[i]]][i] += 1;
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
SortedDict<int, Range> cn_trie0(get<2>(cn));
for (int i = 0; i < get<2>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]][i] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<1>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto interm1_it = interm1_trie0.find(x0);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto mc_it = mc_trie0.find(x0);
if (mc_it != mc_trie0.end()) {
const auto &mc_trie1 = mc_it->second;
for (auto interm1_off = interm1_trie1.left(); interm1_off < interm1_trie1.right(); ++interm1_off) {
const auto &interm1_tuple = interm1[interm1_off];
const auto x1 = get<4>(interm1_tuple);
const auto lt_it = lt_trie0.find(x1);
if (lt_it != lt_trie0.end()) {
const auto &lt_trie1 = lt_it->second;
const auto &interm1_tuple = interm1[interm1_off];
const auto x2 = get<2>(interm1_tuple);
const auto k_it = k_trie0.find(x2);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
for (auto mc_i = mc_trie1.left(); mc_i < mc_trie1.right(); ++mc_i) {
const auto mc_off = mc_offsets[mc_i];
const auto x3 = get<2>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x3);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
const auto x4 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x4);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
auto mn_interm1 = tuple<string>(get<3>(interm1_tuple));
auto mn_lt = tuple<string>(STRING_MAX);
for (auto lt_i = lt_trie1.left(); lt_i < lt_trie1.right(); ++lt_i) {
const auto lt_off = lt_offsets[lt_i];
min_inplace(mn_lt, {get<1>(lt)[lt_off]});
}
auto mn_cn = tuple<string>(STRING_MAX);
for (auto cn_i = cn_trie1.left(); cn_i < cn_trie1.right(); ++cn_i) {
const auto cn_off = cn_offsets[cn_i];
min_inplace(mn_cn, {get<1>(cn)[cn_off]});
}
min_inplace(res, {get<0>(mn_interm1), get<0>(mn_lt), get<0>(mn_cn)});
}
}
}
}
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}