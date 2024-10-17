#include "../runtime/headers.h"

using namespace std;

const rapidcsv::Document MI_CSV("../datasets/job/19c/mi.csv", NO_HEADERS, SEPARATOR);
auto mi = tuple(/* movie_id */ MI_CSV.GetColumn<int>(1), /* info_type_id */ MI_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MI_CSV.GetRowCount()));
const rapidcsv::Document T_CSV("../datasets/job/19c/t.csv", NO_HEADERS, SEPARATOR);
auto t = tuple(/* id */ T_CSV.GetColumn<int>(0), /* title */ T_CSV.GetColumn<string>(1), /* size */ static_cast<int>(T_CSV.GetRowCount()));
const rapidcsv::Document MC_CSV("../datasets/job/movie_companies.csv", NO_HEADERS, SEPARATOR);
auto mc = tuple(/* movie_id */ MC_CSV.GetColumn<int>(1), /* company_id */ MC_CSV.GetColumn<int>(2), /* size */ static_cast<int>(MC_CSV.GetRowCount()));
const rapidcsv::Document IT_CSV("../datasets/job/19c/it.csv", NO_HEADERS, SEPARATOR);
auto it = tuple(/* id */ IT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(IT_CSV.GetRowCount()));
const rapidcsv::Document CN_CSV("../datasets/job/19c/cn.csv", NO_HEADERS, SEPARATOR);
auto cn = tuple(/* id */ CN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CN_CSV.GetRowCount()));
const rapidcsv::Document N_CSV("../datasets/job/19c/n.csv", NO_HEADERS, SEPARATOR);
auto n = tuple(/* id */ N_CSV.GetColumn<int>(0), /* name */ N_CSV.GetColumn<string>(1), /* size */ static_cast<int>(N_CSV.GetRowCount()));
const rapidcsv::Document AN_CSV("../datasets/job/aka_name.csv", NO_HEADERS, SEPARATOR);
auto an = tuple(/* person_id */ AN_CSV.GetColumn<int>(1), /* size */ static_cast<int>(AN_CSV.GetRowCount()));
const rapidcsv::Document CI_CSV("../datasets/job/19c/ci.csv", NO_HEADERS, SEPARATOR);
auto ci = tuple(/* person_id */ CI_CSV.GetColumn<int>(1), /* movie_id */ CI_CSV.GetColumn<int>(2), /* person_role_id */ CI_CSV.GetColumn<int>(3), /* role_id */ CI_CSV.GetColumn<int>(6), /* size */ static_cast<int>(CI_CSV.GetRowCount()));
const rapidcsv::Document RT_CSV("../datasets/job/19c/rt.csv", NO_HEADERS, SEPARATOR);
auto rt = tuple(/* id */ RT_CSV.GetColumn<int>(0), /* size */ static_cast<int>(RT_CSV.GetRowCount()));
const rapidcsv::Document CHN_CSV("../datasets/job/char_name.csv", NO_HEADERS, SEPARATOR);
auto chn = tuple(/* id */ CHN_CSV.GetColumn<int>(0), /* size */ static_cast<int>(CHN_CSV.GetRowCount()));

