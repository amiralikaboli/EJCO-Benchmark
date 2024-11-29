#include "../runtime/headers.h"

using namespace std;

int main(int argc, char *argv[]) {
    const int R_size = stoi(argv[1]) * 100000;
    const int S_size = stoi(argv[2]) * 100000;
    const int rep = stoi(argv[3]);
    const int R_limit = R_size / rep;
    const int S_limit = R_limit * 2;
    const int iters = stoi(argv[4]);

    srand(42);

    vector<int> R(R_size);
    vector<int> S(S_size);
    for (int i = 0; i < R_size; i++)
        R[i] = rand() % R_limit;
    for (int i = 0; i < S_size; i++)
        S[i] = rand() % S_limit;
    
    HighPrecisionTimer timer;
    
    int hash_ans;
    for (int iter = 0; iter <= iters; iter++) {
        hash_ans = 0;
        timer.Reset();
        phmap::flat_hash_map<int, smallvecdict<int ,4>> R_trie(R_size);
        for (int i = 0; i < R_size; i++)
            R_trie[R[i]][i] += 1;

        for (int i = 0; i < S_size; i++) {
            auto it = R_trie.find(S[i]);
            if (it != R_trie.end())
                ++hash_ans;
        }
        timer.StoreElapsedTime(0);
    }

    sort(R.begin(), R.end());
    sort(S.begin(), S.end());
    
    int sort_ans;
    for (int iter = 0; iter <= iters; iter++) {
        sort_ans = 0;
        timer.Reset();
        SortedDict<int, Range> R_trie(R_size);
        for (int i = 0; i < R_size; i++)
            R_trie[R[i]][i] += 1;

        for (int i = 0; i < S_size; i++) {
            auto it = R_trie.find(S[i]);
            if (it != R_trie.end())
                ++sort_ans;
        }
        timer.StoreElapsedTime(1);
    }

    assert(hash_ans == sort_ans);

    cout << R_size << "," << S_size << "," << rep << ",";
    cout << long(timer.GetMean(0)) << "," << long(timer.GetMean(1)) << endl;
}