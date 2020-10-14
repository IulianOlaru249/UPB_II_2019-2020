#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
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
        //for (int i = 1; i <= m; i++) {
        //    for (int j = 0; j < adj[i].size(); j++)
        //        cout << adj[i][j] << " ";
        //    cout << endl;
        //}
        vector<int> topsort;

        vector <int> in_degree(n+1, 0);
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < adj[i].size(); j++) {
                in_degree[adj[i][j]] ++;
            }
        }
        
        for (int i = 1; i <= n; i++)
            cout << in_degree[i] << " ";
        cout << endl;
        
        queue<int> q;
        for (int i = 1; i <= n; i++)
            if (in_degree[i] == 0)
                q.push(i);
        
        /* To check for cycles*/
        int count = 0;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            topsort.push_back(u);
            
            for (int i = 0; i < adj[u].size(); i++) {
                in_degree[adj[u][i]] --;
                if (in_degree[adj[u][i]] == 0)
                    q.push(adj[u][i]);
            }
            count ++;
        }
        
        return topsort;
    }

    void print_output(vector<int> result) {
        ofstream fout("out");
        for (int i = 0; i < int(result.size()); i++) {
            fout << result[i] << ' ';
        }
        cout << '\n';
        fout.close();
    }
};

int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
}
