#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MC_CSV("../datasets/job/8a/mc.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/8a/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/8a/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* role_id */ CI_CSV.GetColumn<int>(6), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document N1_CSV("../datasets/job/8a/n1.csv", NO_HEADERS, SEPARATOR);
auto n1 = tuple(/* id */ N1_CSV.GetColumn<int>(0), /* size */ static_cast<int>(N1_CSV.GetRowCount()));
const rapidcsv::Document RT_CSV("../datasets/job/8a/rt.csv", NO_HEADERS, SEPARATOR);
auto rt = tuple(/* id */ RT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(RT_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/title.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document AN1_CSV("../datasets/job/aka_name.csv", NO_HEADERS, SEPARATOR);
auto an1 = tuple(/* person_id */ AN1_CSV.GetColumn<int>(1), /* name */ AN1_CSV.GetColumn<string>(2), /* size */ static_cast<int>(AN1_CSV.GetRowCount()));

int main() {
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> rt_offsets(get<1>(rt));
iota(rt_offsets.begin(), rt_offsets.end(), 0);
sort(rt_offsets.begin(), rt_offsets.end(), [&](const int i, const int j) { return get<0>(rt)[i] < get<0>(rt)[j]; });
vector<int> n1_offsets(get<1>(n1));
iota(n1_offsets.begin(), n1_offsets.end(), 0);
sort(n1_offsets.begin(), n1_offsets.end(), [&](const int i, const int j) { return get<0>(n1)[i] < get<0>(n1)[j]; });
vector<int> an1_offsets(get<2>(an1));
iota(an1_offsets.begin(), an1_offsets.end(), 0);
sort(an1_offsets.begin(), an1_offsets.end(), [&](const int i, const int j) { return get<0>(an1)[i] < get<0>(an1)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
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
SortedDict<int, int> n1_trie0(get<1>(n1));
for (int i = 0; i < get<1>(n1); ++i)
n1_trie0[get<0>(n1)[n1_offsets[i]]] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int> &a, const tuple<int, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, Range> an1_trie0(get<2>(an1));
for (int i = 0; i < get<2>(an1); ++i)
an1_trie0[get<0>(an1)[an1_offsets[i]]][i] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<3>(ci); ++ci_off) {
const auto x0 = get<2>(ci)[ci_off];
const auto rt_it = rt_trie0.find(x0);
if (rt_it != rt_trie0.end()) {
const auto &rt_trie1 = rt_it->second;
const auto x1 = get<0>(ci)[ci_off];
const auto n1_it = n1_trie0.find(x1);
if (n1_it != n1_trie0.end()) {
const auto &n1_trie1 = n1_it->second;
const auto x2 = get<1>(ci)[ci_off];
const auto interm0_it = interm0_trie0.find(x2);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto an1_it = an1_trie0.find(x1);
if (an1_it != an1_trie0.end()) {
const auto &an1_trie1 = an1_it->second;
const auto t_it = t_trie0.find(x2);
if (t_it != t_trie0.end()) {
const auto &t_trie1 = t_it->second;
auto mn_t = tuple<string>(STRING_MAX);
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
min_inplace(mn_t, {get<1>(t)[t_off]});
}
auto mn_an1 = tuple<string>(STRING_MAX);
for (auto an1_i = an1_trie1.left(); an1_i < an1_trie1.right(); ++an1_i) {
const auto an1_off = an1_offsets[an1_i];
min_inplace(mn_an1, {get<1>(an1)[an1_off]});
}
min_inplace(res, {get<0>(mn_t), get<0>(mn_an1)});
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