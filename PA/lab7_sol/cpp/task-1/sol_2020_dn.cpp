#include <bits/stdc++.h>
using namespace std;

const int kNmax = 100005;
const int kInf  = (1 << 30);

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
    vector<int> adj[kNmax];

    void read_input() {
        ifstream fin("in");
        fin >> n >> m >> source;
        for (int i = 1, x, y; i <= m; i++) {
            fin >> x >> y;
            adj[x].push_back(y);
            adj[y].push_back(x);
        }
        fin.close();
    }

    vector<int> get_result() {
        /*
        TODO: Faceti un BFS care sa construiasca in d valorile d[i] = numarul
        minim de muchii de parcurs de la nodul source la nodul i.
        d[source] = 0
        d[x] = -1 daca nu exista drum de la source la x.
        *******
        ATENTIE: nodurile sunt indexate de la 1 la n.
        *******
        */
        vector<int> d(n + 1, kInf);

        // initializat coada cu nodul sursa
        queue<int> Q;
		Q.push(source);
		d[source] = 0;

        // cat timp nu am parcurs toate nodurile
		while (!Q.empty()) {
            // extragem si procesam urmatorul nod din coada
			int node = Q.front();
			Q.pop();

            // parcurgem vecinii lui node
			for (auto &x : adj[node]) {
                // actulizam distanta lui x fata de sursa daca
                // am gasit o solutie mai buna
				if (d[node] + 1 < d[x]) {
					d[x] = d[node] + 1;
					Q.push(x);
				} 
			}
		}

        // problema ne cere ca acolo unde nu exista drum sa punem -1
		for (auto &x : d) {
			if (x == kInf) {
				x = -1;
			}
		}
		
		return d;
    }

    void print_output(vector<int> result) {
        ofstream fout("out");
        for (int i = 1; i <= n; i++) {
            fout << result[i] << (i == n ? '\n' : ' ');
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
