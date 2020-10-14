#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
using namespace std;

bool check(vector<vector<int>> &board, int row, int col) {
    int i, j; 

    for (i = 0; i < col; i++) 
        if (board[row][i]) 
            return false; 
  
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--) 
        if (board[i][j]) 
            return false; 
  

    for (i = row, j = col; j >= 0 && i < board.size(); i++, j--) 
        if (board[i][j]) 
            return false; 
  
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
 
void back(int step, int stop,
          int col,
          vector<vector<int>> &board,
          vector<int> &domain,
          vector<vector<int>> &result,
          vector<int> &solution, 
          unordered_set<int> &visited) {

    if (step == stop) {
        result.push_back(solution);
        return;
    }

    for (int i = 0; i < domain.size(); i++) {
        if (visited.find(domain[i]) == visited.end()) {
            if(check(board, i, col)) {
                visited.insert(domain[i]);
                solution.push_back(domain[i]);
                board[i][col] = 1;
            
                back(step + 1, stop, col + 1, board, domain, result, solution, visited);
            
                board[i][col] = 0;
                visited.erase(domain[i]);
                solution.erase(solution.end() - 1);
            }
        }
    }
}
 
int main() {
    int n;
    cin >> n;
    
    vector<int> domain(n), solution;
    unordered_set<int> visited;
    
    for (int i = 0; i < n; i++) {
        domain[i] = i + 1;
    }
    vector<vector<int>> result;
    vector<vector<int>> board(n, vector <int> (n, 0));
    back(0, n, 0, board, domain, result, solution, visited);
    print_output(result);
}
