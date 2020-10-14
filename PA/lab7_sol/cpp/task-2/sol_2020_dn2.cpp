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
    int n;
    int m;
    vector<int> adj[kNmax];
    vector<int> in_degree;

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;

        // grad intern 0 pentru toate nodurile... momentan
        in_degree = vector<int> (n + 1, 0);

        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y;
            adj[x].push_back(y); // muchia x -> y
            ++in_degree[y];      // inca o muchie care intra in y
        }
        fin.close();
    }

    vector<int> get_result() {
        /*
        TODO: Faceti sortarea topologica a grafului stocat cu liste de
        adiacenta in adj.
        *******
        ATENTIE: nodurile sunt indexate de la 1 la n.
        *******
        */
        return solve_bfs(); // BFS: O(n + m);
    }

    vector<int> solve_bfs() {
        // Step 0: declaram o coada in care putem baga noduri
        queue<int> Q;

        // Step 1: initializare topsort - permutare vida initial
        vector<int> topsort;

        // Step 2: pasul initial: pun in coada TOATE nodurile cu grad intern 0
        for (int i = 1; i <= n; ++i) {
            if (in_degree[i] == 0) {
                Q.push(i);
            }
        }

        // Step 3: parcurg in latime graful
        while (!Q.empty()) {
            // 3.1: SCOT primul nod din coada
            int node = Q.front();
            Q.pop();

            // adaug la solutie elementul scos
            topsort.push_back(node);

            // 3.2 Ii parcurg toti vecinii
            for (auto &it : adj[node]) {
                // sterg muchia node->it
                // obs1. NU e nevoie sa o sterg la propriu
                //       Daca nu am cicluri, nu se va ajunge aici
                // obs2. Simulez stergerea prin scaderea gradului intern a lui it
                --in_degree[it];

                // daca it a ajuns nod cu grad intern 0, atunci este adaugat in coada
                if (in_degree[it] == 0) {
                    Q.push(it);
                }
            }
        }

        // STEP 4: verifica ca topsort chiar reprezinta o sortare topologica valida
        // Ce inseamna asta? S-au sters toate muchiile din graf.
        // Daca nu s-a sters tot, atunci graful este ciclic!
        bool is_valid = true;
        for (int i = 1; i <= n; ++i) {
            if (in_degree[i] > 0) {
                is_valid = false;
                break;
            }
        }

        if (is_valid) {
            return topsort;  // sortarea topologica obtinuta
        } else { 
            return {};       // vector gol == nu se poate face o sortare topologica
        }
    }

    void print_output(vector<int> result) {
        ofstream fout("out");
        for (int i = 0; i < int(result.size()); i++) {
            fout << result[i] << ' ';
        }
        fout << '\n';
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
