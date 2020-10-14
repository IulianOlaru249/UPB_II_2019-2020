#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int kNmax = 50005;
const int kInf = 0x3f3f3f3f;

int n, m;
vector<int> adj[kNmax];

bool hasCycleHelper( vector<bool> visited, vector<bool> recStack, int u)
{
    visited[u] = true;
    recStack[u] = true;
    for (int v : adj[u])
        if (visited[v] == false && hasCycleHelper(visited, recStack, v))
            return true;
        else if (recStack[v])
            return true;
    recStack[u] = false;
    return false;
}

bool hasCycle()
{
    vector<bool> visited(n +1, false);
    vector<bool> recStack(n + 1, false);

    for (int u = 1; u <= n; u++)
        if (visited[u] == false)
            if (hasCycleHelper(visited, recStack, u))
                return true;

    return false;
}

int main()
{
    ifstream fin;
    fin.open("./gcycle.in");
    fin >> n >> m;
    for (int i = 1, x, y; i <= m; i++) {
			fin >> x >> y;
			adj[x].push_back(y);
	}
    fin.close();

    for (int i = 1; i <= n; i++) {
        cout << i << ": ";
       for (int j = 0; j < adj[i].size(); j++) {
            cout << adj[i][j] << " ";   
       }
       cout << endl;
    }

    if (hasCycle()) {
        cout << "Has cycle!\n";
    }

}