#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MI_CSV("../datasets/job/5c/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/5c/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/5c/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/info_type.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/5c/ct.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));

int main() {
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
vector<int> mc_offsets(get<2>(mc));
iota(mc_offsets.begin(), mc_offsets.end(), 0);
sort(mc_offsets.begin(), mc_offsets.end(), [&](const int i, const int j) { return get<0>(mc)[i] < get<0>(mc)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
SortedDict<int, Range> mc_trie0(get<2>(mc));
for (int i = 0; i < get<2>(mc); ++i)
mc_trie0[get<0>(mc)[mc_offsets[i]]][i] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
auto res = tuple<string>(STRING_MAX);
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<1>(mi)[mi_off];
const auto it_it = it_trie0.find(x0);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
const auto x1 = get<0>(mi)[mi_off];
const auto mc_it = mc_trie0.find(x1);
if (mc_it != mc_trie0.end()) {
const auto &mc_trie1 = mc_it->second;
const auto t_it = t_trie0.find(x1);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto mc_i = mc_trie1.left(); mc_i < mc_trie1.right(); ++mc_i) {
const auto mc_off = mc_offsets[mc_i];
const auto x2 = get<1>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x2);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
auto mn_t = tuple<string>(STRING_MAX);
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
min_inplace(mn_t, {get<1>(t)[t_off]});
}
min_inplace(res, {get<0>(mn_t)});
}
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}