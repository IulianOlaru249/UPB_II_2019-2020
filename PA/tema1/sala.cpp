#include <bits/stdc++.h>
#include <vector>

using namespace std;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    const int MOD = 1000000007;

    int n;
    int m;
    vector<pair<int, int>> weights;

    static bool compare_weight(const pair<int, int> &a,
            const pair<int, int> &b) {
        if (a.first != b.first)
            return (a.first > b.first);
        else
            return (a.second < b.second);
    }

    void read_input() {
        ifstream fin("sala.in");
        fin >> n;
        fin >> m;
        int x, y;
        for (int i=0; i < n; i++) {
            fin >> x;
            fin >> y;
            weights.push_back(make_pair(x, y));
        }
        fin.close();
    }

    int get_result() {
        /* Sort in descending order of weights to get heavyer 
         * weights out of the way
         */
        sort(weights.begin(), weights.end(), compare_weight);

        int total_reps = 0;
        int max_gain = 0;
        int curr_gain = 0;

        /* Count the maximum gain achieved with each weight. This is done
         * because there can be picked less than M weights.
         */
        for (int i = 0; i < m; i++) {
            total_reps = (total_reps % MOD + weights[i].second % MOD) % MOD;
            curr_gain = (1LL *total_reps * weights[i].first) % MOD;
            if (curr_gain >= max_gain)
                max_gain = curr_gain;
        }

        /* Remove the last (least heavy) weight from the set and replace it
         * with the others to look for better gain.
         */
        for (int i = m; i < n; i++) {
            total_reps -= weights[i - 1].second;
            total_reps = (total_reps % MOD +  weights[i].second % MOD) % MOD;
            curr_gain = (1LL * total_reps * weights[i].first) % MOD;

            if (curr_gain >= max_gain)
                max_gain = curr_gain;
        }

        return max_gain;
    }

    void print_output(int result) {
        ofstream fout("sala.out");
        fout << result;
        fout.close();
    }
};

int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
}
