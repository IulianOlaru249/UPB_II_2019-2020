#include <bits/stdc++.h>
using namespace std;

// ATENTIE! In aceasta sursa sunt prezentate 4 metode corecte de a folosi std::sort.
// Alegeti una!

struct Object {
    int weight;
    int price;

    Object(int _weight, int _price) : weight(_weight), price(_price) {}

    // Metoda 1: comparare cu operator<
    bool operator<(const Object &a) const {
        return 1.0 * this->weight / this->price < 1.0 * a.weight / a.price;
    }
};


// Metoda 2: functie libera
bool cmp(Object &a, Object &b) {
    return 1.0 * a.weight / a.price < 1.0 * b.weight / b.price;
}

// Metoda 3: comparator
struct Comparator {
    bool operator()(const Object &a, const Object &b) const {
        return 1.0 * a.weight / a.price < 1.0 * b.weight / b.price;
    }
};


class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n, w;
    vector<Object> objs;

    void read_input() {
        ifstream fin("in");
        fin >> n >> w;
        for (int i = 0, weight, price; i < n; i++) {
            fin >> weight >> price;
            objs.push_back(Object(weight, price));
        }
        fin.close();
    }


    double get_result() {
        /*
        TODO: Aflati profitul maxim care se poate obtine cu obiectele date.
        */
        // sort(objs.begin(), objs.end());                  // Metoda 1
        // sort(objs.begin(), objs.end(), cmp);				// Metoda 2
        // sort(objs.begin(), objs.end(), Comparator());	// Metoda 3

        return knapsack_greedy();
    }

    // T = O(n log n) - sortare
    // S = O(1)       - nu am inclus inputul
    double knapsack_greedy() {
        // Sortare crescatoare greutate/pret
        // Metoda 4: lambda function
        sort(objs.begin(), objs.end(), [](const Object &a, const Object &b) {
            return 1.0 * a.weight / a.price < 1.0 * b.weight / b.price;
        });

        double sol = 0.0;

        // parcurg lista de obiecte
        for (auto &o : objs) {
            if (w == 0.0) {
                break; // ghiozdan plin
            }

            // in rucsac pot pune ori tot obiectul, ori o bucata din el (cat mai are loc)
            double weight = std::min<double>(w, o.weight);
            w -= weight;
            sol += 1.0 * weight / o.weight * o.price;
        }

        return sol;
    }

    void print_output(double result) {
        ofstream fout("out");
        fout << setprecision(4) << fixed << result;
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
