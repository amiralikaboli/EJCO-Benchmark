#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document T_CSV("../datasets/job/title.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* kind_id */ T_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MIIDX_CSV("../datasets/job/movie_info_idx.csv", NO_HEADERS, SEPARATOR);
auto miidx = tuple(/* movie_id */ MIIDX_CSV.GetColumn<int>(1), /* info_type_id */ MIIDX_CSV.GetColumn<int>(2), /* info */ MIIDX_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MIIDX_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/movie_info.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document KT_CSV("../datasets/job/13d/kt.csv", NO_HEADERS, SEPARATOR);
auto kt = tuple(/* id */ KT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT_CSV.GetRowCount()));
const rapidcsv::Document IT2_CSV("../datasets/job/13d/it2.csv", NO_HEADERS, SEPARATOR);
auto it2 = tuple(/* id */ IT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT2_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/13d/it.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/13d/ct.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/13d/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* name */ CN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CN_CSV.GetRowCount()));

int main() {
vector<int> miidx_offsets(get<3>(miidx));
iota(miidx_offsets.begin(), miidx_offsets.end(), 0);
sort(miidx_offsets.begin(), miidx_offsets.end(), [&](const int i, const int j) { return get<0>(miidx)[i] < get<0>(miidx)[j]; });
vector<int> mc_offsets(get<3>(mc));
iota(mc_offsets.begin(), mc_offsets.end(), 0);
sort(mc_offsets.begin(), mc_offsets.end(), [&](const int i, const int j) { return get<0>(mc)[i] < get<0>(mc)[j]; });
vector<int> kt_offsets(get<1>(kt));
iota(kt_offsets.begin(), kt_offsets.end(), 0);
sort(kt_offsets.begin(), kt_offsets.end(), [&](const int i, const int j) { return get<0>(kt)[i] < get<0>(kt)[j]; });
vector<int> it2_offsets(get<1>(it2));
iota(it2_offsets.begin(), it2_offsets.end(), 0);
sort(it2_offsets.begin(), it2_offsets.end(), [&](const int i, const int j) { return get<0>(it2)[i] < get<0>(it2)[j]; });
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<2>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> miidx_trie0(get<3>(miidx));
for (int i = 0; i < get<3>(miidx); ++i)
miidx_trie0[get<0>(miidx)[miidx_offsets[i]]][i] += 1;
vector<tuple<int, string, int, string, int>> interm0;
for (auto t_off = 0; t_off < get<3>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto miidx_it = miidx_trie0.find(x0);
if (miidx_it != miidx_trie0.end()) {
const auto &miidx_trie1 = miidx_it->second;
for (auto miidx_i = miidx_trie1.left(); miidx_i < miidx_trie1.right(); ++miidx_i) {
const auto miidx_off = miidx_offsets[miidx_i];
interm0.emplace_back(get<0>(t)[t_off], get<1>(t)[t_off], get<2>(t)[t_off], get<2>(miidx)[miidx_off], get<1>(miidx)[miidx_off]);}
}
}
sort(interm0.begin(), interm0.end(), [](const tuple<int, string, int, string, int> &a, const tuple<int, string, int, string, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, Range> mc_trie0(get<3>(mc));
for (int i = 0; i < get<3>(mc); ++i)
mc_trie0[get<0>(mc)[mc_offsets[i]]][i] += 1;
SortedDict<int, int> kt_trie0(get<1>(kt));
for (int i = 0; i < get<1>(kt); ++i)
kt_trie0[get<0>(kt)[kt_offsets[i]]] += 1;
SortedDict<int, int> it2_trie0(get<1>(it2));
for (int i = 0; i < get<1>(it2); ++i)
it2_trie0[get<0>(it2)[it2_offsets[i]]] += 1;
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
SortedDict<int, Range> cn_trie0(get<2>(cn));
for (int i = 0; i < get<2>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]][i] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto mc_it = mc_trie0.find(x0);
if (mc_it != mc_trie0.end()) {
const auto &mc_trie1 = mc_it->second;
for (auto interm0_off = interm0_trie1.left(); interm0_off < interm0_trie1.right(); ++interm0_off) {
const auto &interm0_tuple = interm0[interm0_off];
const auto x1 = get<2>(interm0_tuple);
const auto kt_it = kt_trie0.find(x1);
if (kt_it != kt_trie0.end()) {
const auto &kt_trie1 = kt_it->second;
const auto x2 = get<1>(mi)[mi_off];
const auto it2_it = it2_trie0.find(x2);
if (it2_it != it2_trie0.end()) {
const auto &it2_trie1 = it2_it->second;
const auto &interm0_tuple = interm0[interm0_off];
const auto x3 = get<4>(interm0_tuple);
const auto it_it = it_trie0.find(x3);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
for (auto mc_i = mc_trie1.left(); mc_i < mc_trie1.right(); ++mc_i) {
const auto mc_off = mc_offsets[mc_i];
const auto x4 = get<2>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x4);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
const auto x5 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x5);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
auto mn_interm0 = tuple<string, string>(get<1>(interm0_tuple), get<3>(interm0_tuple));
auto mn_cn = tuple<string>(STRING_MAX);
for (auto cn_i = cn_trie1.left(); cn_i < cn_trie1.right(); ++cn_i) {
const auto cn_off = cn_offsets[cn_i];
min_inplace(mn_cn, {get<1>(cn)[cn_off]});
}
min_inplace(res, {get<0>(mn_interm0), get<1>(mn_interm0), get<0>(mn_cn)});
}
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