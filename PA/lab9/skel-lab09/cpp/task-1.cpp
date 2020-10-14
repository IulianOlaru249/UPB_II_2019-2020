#include <bits/stdc++.h>
using namespace std;

const int kNmax = 50005;
const int kInf = 0x3f3f3f3f;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n;
    int m;
    int source;
    vector<pair<int, int> > adj[kNmax];

    void read_input() {
        ifstream fin("in");
        fin >> n >> m >> source;
        for (int i = 1, x, y, w; i <= m; i++) {
            fin >> x >> y >> w;
            adj[x].push_back(make_pair(y, w));
        }
        fin.close();
    }

    vector<int> get_result() {
        /*
        TODO: Gasiti distantele minime de la nodul source la celelalte noduri
        folosind Dijkstra pe graful orientat cu n noduri, m arce stocat in adj.
            d[node] = costul minim / lungimea minima a unui drum de la source la nodul
        node;
            d[source] = 0;
            d[node] = -1, daca nu se poate ajunge de la source la node.

        Atentie:
        O muchie este tinuta ca o pereche (nod adiacent, cost muchie):
            adj[x][i].first = nodul adiacent lui x,
            adj[x][i].second = costul.
        */
       
        vector<bool> selected(n + 1, false);

        vector<int> d(n + 1, kInf);
        d[source] = 0;
        selected[source] = true;

        //folosim coada de prioritati pentru a avea mereu elementul cu distanta minima in varful cozii
        priority_queue <pair<int, int>> q;
        q.push(make_pair(0, source));

        while (!q.empty()) {
            int u = q.top().second;
            selected[u] = true;
            q.pop();
            
            //pentru fiecare nod adiacent
            for(auto node : adj[u]) {
                //nodul adiacent lui u
                int v = node.first;
                //costul muchiei u-v
                int w = node.second;

                //daca nodul imbunatateste drumul
                if ( !selected[v] && d[v] > d[u] + w) {
                    d[v] = d[u] + w;
                    q.push(make_pair(d[v], v));
                }
            }
        }

        //nodurile la care nu se poate ajunge
        for(int i = 1; i <= n; i++)
            if (!selected[i])
                d[i] = -1;

        return d;
    }

    void print_output(vector<int> result) {
        ofstream fout("out");
        for (int i = 1; i <= n; i++) {
            fout << result[i] << " ";
        }
        fout << "\n";
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
