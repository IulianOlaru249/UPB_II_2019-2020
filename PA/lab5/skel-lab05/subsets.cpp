#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
using namespace std;

bool check(vector<int> &solution) {
    return true;
}

void print_output(vector<vector<int> > result) {
    cout << result.size();
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }
}

 
void printSolution(vector<int> &solution) {
    for (int s : solution) {
        cout << s << " ";
    }
    cout << "\n";
}
 
void back(int index, int step, int stop, 
          vector<int> &domain,
          vector<vector<int>> &result,
          vector<int> &solution) {

        if(check(solution)) {
            result.push_back(solution);
        }

    for (int i = index; i < domain.size(); i++) {
            solution.push_back(domain[i]);  
            back(i + 1, step + 1, stop, domain, result, solution);       
            solution.erase(solution.end() - 1);
    }
}
 
int main() {

    int n;
    cin >> n;
    
    vector<int> domain(n), solution;
    
    for (int i = 0; i < n; i++) {
        domain[i] = i + 1;
    }
    vector<vector<int>> result;
    back(0, 0, n, domain, result, solution);
    print_output(result);
}
