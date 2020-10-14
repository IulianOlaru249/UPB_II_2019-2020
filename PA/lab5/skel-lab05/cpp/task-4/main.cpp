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
    string caractere;
    vector<int> freq;

    void read_input() {
        ifstream fin("in");
        fin >> n >> k;
        fin >> caractere;
        caractere = " " + caractere; // Adaugare element fictiv -
                                     // indexare de la 1.
        freq.push_back(-1); // Adaugare element fictiv - indexare de la 1.
        for (int i = 1, f; i <= n; i++) {
            fin >> f;
            freq.push_back(f);
        }
        fin.close();
    }

    vector<vector<char> > get_result() {
        vector<vector<char> > all;

        /*
        TODO: Construiti toate sirurile cu caracterele in stringul caractere
        (indexat de la 1 la n), si frecventele in vectorul freq (indexat de la
        1 la n), stiind ca nu pot fi mai mult de K aparitii consecutive ale
        aceluiasi caracter.

        Pentru a adauga un nou sir:
            vector<char> sir;
            all.push_back(sir);
        */

        return all;
    }

    void print_output(vector<vector<char> > result) {
        ofstream fout("out");
        fout << result.size() << '\n';
        for (int i = 0; i < (int)result.size(); i++) {
            for (int j = 0; j < (int)result[i].size(); j++) {
                fout << result[i][j];
            }
            fout << '\n';
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
