#include <algorithm>
#include <bits/stdc++.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <queue>
#include <vector>
using namespace std;

const int kNmax = 1005;
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
    vector<int> adj[kNmax];
    int C[kNmax][kNmax];

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;

        memset(C, 0, sizeof C);
        for (int i = 1, x, y, z; i <= m; i++) {
            fin >> x >> y >> z;
            adj[x].push_back(y);
            adj[y].push_back(x);
            C[x][y] += z;
        }
        fin.close();
    }


    int bfs(int source, int end, vector<int> &visited, vector<int> &parent) {
        queue<int> q;
        int current;

        fill(visited.begin(), visited.end(), 0);
        q.push(source);
        visited[source] = 1;

        while (!q.empty()) {
            current = q.front();
            q.pop();

            if (current == end) {
                break;
            }

			for (int next : adj[current]) {
                if (visited[next] == 0 && C[current][next] > 0) {
                    visited[next] = 1;
                    parent[next] = current;
                    q.push(next);
                }
            }
        }
        return visited[end];
    }

    int max_flow(int source, int end) {

        vector<int> parent(n + 1, 0);
        vector<int> visited(n + 1, 0);
        int maxflow = 0, flow = kInf, current = 0;

        while (bfs(source, end, visited, parent)) {

            for (int current : adj[end]) {
                if (visited[current]) {
					flow = kInf;
					parent[end] = current;

					current = end;
					while (current != source) {
						int prev = parent[current];
						flow = min(flow, C[prev][current]);
						current = prev;
					}

					if (flow > 0) {
						current = end;
						while (current != source) {
							int prev = parent[current];
							C[prev][current] -= flow;
							C[current][prev] += flow;
							current = prev;
						}
						maxflow += flow;
					}
				}
			}
        }

        return maxflow;
    }

    int get_result() {
        /*
		TODO: Calculati fluxul maxim pe graful orientat dat.
		Sursa este nodul 1.
		Destinatia este nodul n.

		In adj este stocat graful neorientat obtinut dupa ce se elimina orientarea
		arcelor, iar in C sunt stocate capacitatile arcelor.
		De exemplu, un arc (x, y) de capacitate z va fi tinut astfel:
		C[x][y] = z, adj[x] contine y, adj[y] contine x.
		*/

		int flow = 0;
		flow = max_flow(1, n);
        return flow;
    }

    void print_output(int result) {
        ofstream fout("out");
        fout << result << '\n';
        fout.close();
    }
};

int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
}