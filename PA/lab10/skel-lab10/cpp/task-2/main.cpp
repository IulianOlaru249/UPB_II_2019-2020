#include <bits/stdc++.h>
using namespace std;

const int kNmax = 2e5 + 10;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n;
    int m;
    vector<pair<int, int> > adj[kNmax];

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;
        for (int i = 1, x, y, w; i <= m; i++) {
            fin >> x >> y >> w;
            adj[x].push_back({y, w});
            adj[y].push_back({x, w});
        }
        fin.close();
    }

    struct my_compare {
        bool operator()(pair<int, int> const& p1, pair<int, int> const& p2) {
            return p1.second > p2.second;
        }
    };


    int get_result() {
        /*
        TODO: Calculati costul minim al unui arbore de acoperire
        folosind algoritmul lui Prim.
        */
        vector<int> d(n + 1, kNmax);
        vector<int> parents(n + 1, -1);
        vector<int> visited(n + 1, false);

        int cost = 0;
        d[1] = 0;
        visited[1] = true;

        priority_queue<pair<int, int>, vector<pair<int, int>>, my_compare> q;
        q.push(make_pair(1, 0));

        while(!q.empty()) {
            pair<int, int> u = q.top();
            q.pop();
            visited[u.first] = true;

            for(pair<int, int> edge : adj[u.first]) {
                /* Verifica si daca a fost deja inclus in AMA. */
                if(edge.second < d[edge.first] && !visited[edge.first]) {
                    d[edge.first] = edge.second;
                    parents[edge.first] = u.first;
                    q.push(make_pair(edge.first, d[edge.first]));
                }

            }
        }

        for(int i = 2; i <= n; i++) {
            cost += d[i];
        }

        return cost;
    }

    void print_output(int result) {
        ofstream fout("out");
        fout << result << "\n";
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
