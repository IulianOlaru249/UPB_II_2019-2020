#include <bits/stdc++.h>
using namespace std;

const int kNmax = 100005;

class Task {
public:
    void solve() {
        read_input();
        print_output(get_result());
    }

private:
    struct Edge {
        int x;
        int y;

        Edge() {}
        Edge(int x, int y) : x(x), y(y) {}
    };

    int n;
    int m;
    vector<int> adj[kNmax];

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;
        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y;
            adj[x].push_back(y);
            adj[y].push_back(x);
        }
        fin.close();
    }

    vector<Edge> get_result() {
        /*
        TODO: Gasiti muchiile critice ale grafului neorientat stocat cu liste
        de adiacenta in adj.
        */

        vector<Edge> sol;
        return sol;
    }

    void print_output(const vector<Edge>& result) {
        ofstream fout("out");
        fout << result.size() << '\n';
        for (int i = 0; i < int(result.size()); i++) {
            fout << result[i].x << ' ' << result[i].y << '\n';
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
