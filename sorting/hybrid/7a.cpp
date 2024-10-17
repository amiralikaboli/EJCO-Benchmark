#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document N_CSV("../datasets/job/7a/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* name */ N_CSV.GetColumn<string>(1), /* size */ static_cast<int>(N_CSV.GetRowCount()));
const rapidcsv::Document PI_CSV("../datasets/job/7a/pi.csv", NO_HEADERS, SEPARATOR);
auto pi = tuple(/* person_id */ PI_CSV.GetColumn<int>(1), /* info_type_id */ PI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(PI_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/7a/it.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/7a/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document ML_CSV("../datasets/job/movie_link.csv", NO_HEADERS, SEPARATOR);
auto ml = tuple(/* linked_movie_id */ ML_CSV.GetColumn<int>(2), /* link_type_id */ ML_CSV.GetColumn<int>(3), /* size */ static_cast<int>(ML_CSV.GetRowCount()));
const rapidcsv::Document LT_CSV("../datasets/job/7a/lt.csv", NO_HEADERS, SEPARATOR);
auto lt = tuple(/* id */ LT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(LT_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/cast_info.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document AN_CSV("../datasets/job/7a/an.csv", NO_HEADERS, SEPARATOR);
auto an = tuple(/* person_id */ AN_CSV.GetColumn<int>(1), /* size */ static_cast<int>(AN_CSV.GetRowCount()));

int main() {
vector<int> pi_offsets(get<2>(pi));
iota(pi_offsets.begin(), pi_offsets.end(), 0);
sort(pi_offsets.begin(), pi_offsets.end(), [&](const int i, const int j) { return get<0>(pi)[i] < get<0>(pi)[j]; });
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
vector<int> ml_offsets(get<2>(ml));
iota(ml_offsets.begin(), ml_offsets.end(), 0);
sort(ml_offsets.begin(), ml_offsets.end(), [&](const int i, const int j) { return get<0>(ml)[i] < get<0>(ml)[j]; });
vector<int> lt_offsets(get<1>(lt));
iota(lt_offsets.begin(), lt_offsets.end(), 0);
sort(lt_offsets.begin(), lt_offsets.end(), [&](const int i, const int j) { return get<0>(lt)[i] < get<0>(lt)[j]; });
vector<int> an_offsets(get<1>(an));
iota(an_offsets.begin(), an_offsets.end(), 0);
sort(an_offsets.begin(), an_offsets.end(), [&](const int i, const int j) { return get<0>(an)[i] < get<0>(an)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, Range> pi_trie0(get<2>(pi));
for (int i = 0; i < get<2>(pi); ++i)
pi_trie0[get<0>(pi)[pi_offsets[i]]][i] += 1;
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<string, int>, 4>>(min(1000000, get<2>(n)));
for (auto n_off = 0; n_off < get<2>(n); ++n_off) {
const auto x0 = get<0>(n)[n_off];
const auto pi_it = pi_trie0.find(x0);
if (pi_it != pi_trie0.end()) {
const auto &pi_trie1 = pi_it->second;
for (auto pi_i = pi_trie1.left(); pi_i < pi_trie1.right(); ++pi_i) {
const auto pi_off = pi_offsets[pi_i];
const auto x1 = get<1>(pi)[pi_off];
const auto it_it = it_trie0.find(x1);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
interm0_trie0[get<0>(n)[n_off]][tuple<string, int>(get<1>(n)[n_off], get<1>(pi)[pi_off])] += 1;
}
}
}
}
SortedDict<int, Range> ml_trie0(get<2>(ml));
for (int i = 0; i < get<2>(ml); ++i)
ml_trie0[get<0>(ml)[ml_offsets[i]]][i] += 1;
SortedDict<int, int> lt_trie0(get<1>(lt));
for (int i = 0; i < get<1>(lt); ++i)
lt_trie0[get<0>(lt)[lt_offsets[i]]] += 1;
auto interm1_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<string, int>, 4>>(min(1000000, get<2>(t)));
for (auto t_off = 0; t_off < get<2>(t); ++t_off) {
const auto x0 = get<0>(t)[t_off];
const auto ml_it = ml_trie0.find(x0);
if (ml_it != ml_trie0.end()) {
const auto &ml_trie1 = ml_it->second;
for (auto ml_i = ml_trie1.left(); ml_i < ml_trie1.right(); ++ml_i) {
const auto ml_off = ml_offsets[ml_i];
const auto x1 = get<1>(ml)[ml_off];
const auto lt_it = lt_trie0.find(x1);
if (lt_it != lt_trie0.end()) {
const auto &lt_trie1 = lt_it->second;
interm1_trie0[get<0>(t)[t_off]][tuple<string, int>(get<1>(t)[t_off], get<1>(ml)[ml_off])] += 1;
}
}
}
}
SortedDict<int, int> an_trie0(get<1>(an));
for (int i = 0; i < get<1>(an); ++i)
an_trie0[get<0>(an)[an_offsets[i]]] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<2>(ci); ++ci_off) {
const auto x0 = get<0>(ci)[ci_off];
const auto interm0_it = interm0_trie0.find(x0);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto x1 = get<1>(ci)[ci_off];
const auto interm1_it = interm1_trie0.find(x1);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto an_it = an_trie0.find(x0);
if (an_it != an_trie0.end()) {
const auto &an_trie1 = an_it->second;
auto mn_interm1 = tuple<string>(STRING_MAX);
for (const auto &interm1_tuple: interm1_trie1) {
min_inplace(mn_interm1, {get<0>(interm1_tuple)});
}
auto mn_interm0 = tuple<string>(STRING_MAX);
for (const auto &interm0_tuple: interm0_trie1) {
min_inplace(mn_interm0, {get<0>(interm0_tuple)});
}
min_inplace(res, {get<0>(mn_interm1), get<0>(mn_interm0)});
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}