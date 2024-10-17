#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* company_type_id */ MC_CSV.GetColumn<int>(3), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/10a/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document CT_CSV("../datasets/job/company_type.csv", NO_HEADERS, SEPARATOR);
auto ct = tuple(/* id */ CT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/10a/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/10a/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* movie_id */ CI_CSV.GetColumn<int>(2), /* person_role_id */ CI_CSV.GetColumn<int>(3), /* role_id */ CI_CSV.GetColumn<int>(6), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document RT_CSV("../datasets/job/10a/rt.csv", NO_HEADERS, SEPARATOR);
auto rt = tuple(/* id */ RT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(RT_CSV.GetRowCount()));
const rapidcsv::Document CHN_CSV("../datasets/job/char_name.csv", NO_HEADERS, SEPARATOR);
auto chn = tuple(/* id */ CHN_CSV.GetColumn<int>(0), /* name */ CHN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CHN_CSV.GetRowCount()));

int main() {
vector<int> ct_offsets(get<1>(ct));
iota(ct_offsets.begin(), ct_offsets.end(), 0);
sort(ct_offsets.begin(), ct_offsets.end(), [&](const int i, const int j) { return get<0>(ct)[i] < get<0>(ct)[j]; });
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> rt_offsets(get<1>(rt));
iota(rt_offsets.begin(), rt_offsets.end(), 0);
sort(rt_offsets.begin(), rt_offsets.end(), [&](const int i, const int j) { return get<0>(rt)[i] < get<0>(rt)[j]; });
vector<int> chn_offsets(get<2>(chn));
iota(chn_offsets.begin(), chn_offsets.end(), 0);
sort(chn_offsets.begin(), chn_offsets.end(), [&](const int i, const int j) { return get<0>(chn)[i] < get<0>(chn)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> ct_trie0(get<1>(ct));
for (int i = 0; i < get<1>(ct); ++i)
ct_trie0[get<0>(ct)[ct_offsets[i]]] += 1;
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
auto interm0_trie0 = phmap::flat_hash_map<int, smallvecdict<tuple<int, int, string>, 4>>(min(1000000, get<3>(mc)));
for (auto mc_off = 0; mc_off < get<3>(mc); ++mc_off) {
const auto x0 = get<2>(mc)[mc_off];
const auto ct_it = ct_trie0.find(x0);
if (ct_it != ct_trie0.end()) {
const auto &ct_trie1 = ct_it->second;
const auto x1 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x1);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
const auto x2 = get<0>(mc)[mc_off];
const auto t_it = t_trie0.find(x2);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
interm0_trie0[get<0>(mc)[mc_off]][tuple<int, int, string>(get<2>(mc)[mc_off], get<1>(mc)[mc_off], get<1>(t)[t_off])] += 1;
}
}
}
}
}
SortedDict<int, int> rt_trie0(get<1>(rt));
for (int i = 0; i < get<1>(rt); ++i)
rt_trie0[get<0>(rt)[rt_offsets[i]]] += 1;
SortedDict<int, Range> chn_trie0(get<2>(chn));
for (int i = 0; i < get<2>(chn); ++i)
chn_trie0[get<0>(chn)[chn_offsets[i]]][i] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<3>(ci); ++ci_off) {
const auto x0 = get<2>(ci)[ci_off];
const auto rt_it = rt_trie0.find(x0);
if (rt_it != rt_trie0.end()) {
const auto &rt_trie1 = rt_it->second;
const auto x1 = get<0>(ci)[ci_off];
const auto interm0_it = interm0_trie0.find(x1);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto x2 = get<1>(ci)[ci_off];
const auto chn_it = chn_trie0.find(x2);
if (chn_it != chn_trie0.end()) {
const auto &chn_trie1 = chn_it->second;
auto mn_interm0 = tuple<string>(STRING_MAX);
for (const auto &interm0_tuple: interm0_trie1) {
min_inplace(mn_interm0, {get<2>(interm0_tuple)});
}
auto mn_chn = tuple<string>(STRING_MAX);
for (auto chn_i = chn_trie1.left(); chn_i < chn_trie1.right(); ++chn_i) {
const auto chn_off = chn_offsets[chn_i];
min_inplace(mn_chn, {get<1>(chn)[chn_off]});
}
min_inplace(res, {get<0>(mn_interm0), get<0>(mn_chn)});
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}