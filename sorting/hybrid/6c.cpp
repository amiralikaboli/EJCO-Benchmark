#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/6c/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/6c/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* keyword */ K_CSV.GetColumn<string>(1), /* size */ static_cast<int>(K_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/cast_info.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/6c/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* name */ N_CSV.GetColumn<string>(1), /* size */ static_cast<int>(N_CSV.GetRowCount()));

int main() {
vector<int> k_offsets(get<2>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> n_offsets(get<2>(n));
iota(n_offsets.begin(), n_offsets.end(), 0);
sort(n_offsets.begin(), n_offsets.end(), [&](const int i, const int j) { return get<0>(n)[i] < get<0>(n)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> k_trie0(get<2>(k));
for (int i = 0; i < get<2>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]][i] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, string, string>, 4>>(min(1000000, get<2>(mk)));
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x0);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
const auto x1 = get<0>(mk)[mk_off];
const auto t_it = t_trie0.find(x1);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
for (auto k_i = k_trie1.left(); k_i < k_trie1.right(); ++k_i) {
const auto k_off = k_offsets[k_i];
interm0_trie0[get<0>(mk)[mk_off]][tuple<int, string, string>(get<1>(mk)[mk_off], get<1>(t)[t_off], get<1>(k)[k_off])] += 1;
}
}
}
}
}
SortedDict<int, Range> n_trie0(get<2>(n));
for (int i = 0; i < get<2>(n); ++i)
n_trie0[get<0>(n)[n_offsets[i]]][i] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<2>(ci); ++ci_off) {
const auto x0 = get<0>(ci)[ci_off];
const auto n_it = n_trie0.find(x0);
if (n_it != n_trie0.end()) {
const auto &n_trie1 = n_it->second;
const auto x1 = get<1>(ci)[ci_off];
const auto interm0_it = interm0_trie0.find(x1);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
auto mn_n = tuple<string>(STRING_MAX);
for (auto n_i = n_trie1.left(); n_i < n_trie1.right(); ++n_i) {
const auto n_off = n_offsets[n_i];
min_inplace(mn_n, {get<1>(n)[n_off]});
}
auto mn_interm0 = tuple<string, string>(STRING_MAX, STRING_MAX);
for (const auto &interm0_tuple: interm0_trie1) {
min_inplace(mn_interm0, {get<1>(interm0_tuple), get<2>(interm0_tuple)});
}
min_inplace(res, {get<0>(mn_n), get<0>(mn_interm0), get<1>(mn_interm0)});
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}