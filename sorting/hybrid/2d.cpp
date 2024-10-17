#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/title.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/2d/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/2d/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));

int main() {
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> mc_offsets(get<2>(mc));
iota(mc_offsets.begin(), mc_offsets.end(), 0);
sort(mc_offsets.begin(), mc_offsets.end(), [&](const int i, const int j) { return get<0>(mc)[i] < get<0>(mc)[j]; });
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, Range> mc_trie0(get<2>(mc));
for (int i = 0; i < get<2>(mc); ++i)
mc_trie0[get<0>(mc)[mc_offsets[i]]][i] += 1;
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
auto res = tuple<string>(STRING_MAX);
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x0);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x1 = get<0>(mk)[mk_off];
const auto t_it = t_trie0.find(x1);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
const auto mc_it = mc_trie0.find(x1);
if (mc_it != mc_trie0.end()) {
const auto &mc_trie1 = mc_it->second;
for (auto mc_i = mc_trie1.left(); mc_i < mc_trie1.right(); ++mc_i) {
const auto mc_off = mc_offsets[mc_i];
const auto x2 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x2);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
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