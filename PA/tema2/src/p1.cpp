#include <bits/stdc++.h>
using namespace std;

const int kNmax = 100000;
const int kInf = 0x3f3f3f3f;

int n;
int m;
int k;
int orase_start[kNmax];
int perm[kNmax];
vector<int> adj[kNmax];

void read_input() {
    ifstream fin("p1.in");
    fin >> n >> m >> k;
    for (int i = 0, oras; i < k; i++) {
        fin >> oras;
        orase_start[i] = oras;
    }

    for (int i = 0, oras; i < n - 1; i++) {
        fin >> oras;
        perm[i] = oras;
    }

    for (int i = 1, x, y; i <= m; i++) {
        fin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    fin.close();
}

/* BFS is used to check if we can reach any of the nodes that the lords are in
 * from the source node. 
 */
bool bfs(vector<bool>blocked) {
    vector<int> d(n + 1, kInf);
    queue<int> Q;
    Q.push(1);
    d[1] = 0;;

    /* As long as there are edges left to visit. */
    while (!Q.empty()) {
        int node = Q.front();
        Q.pop();

        /* For each neighbour. */
        for (auto &x : adj[node]) {
            /* Update the distance if there is a better path
             * and is is not blocked. */
            if (d[node] + 1 < d[x] && !blocked[x]) {
                d[x] = d[node] + 1;
                Q.push(x);
            }
        }
    }

    /* The result is not good if any lord city can be reached 
     * from the source city.
     */
    for (int oras : orase_start) {
        if (d[oras] != kInf)
            return false;
    }

    return true;
}

/* This solution uses something resembling a binary search. First, block the 
 * first half of the cities and check if Robin is safe. If he is not safe, 
 * try block 3/4 of the cities and so on. If he is safe, look for a better
 * solution by trying to block only 1/2 of cities and so on.
 * It takes O(log n * (2n + m)) time.
 */
void bin_search(int l, int r, int *min) {
    /* Keep track of blocked cities. */
    vector<bool> blocked(n + 1);
    /* As long as we ca look for a better solution.
     * This will take O(log n) time.
     */
    while (l <= r) {
        /* Unblock all cities. 
         * This will take O(n) time.
         */
        fill(blocked.begin(), blocked.end(), false);

        /* Thy block only half of them. */
        int m = (l + r) / 2;
        for (int i = 0; i <= m; i++) {
            blocked[perm[i]] = true;
        }

        /* If the solution is valid, look for a better one.
        * If not, keep looking for a valid one.
        * This will take O(n + m) time.
        */
        if (bfs(blocked)) {
            /* Keep thack of best solution so far. */
            *min =  m + 1;
            /* Try block less cities. */
            r = m - 1;
        } else {
            /* try block more cities. */
            l = m + 1;
        }
    }
}

void get_result() {
    vector<int> d(n + 1, kInf);
    int min = -1;
    bin_search(0, n - 1, &min);

    ofstream fout("p1.out");
    fout << min << endl;
    fout.close();
}

int main(void) {
    read_input();
    get_result();
    return 0;
}
