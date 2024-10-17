#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/3b/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MI_CSV("../datasets/job/3b/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/3b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));

int main() {
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string>, 4>>(min(1000000, get<2>(mk)));
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto t_it = t_trie0.find(x0);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
interm0_trie0[get<0>(mk)[mk_off]][tuple<int, string>(get<1>(mk)[mk_off], get<1>(t)[t_off])] += 1;
}
}
}
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
auto res = tuple<string>(STRING_MAX);
for (auto mi_off = 0; mi_off < get<1>(mi); ++mi_off) {
const auto x0 = get<0>(mi)[mi_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (const auto &interm0_tuple: interm0_trie1) {
const auto x1 = get<0>(interm0_tuple);
const auto k_it = k_trie0.find(x1);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
auto mn_interm0 = tuple<string>(get<1>(interm0_tuple));
min_inplace(res, {get<0>(mn_interm0)});
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}