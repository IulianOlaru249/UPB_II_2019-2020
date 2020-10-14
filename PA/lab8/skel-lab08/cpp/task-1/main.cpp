#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

const int maxN = 100001;
const int kNmax = 100005;

class Task {
 public:
	void solve() {
		read_input();
		get_result();
		print_output();
	}

 private:
	int n, sol_index;
	int m;
	vector<int> adj[kNmax];
	vector<int> adjt[kNmax];
	vector<int> sol[kNmax];
	vector<int> dfs_res;

	void read_input() {
		ifstream fin("in");
		fin >> n >> m;
		for (int i = 1, x, y; i <= m; i++) {
			fin >> x >> y;
			adj[x].push_back(y);
			adjt[y].push_back(x);
		}
		fin.close();
	}

	void DFS(int pass, vector <int> *color, int u) {
		/* 	On the first pass of DFS nodes need to be taken from the original tree and
		 * stored in a stack so that on the second pass we can iterate trough the nodes 
		 * in the stack, and add them to te solution.
		 */
		/* 	On the first pass node should me marken as visited while in the second one
		 * node will be marked as unvisited and stored in the result before going into
		 * recursion.
		 */
		if (pass == 1) {
			(*color)[u] = -1;
		}
		else {
			(*color)[u] = 1;
			sol[sol_index].push_back(u);
		}
		int size;
		if (pass == 1)
			size = adj[u].size();
		else 
			size =  adjt[u].size();
		/*	Go trough nodes from the original tree in the first pass and in the second 
		 * pass trough node from transposed tree.
		 */
		for (int i = 0; i < size; i++) {
			/* Check if node visited for the first pass and unvisited for the second. */
			if (pass == 1) {
				if ((*color)[adj[u][i]] == 1)
					DFS(pass, color, adj[u][i]);
			}
			else{
				if ((*color)[adjt[u][i]] == -1)
					DFS(pass, color, adjt[u][i]);
			}
		}
		/* If on the fitst pass the node shoult be put on the stack when back from recustion. */
		if (pass == 1)
			dfs_res.push_back(u);
	}

	void get_result() {
		sol_index = 0;
		/*1 for white, -1 for black */
		vector <int> color (n + 1, 1);
		/* Aplly DFS once to get the stack of nodes */
		for (int i = 1; i <= n; i++) {
			if (color[i] == 1) {
				DFS(1, &color, i);
			}
		}

		/* Go trough each node from the stack dfs returned */
		for (int i = dfs_res.size() - 1; i >= 0; i--) {
			if (color[dfs_res[i]] == -1) {
				sol_index++;
				DFS(2, &color, dfs_res[i]);
			}
		}
	}

	void print_output() {
		ofstream fout("out");
		fout << sol_index << '\n';
		for (int i = sol_index; i >= 0; i--) {
			for(int j = 0; j < sol[i].size(); j++)
				fout << sol[i][j] << " ";
			fout << "\n";
		}
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}
