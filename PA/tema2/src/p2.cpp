#include <bits/stdc++.h>
using namespace std;

const int kNmax = 100000;
const int kInf = 0x3f3f3f3f;

int n;
int m;
int source;
int destination;
vector<pair<int, int>> adj[kNmax];

void read_input() {
    ifstream fin("p2.in");
    fin >> n >> m >> source >> destination;
    for (int i = 1, x, y, w; i <= m; i++) {
        fin >> x >> y >> w;
        adj[x].push_back(make_pair(y, w));
    }
    fin.close();
}

/* This solution uses an adaptation of the bellman-ford algorithm because 
 * the shortest path (the oane that saves most energy) is required and
 * there can be edges with negative cost. This version of the algorithm 
 * does only one pass because it will work on a graph sorted in topological
 * order.
 */
bool one_pass_bellman(std::vector<int>& d, vector<int> toposort) {
    d[source] = 0;
    /* For each node in topological order. This takes O(n * m). */
    for (int u : toposort) {
        /* For each neighbour. */
        for (const auto& node : adj[u]) {
            int v = node.first;
            int w = node.second;

            /* If it is a better choice, update distance. */
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
            }
        }
    }

    return true;
}

/* DFS function used to generate the topological order. This is possible 
 * because there can not be a cycle in the graph.
 */
void dfs(int node, vector<int> &used, vector<int> &topsort) {
    used[node] = 1;
    /*For each neighbour. */
    for (auto &v : adj[node]) {
        /* Don't explore the same node twice. */
        if (!used[v.first]) {
            dfs(v.first, used, topsort);
        }
    }

    /* After all the neighbours have been visited (the node is fully explored)
     * the node will be added in the topological order.
     */
    topsort.push_back(node);
}

/* This takes O(n + m) time. */
vector<int> solve_dfs() {
    /* Keep track of the nodes in order. */
    vector<int> topsort;

    vector<int> used(n + 1, 0);

    /* For each node. */
    for (int i = 1; i <= n; ++i) {
        /* Explore the node if it is not visited. */
        if (!used[i]) {
            dfs(i, used, topsort);
        }
    }

    /* Reverse the result so it is in the correct order. */
    reverse(topsort.begin(), topsort.end());

    return topsort;
}

vector<int> get_result() {
    /* Initialize distances with the worst possible case. */
    vector<int> d(n + 1, kInf);
    one_pass_bellman(d, solve_dfs());
    return d;
}

void print_output(vector<int> result) {
    ofstream fout("p2.out");
    /* Only the distance from source to destination matters. */
    fout << result[destination];
    fout.close();
}

int main(void) {
    read_input();
    print_output(get_result());
    return 0;
}
