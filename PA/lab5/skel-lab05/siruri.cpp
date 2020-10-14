 #include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
using namespace std;

bool check(vector<char> &solution, int k) {
        int len = solution.size();
        int f = 0;

        for (int i = len - 2; i > -1 && solution[i] == solution[i + 1]; --i) {
            if (++f == k) {
                return false;
            }
        }

        return true;
}

void print_output(vector<vector<char> > result) {
    cout << result.size() << "\n";
    for (int i = 0; i < result.size(); i++) {
        for (int j = 0; j < result[i].size(); j++) {
            cout << result[i][j];
        }
        cout << endl;
    }
}

void back(int step, int stop, int k,
          vector<char> &domain,
          vector<int> &freq,
          vector<vector<char>> &result,
          vector<char> &solution) {
    
    if (step == stop) {
        if (check(solution, k)) {
            result.push_back(solution);
        }
        return;
    }

    for (int i = 0; i < domain.size(); i++) {
        if (freq[i] > 0) {
                solution.push_back(domain[i]);
                freq[i] --;
            
                if (check(solution, k)) {
                    back(step + 1, stop, k, domain, freq, result, solution);       
                }
            
                freq[i]++;
                solution.erase(solution.end() - 1);
        }
    }
}
 
int main() {

    int n, k;
    cin >> n >> k;
    
    vector<char> characters (n);
    for (int i = 0; i < n; i++) {
        cin >> characters[i];
    }
    
    vector<int> frequencies (n);
    int freqSum = 0;
    for (int i = 0; i < n; i++) {
        cin >> frequencies[i];
        freqSum += frequencies[i];
    }
    
    vector<char> solution;
    vector<vector<char>> result;
    back(0, freqSum, k, characters, frequencies, result, solution);
    print_output(result);
}
