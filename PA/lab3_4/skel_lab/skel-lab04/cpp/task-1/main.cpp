#include <bits/stdc++.h>
using namespace std;

const int kMod = 1e9 + 7;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n;
    vector<int> v;

    void read_input() {
        ifstream fin("in");
        fin >> n;
        v.push_back(-1); // adaugare element fictiv - indexare de la 1
        for (int i = 1, e; i <= n; i++) {
            fin >> e;
            v.push_back(e);
        }
        fin.close();
    }

    int get_result() {
        /*
        Calculati numarul de subsiruri ale lui v cu suma numerelor para si
        returnati restul impartirii numarului la 10^9 + 7.
        */
        return 0;
    }

    void print_output(int result) {
        ofstream fout("out");
        fout << result;
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
