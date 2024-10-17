#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/15c/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/15c/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* info */ MI_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/keyword.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/15c/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/company_type.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/15c/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document AT_CSV("../datasets/job/aka_title.csv", NO_HEADERS, SEPARATOR);
auto at = tuple(/* movie_id */ AT_CSV.GetColumn<int>(1), /* size */ static_cast<int>(AT_CSV.GetRowCount()));

int main() {
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> mc_offsets(get<3>(mc));
iota(mc_offsets.begin(), mc_offsets.end(), 0);
sort(mc_offsets.begin(), mc_offsets.end(), [&](const int i, const int j) { return get<0>(mc)[i] < get<0>(mc)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> it1_offsets(get<1>(it1));
iota(it1_offsets.begin(), it1_offsets.end(), 0);
sort(it1_offsets.begin(), it1_offsets.end(), [&](const int i, const int j) { return get<0>(it1)[i] < get<0>(it1)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> at_offsets(get<1>(at));
iota(at_offsets.begin(), at_offsets.end(), 0);
sort(at_offsets.begin(), at_offsets.end(), [&](const int i, const int j) { return get<0>(at)[i] < get<0>(at)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
vector<tuple<int, int, string>> interm0;
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto t_it = t_trie0.find(x0);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
interm0.emplace_back(get<0>(mk)[mk_off], get<1>(mk)[mk_off], get<1>(t)[t_off]);}
}
}
SortedDict<int, Range> mc_trie0(get<3>(mc));
for (int i = 0; i < get<3>(mc); ++i)
mc_trie0[get<0>(mc)[mc_offsets[i]]][i] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int, string> &a, const tuple<int, int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
SortedDict<int, int> at_trie0(get<1>(at));
for (int i = 0; i < get<1>(at); ++i)
at_trie0[get<0>(at)[at_offsets[i]]] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto mi_off = 0; mi_off < get<3>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto mc_it = mc_trie0.find(x0);
if (mc_it != mc_trie0.end()) {
const auto &mc_trie1 = mc_it->second;
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (auto interm0_off = interm0_trie1.left(); interm0_off < interm0_trie1.right(); ++interm0_off) {
const auto &interm0_tuple = interm0[interm0_off];
const auto x1 = get<1>(interm0_tuple);
const auto k_it = k_trie0.find(x1);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x2 = get<1>(mi)[mi_off];
const auto it1_it = it1_trie0.find(x2);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
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
const auto at_it = at_trie0.find(x0);
if (at_it != at_trie0.end()) {
const auto &at_trie1 = at_it->second;
auto mn_mi = tuple<string>(get<2>(mi)[mi_off]);
auto mn_interm0 = tuple<string>(get<2>(interm0_tuple));
min_inplace(res, {get<0>(mn_mi), get<0>(mn_interm0)});
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
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}