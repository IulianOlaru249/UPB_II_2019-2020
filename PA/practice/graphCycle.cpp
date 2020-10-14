#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

const int kNmax = 50005;
const int kInf = 0x3f3f3f3f;

int n, m;
vector<int> adj[kNmax];
map <int, int> links;
int cycleNode = -1;

bool dfs (int prev, int u, set<int> *whiteSet, set<int> *graySet, set<int> *blackSet)
{
    /* Muta nodul in multimea nodurilor in curs de explorare. */
    whiteSet->erase(u);
    graySet->insert(u);

    /* Tine minte de la ce nod a pornit explorarea lui v. */
    links.insert(make_pair(u, prev));
    
    /* Pentru fiecare nod vecin */
    for (int v : adj[u]) {
        /* Daca vecinul a fost explorat complet. */
        if (blackSet->find(v) != blackSet->end())
            continue;

        /* Daca vecinul este deja in curs de explorare, atunci graful are ciclu. */
        if (graySet->find(v) != graySet->end()) {
            cycleNode = u;
            return true;
        }

        /* Exploreaza vecinul. */
        if (dfs (u, v, whiteSet, graySet, blackSet))
            return true;
    }

    /* Muta nodul in multimea nodurilor explorte. */
    graySet->erase(u);
    blackSet->insert(u);
    return false;
}

bool hasCycle()
{
    set<int> whiteSet;
    set<int> graySet;
    set<int> blackSet;

    /* Initial toate nodurile sunt nevizitate. */
    for (int i = 1; i <= n; i++)
        whiteSet.insert(i);

    /* Atata timp cat sunt noduri neexplorate */
    while (whiteSet.size() > 0) {
        int u = *whiteSet.begin();
        if (dfs (-1, u, &whiteSet, &graySet, &blackSet)) {
            return true;
        }
    }
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

        //cout << cycleNode;
        while (cycleNode != -1) {
            cout << cycleNode << " ";
            cycleNode = (links.find(cycleNode))->second;
        }
        cout << endl;
    }

    // map<int, int>::iterator itr; 
    // for (itr = links.begin(); itr != links.end(); ++itr) { 
    //    cout << '\t' << itr->first 
    //         << '\t' << itr->second << '\n'; 
    // } 
    // cout << endl; 
}