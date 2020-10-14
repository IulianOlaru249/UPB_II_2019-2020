#include <bits/stdc++.h>
using namespace std;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n;

    void read_input() {
        ifstream fin("in");
        fin >> n;
        fin.close();
    }

    vector<int> get_result() {
        vector<int> sol(n + 1, 0);

        /*
        TODO: Gasiti o solutie pentru problema damelor pe o tabla de dimensiune
        n x n.

        Pentru a plasa o dama pe randul i, coloana j:
            sol[i] = j.
        Randurile si coloanele se indexeaza de la 1 la n.

        De exemplu, configuratia (n = 5)
        X----
        --X--
        ----X
        -X---
        ---X-
        se va reprezenta prin sol[1..5] = {1, 3, 5, 2, 4}.
        */

        return sol;
    }

    void print_output(vector<int> result) {
        ofstream fout("out");
        for (int i = 1; i <= n; i++) {
            fout << result[i] << (i != n ? ' ' : '\n');
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
