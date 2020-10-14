#include <bits/stdc++.h>
using namespace std;

class Task {
public:
    void solve() {
        read_input();
        print_output(compute_sqrt());
    }

private:
    double n;

    void read_input() {
        ifstream fin("in");
        fin >> n;
        fin.close();
    }

    double compute_sqrt() {
        /*
        TODO: Calculati sqrt(n) cu o precizie de 0.001
        Precizie de 10^-x = |rezultatul vostru - rezultatul corect| <= 10^-x
        */
        const double eps = 1e-3;

        int steps = 30; // limitez prin numarare de pasi

        double left = 0.0;
        double right = (n < 1 ? 1.0 : n);

        while (steps--) {
            double middle = (left + right) * 0.5;
            double diff = n - middle * middle;

            if (fabs(diff) <= eps) return middle;

            if (diff > 0) {
                left = middle;
            } else {
                right = middle;
            }
        }

        return 0.0;
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
