#include <bits/stdc++.h>
using namespace std;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n, k;

    void read_input() {
        ifstream fin("in");
        fin >> n >> k;
        fin.close();
    }

    vector<vector<int> > get_result() {
        vector<vector<int> > all;

        /*
        TODO: Construiti toate aranjamentele de N luate cate K ale
        multimii {1, ..., N}.

        Pentru a adauga un nou aranjament:
            vector<int> aranjament;
            all.push_back(aranjament);
        */

        return all;
    }

    void print_output(vector<vector<int> > result) {
        ofstream fout("out");
        fout << result.size() << '\n';
        for (int i = 0; i < (int)result.size(); i++) {
            for (int j = 0; j < (int)result[i].size(); j++) {
                fout << result[i][j] <<
                    (j + 1 != result[i].size() ? ' ' : '\n');
            }
        }
        fout.close();
    }
};

// Please always keep this simple main function!
int main() {
    // Allocate a Task object on heap in order to be able to
    // declare huge static-allocated data structures inside the class.
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
}
