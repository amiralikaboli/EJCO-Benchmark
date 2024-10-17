#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document ML_CSV("../datasets/job/movie_link.csv", NO_HEADERS, SEPARATOR);
auto ml = tuple(/* movie_id */ ML_CSV.GetColumn<int>(1), /* linked_movie_id */ ML_CSV.GetColumn<int>(2), /* link_type_id */ ML_CSV.GetColumn<int>(3), /* size */ static_cast<int>(ML_CSV.GetRowCount()));
const rapidcsv::Document LT_CSV("../datasets/job/33b/lt.csv", NO_HEADERS, SEPARATOR);
auto lt = tuple(/* id */ LT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(LT_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX1_CSV("../datasets/job/movie_info_idx.csv", NO_HEADERS, SEPARATOR);
auto mi_idx1 = tuple(/* movie_id */ MI_IDX1_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX1_CSV.GetColumn<int>(2), /* info */ MI_IDX1_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX1_CSV.GetRowCount()));
const rapidcsv::Document IT1_CSV("../datasets/job/33b/it1.csv", NO_HEADERS, SEPARATOR);
auto it1 = tuple(/* id */ IT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT1_CSV.GetRowCount()));
const rapidcsv::Document MI_IDX2_CSV("../datasets/job/33b/mi_idx2.csv", NO_HEADERS, SEPARATOR);
auto mi_idx2 = tuple(/* movie_id */ MI_IDX2_CSV.GetColumn<int>(1), /* info_type_id */ MI_IDX2_CSV.GetColumn<int>(2), /* info */ MI_IDX2_CSV.GetColumn<string>(3), /* size */ static_cast<int>(MI_IDX2_CSV.GetRowCount()));
const rapidcsv::Document IT2_CSV("../datasets/job/33b/it2.csv", NO_HEADERS, SEPARATOR);
auto it2 = tuple(/* id */ IT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT2_CSV.GetRowCount()));
const rapidcsv::Document T1_CSV("../datasets/job/title.csv", NO_HEADERS, SEPARATOR);
auto t1 = tuple(/* id */ T1_CSV.GetColumn<int>(0), /* title */ T1_CSV.GetColumn<string>(1), /* kind_id */ T1_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T1_CSV.GetRowCount()));
const rapidcsv::Document KT1_CSV("../datasets/job/33b/kt1.csv", NO_HEADERS, SEPARATOR);
auto kt1 = tuple(/* id */ KT1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT1_CSV.GetRowCount()));
const rapidcsv::Document T2_CSV("../datasets/job/33b/t2.csv", NO_HEADERS, SEPARATOR);
auto t2 = tuple(/* id */ T2_CSV.GetColumn<int>(0), /* title */ T2_CSV.GetColumn<string>(1), /* kind_id */ T2_CSV.GetColumn<int>(3), /* size */ static_cast<int>(T2_CSV.GetRowCount()));
const rapidcsv::Document KT2_CSV("../datasets/job/33b/kt2.csv", NO_HEADERS, SEPARATOR);
auto kt2 = tuple(/* id */ KT2_CSV.GetColumn<int>(0), /* size */ static_cast<int>(KT2_CSV.GetRowCount()));
const rapidcsv::Document MC1_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc1 = tuple(/* movie_id */ MC1_CSV.GetColumn<int>(1), /* company_id */ MC1_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC1_CSV.GetRowCount()));
const rapidcsv::Document CN1_CSV("../datasets/job/33b/cn1.csv", NO_HEADERS, SEPARATOR);
auto cn1 = tuple(/* id */ CN1_CSV.GetColumn<int>(0), /* name */ CN1_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CN1_CSV.GetRowCount()));
const rapidcsv::Document MC2_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc2 = tuple(/* movie_id */ MC2_CSV.GetColumn<int>(1), /* company_id */ MC2_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC2_CSV.GetRowCount()));
const rapidcsv::Document CN2_CSV("../datasets/job/company_name.csv", NO_HEADERS, SEPARATOR);
auto cn2 = tuple(/* id */ CN2_CSV.GetColumn<int>(0), /* name */ CN2_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CN2_CSV.GetRowCount()));

