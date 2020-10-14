#include <bits/stdc++.h>
using namespace std;

const int kNmax = 100005;

class Task {
public:
	void solve() {
		read_input();
		print_output();
	}

private:
	int n;
	int m;
	int low[kNmax] = {0};
	int children;
	/* will help find distance */
	int time = 0;
	vector<int> sol;
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


	void DFS(int *d, int *parent, vector<int> *color, int u) {
		(*color)[u] = -1;
		low[u] = ++time;
		d[u] = ++time;
		int size = adj[u].size();
		for (int i = 0; i < size; i++) {
			if ((*color)[adj[u][i]] == 1) {
				children ++;
				parent[u] = adj[u][i];

				DFS(d, parent, color, adj[u][i]);

				if (low[u] > low[adj[u][i]])
					low[u] = low[adj[u][i]];

				if (parent[u] == -1 && children > 1)
					sol.push_back(u);
				if (parent[u] != -1 && low[adj[u][i]] > d[adj[u][i]])
					sol.push_back(u);
			}
			else if (adj[u][i] != parent[u]){
				if (low[u] > d[adj[u][i]])
					low[u] = d[adj[u][i]];
			}
		}
	}
	void get_result() {
		/*
		TODO: Gasiti nodurile critice ale grafului neorientat stocat cu liste
		de adiacenta in adj.
		*/

		/* distance to current node <=> how long it took to reach it */
		int d[kNmax];
		/* parents of nodes */
		int parent[kNmax];
		for (int i = 0; i < kNmax; i++)
			parent[i] = -1;
		/*1 for white; -1 for black */
		vector <int> color (n + 1, 1);
		for (int i = 1; i <= n; i++) {
			if (color[i] == 1) {
				DFS(d, parent, &color, i);
			}
		}
	}

	void print_output() {
		ofstream fout("out");
		for (int i = 0; i < int(sol.size()); i++) {
			fout << sol[i] << ' ';
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
