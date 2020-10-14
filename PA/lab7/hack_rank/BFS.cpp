#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

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
	int source;
	vector<int> adj[kNmax];

	void read_input() {
		cin >> n >> m >> source;
		for (int i = 1, x, y; i <= m; i++) {
			cin >> x >> y;
			adj[x].push_back(y);
			adj[y].push_back(x);
		}
	}

	vector<int> get_result() {
        /* Initialize vector with -1 so nodes we can't reach will 
         * default to this
         */
		vector<int> d(n + 1, -1);
        /* 1 for white; 0 for gray; -1 for black */
        vector<int> color(n + 1, 1);
        queue<int> q;
        /* Distance to source is 0 */
        d[source] = 0;
        color[source] = 0;
        q.push(source);
        
        while (!q.empty()) {
            int u = q.front();
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                //cout << v << " ";
                /* if is white node */
                if (color[v] == 1) {
                    d[v] = d[u] + 1;
                    color[v] = 0;
                    q.push(v);
                }
            }
            //cout << endl;
            color[u] = -1;
            q.pop();
        }
        
        return d;
	}

	void print_output(vector<int> result) {
		for (int i = 1; i <= n; i++) {
			cout << result[i] << (i == n ? '\n' : ' ');
		}
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}
