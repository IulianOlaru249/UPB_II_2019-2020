#include <bits/stdc++.h>
using namespace std;

const int kNmax = 2e5 + 10;

// Strctura de date descrisa aici https://infoarena.ro/problema/disjoint.
class DisjointSet {
 private:
    vector<int> parent, size;

 public:
    // Se initializeaza n paduri
    DisjointSet(int n)
    : parent(n + 1),
    size(n + 1) {
        for (int i = 1; i <= n; ++i) {
            // fiecare padure contine un nod initial
            parent[i] = i;
            size[i] = 1;
        }
    }

    // returneaza radacina arborelui din care face parte node
    int find_root(int node) {
        if (node == parent[node]) {
            return node;
        }
        return parent[node] = find_root(parent[node]);
    }

    // reuneste arborii lui root1 si root2 intr-un singur arbore
    void merge_forests(int root1, int root2) {
        if (size[root1] <= size[root2]) {
            size[root2] += size[root1];
            parent[root1] = root2;
        } else {
            size[root1] += size[root2];
            parent[root2] = root1;
        }
    }
};

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n;
    int m;

    typedef tuple<int, int, int> edge;
    vector<edge> edges;

    void read_input() {
        ifstream fin("in");
        fin >> n >> m;
        for (int i = 1, x, y, w; i <= m; i++) {
            fin >> x >> y >> w;
            edges.push_back({x, y, w});
        }
        fin.close();
    }

    static bool my_compare(edge e1, edge e2)
    {
        tuple<int, int, int>& t1 = e1;
        tuple<int, int, int>& t2 = e2;

        return get<2>(t1) < get<2>(t2);
    }

    int get_result() {
        /*
        TODO: Calculati costul minim al unui arbore de acoperire
        folosind algoritmul lui Kruskal.

        Pentru a construi un tuple:
        int a, b, c;
        tuple<int, int, int> t = make_tuple(a, b, c);

        Pentru a accesa elementele dintr-un tuple, exista 2 variante:
        tuple<int, int, int> t;
        int a, b, c;
        tie(a, b, c) = t;

        tuple<int, int, int> t;
        int a = get<0>(t);
        int b = get<1>(t);
        int c = get<2>(t);

        Vi se da implementata structura de Paduri de multimi disjuncte.
        Utilizare:
        DisjointSet dj(n);
        dj.find(x);
        dj.reunion(x, y);
        */
       DisjointSet dj(n);
       int u, v, w, cost = 0;

        sort(edges.begin(), edges.end(), my_compare);

        for (auto  edge : edges) {
            int u = get<0> (edge);
            int v = get<1> (edge);
            int w = get<2> (edge);

            if(dj.find_root(u) != dj.find_root(v))
    		{
    			cost += w;
    			dj.merge_forests(dj.find_root(u), dj.find_root(v));
    		}
            
        }
        return cost;
    }

    void print_output(int result) {
        ofstream fout("out");
        fout << result << "\n";
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
