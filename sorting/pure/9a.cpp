#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MC_CSV("../datasets/job/9a/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/9a/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/9a/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* person_role_id */ CI_CSV.GetColumn<int>(3), /* role_id */ CI_CSV.GetColumn<int>(6), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/9a/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* size */ static_cast<int>(N_CSV.GetRowCount()));
const rapidcsv::Document RT_CSV("../datasets/job/9a/rt.csv", NO_HEADERS, SEPARATOR);
auto rt = tuple(/* id */ RT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(RT_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/9a/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document CHN_CSV("../datasets/job/char_name.csv", NO_HEADERS, SEPARATOR);
auto chn = tuple(/* id */ CHN_CSV.GetColumn<int>(0), /* name */ CHN_CSV.GetColumn<string>(1), /* size */ static_cast<int>(CHN_CSV.GetRowCount()));
const rapidcsv::Document AN_CSV("../datasets/job/aka_name.csv", NO_HEADERS, SEPARATOR);
auto an = tuple(/* person_id */ AN_CSV.GetColumn<int>(1), /* name */ AN_CSV.GetColumn<string>(2), /* size */ static_cast<int>(AN_CSV.GetRowCount()));

int main() {
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> rt_offsets(get<1>(rt));
iota(rt_offsets.begin(), rt_offsets.end(), 0);
sort(rt_offsets.begin(), rt_offsets.end(), [&](const int i, const int j) { return get<0>(rt)[i] < get<0>(rt)[j]; });
vector<int> n_offsets(get<1>(n));
iota(n_offsets.begin(), n_offsets.end(), 0);
sort(n_offsets.begin(), n_offsets.end(), [&](const int i, const int j) { return get<0>(n)[i] < get<0>(n)[j]; });
vector<int> an_offsets(get<2>(an));
iota(an_offsets.begin(), an_offsets.end(), 0);
sort(an_offsets.begin(), an_offsets.end(), [&](const int i, const int j) { return get<0>(an)[i] < get<0>(an)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> chn_offsets(get<2>(chn));
iota(chn_offsets.begin(), chn_offsets.end(), 0);
sort(chn_offsets.begin(), chn_offsets.end(), [&](const int i, const int j) { return get<0>(chn)[i] < get<0>(chn)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
vector<tuple<int, int>> interm0;
for (auto mc_off = 0; mc_off < get<2>(mc); ++mc_off) {
const auto x0 = get<1>(mc)[mc_off];
const auto cn_it = cn_trie0.find(x0);
if (cn_it != cn_trie0.end()) {
const auto &cn_trie1 = cn_it->second;
interm0.emplace_back(get<0>(mc)[mc_off], get<1>(mc)[mc_off]);}
}
SortedDict<int, int> rt_trie0(get<1>(rt));
for (int i = 0; i < get<1>(rt); ++i)
rt_trie0[get<0>(rt)[rt_offsets[i]]] += 1;
SortedDict<int, int> n_trie0(get<1>(n));
for (int i = 0; i < get<1>(n); ++i)
n_trie0[get<0>(n)[n_offsets[i]]] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int> &a, const tuple<int, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, Range> an_trie0(get<2>(an));
for (int i = 0; i < get<2>(an); ++i)
an_trie0[get<0>(an)[an_offsets[i]]][i] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, Range> chn_trie0(get<2>(chn));
for (int i = 0; i < get<2>(chn); ++i)
chn_trie0[get<0>(chn)[chn_offsets[i]]][i] += 1;
auto res = tuple<string, string, string>(STRING_MAX, STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<4>(ci); ++ci_off) {
const auto x0 = get<3>(ci)[ci_off];
const auto rt_it = rt_trie0.find(x0);
if (rt_it != rt_trie0.end()) {
const auto &rt_trie1 = rt_it->second;
const auto x1 = get<0>(ci)[ci_off];
const auto n_it = n_trie0.find(x1);
if (n_it != n_trie0.end()) {
const auto &n_trie1 = n_it->second;
const auto x2 = get<1>(ci)[ci_off];
const auto interm0_it = interm0_trie0.find(x2);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto an_it = an_trie0.find(x1);
if (an_it != an_trie0.end()) {
const auto &an_trie1 = an_it->second;
const auto t_it = t_trie0.find(x2);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
const auto x5 = get<2>(ci)[ci_off];
const auto chn_it = chn_trie0.find(x5);
if (chn_it != chn_trie0.end()) {
const auto &chn_trie1 = chn_it->second;
auto mn_t = tuple<string>(STRING_MAX);
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
min_inplace(mn_t, {get<1>(t)[t_off]});
}
auto mn_chn = tuple<string>(STRING_MAX);
for (auto chn_i = chn_trie1.left(); chn_i < chn_trie1.right(); ++chn_i) {
const auto chn_off = chn_offsets[chn_i];
min_inplace(mn_chn, {get<1>(chn)[chn_off]});
}
auto mn_an = tuple<string>(STRING_MAX);
for (auto an_i = an_trie1.left(); an_i < an_trie1.right(); ++an_i) {
const auto an_off = an_offsets[an_i];
min_inplace(mn_an, {get<1>(an)[an_off]});
}
min_inplace(res, {get<0>(mn_t), get<0>(mn_chn), get<0>(mn_an)});
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