int main() {
vector<int> lt_offsets(get<1>(lt));
iota(lt_offsets.begin(), lt_offsets.end(), 0);
sort(lt_offsets.begin(), lt_offsets.end(), [&](const int i, const int j) { return get<0>(lt)[i] < get<0>(lt)[j]; });
vector<int> it1_offsets(get<1>(it1));
iota(it1_offsets.begin(), it1_offsets.end(), 0);
sort(it1_offsets.begin(), it1_offsets.end(), [&](const int i, const int j) { return get<0>(it1)[i] < get<0>(it1)[j]; });
vector<int> it2_offsets(get<1>(it2));
iota(it2_offsets.begin(), it2_offsets.end(), 0);
sort(it2_offsets.begin(), it2_offsets.end(), [&](const int i, const int j) { return get<0>(it2)[i] < get<0>(it2)[j]; });
vector<int> kt1_offsets(get<1>(kt1));
iota(kt1_offsets.begin(), kt1_offsets.end(), 0);
sort(kt1_offsets.begin(), kt1_offsets.end(), [&](const int i, const int j) { return get<0>(kt1)[i] < get<0>(kt1)[j]; });
vector<int> kt2_offsets(get<1>(kt2));
iota(kt2_offsets.begin(), kt2_offsets.end(), 0);
sort(kt2_offsets.begin(), kt2_offsets.end(), [&](const int i, const int j) { return get<0>(kt2)[i] < get<0>(kt2)[j]; });
vector<int> cn1_offsets(get<2>(cn1));
iota(cn1_offsets.begin(), cn1_offsets.end(), 0);
sort(cn1_offsets.begin(), cn1_offsets.end(), [&](const int i, const int j) { return get<0>(cn1)[i] < get<0>(cn1)[j]; });
vector<int> cn2_offsets(get<2>(cn2));
iota(cn2_offsets.begin(), cn2_offsets.end(), 0);
sort(cn2_offsets.begin(), cn2_offsets.end(), [&](const int i, const int j) { return get<0>(cn2)[i] < get<0>(cn2)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> lt_trie0(get<1>(lt));
for (int i = 0; i < get<1>(lt); ++i)
lt_trie0[get<0>(lt)[lt_offsets[i]]] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int>, 4>>(min(1000000, get<3>(ml)));
for (auto ml_off = 0; ml_off < get<3>(ml); ++ml_off) {
const auto x0 = get<2>(ml)[ml_off];
const auto lt_it = lt_trie0.find(x0);
if (lt_it != lt_trie0.end()) {
const auto &lt_trie1 = lt_it->second;
interm0_trie0[get<0>(ml)[ml_off]][tuple<int, int>(get<2>(ml)[ml_off], get<1>(ml)[ml_off])] += 1;
}
}
SortedDict<int, int> it1_trie0(get<1>(it1));
for (int i = 0; i < get<1>(it1); ++i)
it1_trie0[get<0>(it1)[it1_offsets[i]]] += 1;
auto interm1_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string>, 4>>(min(1000000, get<3>(mi_idx1)));
for (auto mi_idx1_off = 0; mi_idx1_off < get<3>(mi_idx1); ++mi_idx1_off) {
const auto x0 = get<1>(mi_idx1)[mi_idx1_off];
const auto it1_it = it1_trie0.find(x0);
if (it1_it != it1_trie0.end()) {
const auto &it1_trie1 = it1_it->second;
const auto x1 = get<0>(mi_idx1)[mi_idx1_off];
const auto interm0_it = interm0_trie0.find(x1);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
for (const auto &interm0_tuple: interm0_trie1) {
interm1_trie0[get<1>(interm0_tuple)][tuple<int, int, string>(get<1>(mi_idx1)[mi_idx1_off], get<0>(mi_idx1)[mi_idx1_off], get<2>(mi_idx1)[mi_idx1_off])] += 1;
}
}
}
}
SortedDict<int, int> it2_trie0(get<1>(it2));
for (int i = 0; i < get<1>(it2); ++i)
it2_trie0[get<0>(it2)[it2_offsets[i]]] += 1;
auto interm2_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string, string>, 4>>(min(1000000, get<3>(mi_idx2)));
for (auto mi_idx2_off = 0; mi_idx2_off < get<3>(mi_idx2); ++mi_idx2_off) {
const auto x0 = get<1>(mi_idx2)[mi_idx2_off];
const auto it2_it = it2_trie0.find(x0);
if (it2_it != it2_trie0.end()) {
const auto &it2_trie1 = it2_it->second;
const auto x1 = get<0>(mi_idx2)[mi_idx2_off];
const auto interm1_it = interm1_trie0.find(x1);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
for (const auto &interm1_tuple: interm1_trie1) {
interm2_trie0[get<1>(interm1_tuple)][tuple<int, int, string, string>(get<1>(mi_idx2)[mi_idx2_off], get<0>(mi_idx2)[mi_idx2_off], get<2>(mi_idx2)[mi_idx2_off], get<2>(interm1_tuple))] += 1;
}
}
}
}
SortedDict<int, int> kt1_trie0(get<1>(kt1));
for (int i = 0; i < get<1>(kt1); ++i)
kt1_trie0[get<0>(kt1)[kt1_offsets[i]]] += 1;
auto interm3_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string, string, string>, 4>>(min(1000000, get<3>(t1)));
for (auto t1_off = 0; t1_off < get<3>(t1); ++t1_off) {
const auto x0 = get<2>(t1)[t1_off];
const auto kt1_it = kt1_trie0.find(x0);
if (kt1_it != kt1_trie0.end()) {
const auto &kt1_trie1 = kt1_it->second;
const auto x1 = get<0>(t1)[t1_off];
const auto interm2_it = interm2_trie0.find(x1);
if (interm2_it != interm2_trie0.end()) {
auto &interm2_trie1 = interm2_it->second;
for (const auto &interm2_tuple: interm2_trie1) {
interm3_trie0[get<1>(interm2_tuple)][tuple<int, int, string, string, string>(get<2>(t1)[t1_off], get<0>(t1)[t1_off], get<1>(t1)[t1_off], get<2>(interm2_tuple), get<3>(interm2_tuple))] += 1;
}
}
}
}
SortedDict<int, int> kt2_trie0(get<1>(kt2));
for (int i = 0; i < get<1>(kt2); ++i)
kt2_trie0[get<0>(kt2)[kt2_offsets[i]]] += 1;
auto interm4_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string, string, string, string>, 4>>(min(1000000, get<3>(t2)));
for (auto t2_off = 0; t2_off < get<3>(t2); ++t2_off) {
const auto x0 = get<2>(t2)[t2_off];
const auto kt2_it = kt2_trie0.find(x0);
if (kt2_it != kt2_trie0.end()) {
const auto &kt2_trie1 = kt2_it->second;
const auto x1 = get<0>(t2)[t2_off];
const auto interm3_it = interm3_trie0.find(x1);
if (interm3_it != interm3_trie0.end()) {
auto &interm3_trie1 = interm3_it->second;
for (const auto &interm3_tuple: interm3_trie1) {
interm4_trie0[get<1>(interm3_tuple)][tuple<int, int, string, string, string, string>(get<2>(t2)[t2_off], get<0>(t2)[t2_off], get<1>(t2)[t2_off], get<2>(interm3_tuple), get<3>(interm3_tuple), get<4>(interm3_tuple))] += 1;
}
}
}
}
SortedDict<int, Range> cn1_trie0(get<2>(cn1));
for (int i = 0; i < get<2>(cn1); ++i)
cn1_trie0[get<0>(cn1)[cn1_offsets[i]]][i] += 1;
auto interm5_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string, string, string, string, string>, 4>>(min(1000000, get<2>(mc1)));
for (auto mc1_off = 0; mc1_off < get<2>(mc1); ++mc1_off) {
const auto x0 = get<0>(mc1)[mc1_off];
const auto interm4_it = interm4_trie0.find(x0);
if (interm4_it != interm4_trie0.end()) {
auto &interm4_trie1 = interm4_it->second;
const auto x1 = get<1>(mc1)[mc1_off];
const auto cn1_it = cn1_trie0.find(x1);
if (cn1_it != cn1_trie0.end()) {
const auto &cn1_trie1 = cn1_it->second;
for (auto cn1_i = cn1_trie1.left(); cn1_i < cn1_trie1.right(); ++cn1_i) {
const auto cn1_off = cn1_offsets[cn1_i];
for (const auto &interm4_tuple: interm4_trie1) {
interm5_trie0[get<1>(interm4_tuple)][tuple<int, int, string, string, string, string, string>(get<1>(mc1)[mc1_off], get<0>(mc1)[mc1_off], get<1>(cn1)[cn1_off], get<2>(interm4_tuple), get<3>(interm4_tuple), get<4>(interm4_tuple), get<5>(interm4_tuple))] += 1;
}
}
}
}
}
SortedDict<int, Range> cn2_trie0(get<2>(cn2));
for (int i = 0; i < get<2>(cn2); ++i)
cn2_trie0[get<0>(cn2)[cn2_offsets[i]]][i] += 1;
auto res = tuple<string, string, string, string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX, STRING_MAX, STRING_MAX, STRING_MAX);
for (auto mc2_off = 0; mc2_off < get<2>(mc2); ++mc2_off) {
const auto x0 = get<0>(mc2)[mc2_off];
const auto interm5_it = interm5_trie0.find(x0);
if (interm5_it != interm5_trie0.end()) {
auto &interm5_trie1 = interm5_it->second;
const auto x1 = get<1>(mc2)[mc2_off];
const auto cn2_it = cn2_trie0.find(x1);
if (cn2_it != cn2_trie0.end()) {
const auto &cn2_trie1 = cn2_it->second;
auto mn_cn2 = tuple<string>(STRING_MAX);
for (auto cn2_i = cn2_trie1.left(); cn2_i < cn2_trie1.right(); ++cn2_i) {
const auto cn2_off = cn2_offsets[cn2_i];
min_inplace(mn_cn2, {get<1>(cn2)[cn2_off]});
}
auto mn_interm5 = tuple<string, string, string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX, STRING_MAX, STRING_MAX);
for (const auto &interm5_tuple: interm5_trie1) {
min_inplace(mn_interm5, {get<2>(interm5_tuple), get<3>(interm5_tuple), get<4>(interm5_tuple), get<5>(interm5_tuple), get<6>(interm5_tuple)});
}
min_inplace(res, {get<0>(mn_cn2), get<0>(mn_interm5), get<1>(mn_interm5), get<2>(mn_interm5), get<3>(mn_interm5), get<4>(mn_interm5)});
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << " | " << get<2>(res) << " | " << get<3>(res) << " | " << get<4>(res) << " | " << get<5>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}