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

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;
        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y;
            adj[x].push_back(y);
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
        return solve_dfs(); // DFS: O(n + m);
    }

	vector<int> solve_dfs() {
		// vectorul rezultat (in final contine o permutare pentru 1, 2, ..., n)
        vector<int> topsort;

        // used[i] = 1 daca i a fost deja vizitat, 0 altfel 
		vector<int> used(n + 1, 0);

        // pentru fiecare nod
		for (int i = 1; i <= n; ++i) {
            // daca nodul este nevizitat, pornim o parcurgere
			if (!used[i]) {
				dfs(i, used, topsort);
			}
		}

        // rezultatul a fost obtinut in ordine inversa
		reverse(topsort.begin(), topsort.end());

		return topsort;
	}

    // porneste o parcurgere DFS din node
    // foloseste vectorul used pentru a marca nodurile vizitate
	void dfs(int node, vector<int> &used, vector<int> &topsort) {
		used[node] = 1; // marcheze nodul ca fiind vizitat

        // parcurg vecinii
		for (auto &x : adj[node]) {
			if (!used[x]) {
				dfs(x, used, topsort);
			}
		}

        // dupa ce am vizitat recursiv toti vecinii, adaugam
        // nodul la sortare
		topsort.push_back(node);
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