int main() {
vector<int> it_offsets(get<1>(it));
iota(it_offsets.begin(), it_offsets.end(), 0);
sort(it_offsets.begin(), it_offsets.end(), [&](const int i, const int j) { return get<0>(it)[i] < get<0>(it)[j]; });
vector<int> t_offsets(get<2>(t));
iota(t_offsets.begin(), t_offsets.end(), 0);
sort(t_offsets.begin(), t_offsets.end(), [&](const int i, const int j) { return get<0>(t)[i] < get<0>(t)[j]; });
vector<int> mc_offsets(get<2>(mc));
iota(mc_offsets.begin(), mc_offsets.end(), 0);
sort(mc_offsets.begin(), mc_offsets.end(), [&](const int i, const int j) { return get<0>(mc)[i] < get<0>(mc)[j]; });
vector<int> cn_offsets(get<1>(cn));
iota(cn_offsets.begin(), cn_offsets.end(), 0);
sort(cn_offsets.begin(), cn_offsets.end(), [&](const int i, const int j) { return get<0>(cn)[i] < get<0>(cn)[j]; });
vector<int> an_offsets(get<1>(an));
iota(an_offsets.begin(), an_offsets.end(), 0);
sort(an_offsets.begin(), an_offsets.end(), [&](const int i, const int j) { return get<0>(an)[i] < get<0>(an)[j]; });
vector<int> rt_offsets(get<1>(rt));
iota(rt_offsets.begin(), rt_offsets.end(), 0);
sort(rt_offsets.begin(), rt_offsets.end(), [&](const int i, const int j) { return get<0>(rt)[i] < get<0>(rt)[j]; });
vector<int> chn_offsets(get<1>(chn));
iota(chn_offsets.begin(), chn_offsets.end(), 0);
sort(chn_offsets.begin(), chn_offsets.end(), [&](const int i, const int j) { return get<0>(chn)[i] < get<0>(chn)[j]; });
HighPrecisionTimer timer;
for (int iter = 0; iter < 5 + 1; ++iter) {
timer.Reset();
SortedDict<int, int> it_trie0(get<1>(it));
for (int i = 0; i < get<1>(it); ++i)
it_trie0[get<0>(it)[it_offsets[i]]] += 1;
SortedDict<int, Range> t_trie0(get<2>(t));
for (int i = 0; i < get<2>(t); ++i)
t_trie0[get<0>(t)[t_offsets[i]]][i] += 1;
SortedDict<int, Range> mc_trie0(get<2>(mc));
for (int i = 0; i < get<2>(mc); ++i)
mc_trie0[get<0>(mc)[mc_offsets[i]]][i] += 1;
SortedDict<int, int> cn_trie0(get<1>(cn));
for (int i = 0; i < get<1>(cn); ++i)
cn_trie0[get<0>(cn)[cn_offsets[i]]] += 1;
vector<tuple<int, int, string, int>> interm0;
for (auto mi_off = 0; mi_off < get<2>(mi); ++mi_off) {
const auto x0 = get<1>(mi)[mi_off];
const auto it_it = it_trie0.find(x0);
if (it_it != it_trie0.end()) {
const auto &it_trie1 = it_it->second;
const auto x1 = get<0>(mi)[mi_off];
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
for (auto t_i = t_trie1.left(); t_i < t_trie1.right(); ++t_i) {
const auto t_off = t_offsets[t_i];
interm0.emplace_back(get<0>(mi)[mi_off], get<1>(mi)[mi_off], get<1>(t)[t_off], get<1>(mc)[mc_off]);}
}
}
}
}
}
}
SortedDict<int, int> an_trie0(get<1>(an));
for (int i = 0; i < get<1>(an); ++i)
an_trie0[get<0>(an)[an_offsets[i]]] += 1;
vector<tuple<int, string>> interm1;
for (auto n_off = 0; n_off < get<2>(n); ++n_off) {
const auto x0 = get<0>(n)[n_off];
const auto an_it = an_trie0.find(x0);
if (an_it != an_trie0.end()) {
const auto &an_trie1 = an_it->second;
interm1.emplace_back(get<0>(n)[n_off], get<1>(n)[n_off]);}
}
SortedDict<int, int> rt_trie0(get<1>(rt));
for (int i = 0; i < get<1>(rt); ++i)
rt_trie0[get<0>(rt)[rt_offsets[i]]] += 1;
sort(interm1.begin(), interm1.end(), [](const tuple<int, string> &a, const tuple<int, string> &b) { return get<0>(a) < get<0>(b); });
auto interm1_trie0 = SortedDict<int, Range>(interm1.size());
for (int i = 0; i < interm1.size(); ++i)
interm1_trie0[get<0>(interm1[i])][i] += 1;
sort(interm0.begin(), interm0.end(), [](const tuple<int, int, string, int> &a, const tuple<int, int, string, int> &b) { return get<0>(a) < get<0>(b); });
auto interm0_trie0 = SortedDict<int, Range>(interm0.size());
for (int i = 0; i < interm0.size(); ++i)
interm0_trie0[get<0>(interm0[i])][i] += 1;
SortedDict<int, int> chn_trie0(get<1>(chn));
for (int i = 0; i < get<1>(chn); ++i)
chn_trie0[get<0>(chn)[chn_offsets[i]]] += 1;
auto res = tuple<string, string>(STRING_MAX, STRING_MAX);
for (auto ci_off = 0; ci_off < get<4>(ci); ++ci_off) {
const auto x0 = get<3>(ci)[ci_off];
const auto rt_it = rt_trie0.find(x0);
if (rt_it != rt_trie0.end()) {
const auto &rt_trie1 = rt_it->second;
const auto x1 = get<0>(ci)[ci_off];
const auto interm1_it = interm1_trie0.find(x1);
if (interm1_it != interm1_trie0.end()) {
auto &interm1_trie1 = interm1_it->second;
const auto x2 = get<1>(ci)[ci_off];
const auto interm0_it = interm0_trie0.find(x2);
if (interm0_it != interm0_trie0.end()) {
auto &interm0_trie1 = interm0_it->second;
const auto x3 = get<2>(ci)[ci_off];
const auto chn_it = chn_trie0.find(x3);
if (chn_it != chn_trie0.end()) {
const auto &chn_trie1 = chn_it->second;
auto mn_interm1 = tuple<string>(STRING_MAX);
for (auto interm1_off = interm1_trie1.left(); interm1_off < interm1_trie1.right(); ++interm1_off) {
const auto &interm1_tuple = interm1[interm1_off];
min_inplace(mn_interm1, {get<1>(interm1_tuple)});
}
auto mn_interm0 = tuple<string>(STRING_MAX);
for (auto interm0_off = interm0_trie1.left(); interm0_off < interm0_trie1.right(); ++interm0_off) {
const auto &interm0_tuple = interm0[interm0_off];
min_inplace(mn_interm0, {get<2>(interm0_tuple)});
}
min_inplace(res, {get<0>(mn_interm1), get<0>(mn_interm0)});
}
}
}
}
}
timer.StoreElapsedTime(0);
cout << "* " << get<0>(res) << " | " << get<1>(res) << endl;
}cout << timer.GetMean(0) << " ms" << endl;
}