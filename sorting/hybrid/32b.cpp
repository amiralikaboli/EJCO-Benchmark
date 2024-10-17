#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document T1_CSV("../datasets/job/title.csv", NO_HEADERS, SEPARATOR);
auto t1 = tuple(/* id */ T1_CSV.GetColumn<int>(0), /* title */ T1_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T1_CSV.GetRowCount()));
const rapidcsv::Document ML_CSV("../datasets/job/movie_link.csv", NO_HEADERS, SEPARATOR);
auto ml = tuple(/* movie_id */ ML_CSV.GetColumn<int>(1), /* linked_movie_id */ ML_CSV.GetColumn<int>(2), /* link_type_id */ ML_CSV.GetColumn<int>(3), /* size */ static_cast<int>(ML_CSV.GetRowCount()));
const rapidcsv::Document T2_CSV("../datasets/job/title.csv", NO_HEADERS, SEPARATOR);
auto t2 = tuple(/* id */ T2_CSV.GetColumn<int>(0), /* title */ T2_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T2_CSV.GetRowCount()));
const rapidcsv::Document MK_CSV("../datasets/job/movie_keyword.csv", NO_HEADERS, SEPARATOR);
auto mk = tuple(/* movie_id */ MK_CSV.GetColumn<int>(1), /* keyword_id */ MK_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MK_CSV.GetRowCount()));
const rapidcsv::Document LT_CSV("../datasets/job/link_type.csv", NO_HEADERS, SEPARATOR);
auto lt = tuple(/* id */ LT_CSV.GetColumn<int>(0), /* link */ LT_CSV.GetColumn<string>(1), /* size */ static_cast<int>(LT_CSV.GetRowCount()));
const rapidcsv::Document K_CSV("../datasets/job/32b/k.csv", NO_HEADERS, SEPARATOR);
auto k = tuple(/* id */ K_CSV.GetColumn<int>(0), /* size */ static_cast<int>(K_CSV.GetRowCount()));

int main() {
vector<int> ml_offsets(get<3>(ml));
iota(ml_offsets.begin(), ml_offsets.end(), 0);
sort(ml_offsets.begin(), ml_offsets.end(), [&](const int i, const int j) { return get<0>(ml)[i] < get<0>(ml)[j]; });
vector<int> t2_offsets(get<2>(t2));
iota(t2_offsets.begin(), t2_offsets.end(), 0);
sort(t2_offsets.begin(), t2_offsets.end(), [&](const int i, const int j) { return get<0>(t2)[i] < get<0>(t2)[j]; });
vector<int> lt_offsets(get<2>(lt));
iota(lt_offsets.begin(), lt_offsets.end(), 0);
sort(lt_offsets.begin(), lt_offsets.end(), [&](const int i, const int j) { return get<0>(lt)[i] < get<0>(lt)[j]; });
vector<int> k_offsets(get<1>(k));
iota(k_offsets.begin(), k_offsets.end(), 0);
sort(k_offsets.begin(), k_offsets.end(), [&](const int i, const int j) { return get<0>(k)[i] < get<0>(k)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> ml_trie0(get<3>(ml));
for (int i = 0; i < get<3>(ml); ++i)
ml_trie0[get<0>(ml)[ml_offsets[i]]][i] += 1;
SortedDict<int, Range> t2_trie0(get<2>(t2));
for (int i = 0; i < get<2>(t2); ++i)
t2_trie0[get<0>(t2)[t2_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<string, int, int, string>, 4>>(min(1000000, get<2>(t1)));
for (auto t1_off = 0; t1_off < get<2>(t1); ++t1_off) {
const auto x0 = get<0>(t1)[t1_off];
const auto ml_it = ml_trie0.find(x0);
if (ml_it != ml_trie0.end()) {
const auto &ml_trie1 = ml_it->second;
for (auto ml_i = ml_trie1.left(); ml_i < ml_trie1.right(); ++ml_i) {
const auto ml_off = ml_offsets[ml_i];
const auto x1 = get<1>(ml)[ml_off];
const auto t2_it = t2_trie0.find(x1);
if (t2_it != t2_trie0.end()) {
const auto &t2_trie1 = t2_it->second;
for (auto t2_i = t2_trie1.left(); t2_i < t2_trie1.right(); ++t2_i) {
const auto t2_off = t2_offsets[t2_i];
interm0_trie0[get<0>(t1)[t1_off]][tuple<string, int, int, string>(get<1>(t1)[t1_off], get<1>(ml)[ml_off], get<2>(ml)[ml_off], get<1>(t2)[t2_off])] += 1;
}
}
}
}
}
SortedDict<int, Range> lt_trie0(get<2>(lt));
for (int i = 0; i < get<2>(lt); ++i)
lt_trie0[get<0>(lt)[lt_offsets[i]]][i] += 1;
SortedDict<int, int> k_trie0(get<1>(k));
for (int i = 0; i < get<1>(k); ++i)
k_trie0[get<0>(k)[k_offsets[i]]] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto mk_off = 0; mk_off < get<2>(mk); ++mk_off) {
const auto x0 = get<0>(mk)[mk_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (const auto &interm0_tuple: interm0_trie1) {
const auto x1 = get<2>(interm0_tuple);
const auto lt_it = lt_trie0.find(x1);
if (lt_it != lt_trie0.end()) {
const auto &lt_trie1 = lt_it->second;
const auto x2 = get<1>(mk)[mk_off];
const auto k_it = k_trie0.find(x2);
if (k_it != k_trie0.end()) {
const auto &k_trie1 = k_it->second;
auto mn_interm0 = tuple<string, string>(get<0>(interm0_tuple), get<3>(interm0_tuple));
auto mn_lt = tuple<string>(STRING_MAX);
for (auto lt_i = lt_trie1.left(); lt_i < lt_trie1.right(); ++lt_i) {
const auto lt_off = lt_offsets[lt_i];
min_inplace(mn_lt, {get<1>(lt)[lt_off]});
}
min_inplace(res, {get<0>(mn_interm0), get<1>(mn_interm0), get<0>(mn_lt)});
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}