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
    cout << result.size() << '\n';
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }
}

 
void printSolution(vector<int> &solution) {
    //cout << solution.size() << endl;
    for (int s : solution) {
        cout << s << " ";
    }
    cout << "\n";
}
 
void back(int step, int stop, 
          vector<int> &domain,
          vector<vector<int>> &result,
          vector<int> &solution, 
          unordered_set<int> &visited) {

    if (step == stop) {
        if(check(solution)) {
            result.push_back(solution);
        }
        return;
    }

    for (int i = 0; i < domain.size(); i++) {
        if (visited.find(domain[i]) == visited.end()) {
            visited.insert(domain[i]);
            solution.push_back(domain[i]);
            
            back(step + 1, stop, domain, result, solution, visited);
            
            visited.erase(domain[i]);
            solution.erase(solution.end() - 1);
        }
    }
}
 
int main() {

    int n, k;
    cin >> n >> k;
    
    vector<int> domain(n), solution;
    unordered_set<int> visited;
    
    for (int i = 0; i < n; i++) {
        domain[i] = i + 1;
    }
    vector<vector<int>> result;
    back(0, k, domain, result, solution, visited);
    print_output(result);
}
