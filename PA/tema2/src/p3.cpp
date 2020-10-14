#include <bits/stdc++.h>
#include <limits>

using namespace std;
/* Used to print with maximum double precision. */
typedef std::numeric_limits<double> dbl;

const int kNmax = 100000;
const int kInf = 0x3f3f3f3f;

int n;
int m;
/* Total energy. */
double e;
vector<pair<int, int>> adj[kNmax];
vector<pair<int, int>> parent(kNmax);

void read_input() {
    ifstream fin("p3.in");
    fin >> n >> m >> e;
    for (int i = 1, x, y, w; i <= m; i++) {
        fin >> x >> y >> w;
        adj[x].push_back(make_pair(y, w));
    }
    fin.close();
}

/* This solution uses dijkstra's algorithm because the shortest path (the oane
 * that saves most energy) is required. Instead of judging distance by the 
 * weight of the edge, we will decide if a path is better if it saves more
 * energy. This takes O(m log n)
 */
void dijkstra(int source, vector<double> &d) {
    /* A set is used because it will help to always peek the best edge. */
    set<pair<double, int>> pq;

    /* Keep track of visited nodes. */
    vector<bool> vizited(n + 1, false);

    /* No energi is consumed to get to the source. */
    pq.insert({0, source});

    /* Robin hai maximum energy in the source node. */
    d[source] = e;

    while (!pq.empty()) {
        auto top = pq.begin();
        int u = top->second;
        pq.erase(top);
        vizited[u] = true;

        /* For every neighbour of the node. */
        for (const auto &node : adj[u]) {
            int v = node.first;
            int w = node.second;

            /* If robin has more energy left going trough this town. */
            if (!vizited[v] && d[v] < d[u] * (1 - (double)w / 100)) {
                /* Keep track of parent to reconstruct the path. */
                parent[v].first = u;
                parent[v].second = w;

                /* Update energy of edge. */
                d[v] = d[u] * (1 - (double)w / 100);

                /* Update set. The edges will be orderd by the percentage of 
                 * total energy they take.
                 */
                pq.erase({d[v], v});
                pq.insert({(1 - (double)d[v] / e) * 100, v});
            }
        }
    }
}

void get_result() {
    /* The distance vector represents how much energy Robin has in a node.
     * It is initialised with the worst case scenario.
     */
    vector<double> d(n + 1, -kInf);
    /* The source node is node 1. */
    dijkstra(1, d);
}

/* Reconstruct the path based on the parent vector. This takes O(n). */
ofstream fout("p3.out");
void print_path(int u, double energy) {
    /* If the source node is reached, print the remaining energy. */
    if (u == 1) {
        fout.precision(dbl::max_digits10);
        fout << energy << endl
             << u << " ";
        return;
    }

    /* Go to the parent of the current node and update how much 
     * energy it took.
     */
    print_path(parent[u].first,
        energy *= (1 - (double)parent[u].second / 100));

    /* When back from recursion, print the node so the path is 
     * not in reverse.
     */
    fout << u << " ";
}

int main(void) {
    read_input();
    get_result();
    /* The destination node is n and the initial energy is e. */
    print_path(n, e);
    fout.close();
    return 0;
}
