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
    int n;
    vector<pair<int, int>> fences;

    void read_input() {
        ifstream fin("gard.in");
        fin >> n;
        int x1, x2;
        for (int i=0; i < n; i++) {
            fin >> x1;
            fin >> x2;
            fences.push_back(make_pair(x1, x2));
        }
        fin.close();
    }

    /* The fences will be sorted in decreasing order of the starting position
     * so we will not have to worry about a fence that starts before the current
     * one and makes it redundant. Is the starting x is equal for two fences, 
     * there are sorted by the finish x, in ascending order, so longer fences
     * come first.
     */
    static bool compare(const pair<int, int> &a,
              const pair<int, int> &b) {
        if (a.first != b.first)
            return (a.first < b.first);
        else
            return (a.second > b.second);
    }

    /* This solution takes O(n * log n + n) time and O(1) space. */
    int get_result() {
        sort(fences.begin(), fences.end(), compare);
        int count = 0;
        /* Keep track of the last dence placed, as term of comparison */
        pair<int, int> last_fence = fences[0];
        for (int i = 1; i < fences.size(); i++) {
            /* If the current fence piece increases the dimension of the overall 
             * fence then it is not redundant.
             */
            if (fences[i].second <= last_fence.second)
                count ++;
            else
                last_fence = fences[i];
        }

        return count;
    }

    void print_output(int result) {
        ofstream fout("gard.out");
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
