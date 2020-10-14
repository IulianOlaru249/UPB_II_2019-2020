#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

/**
 * Generic method to print the path to a goal state.
 *
 * Each element in the path is a pair containing a matrix cell's row and column.
 *
 * @param path        The path to a goal state.
 */
template <typename T>
void print_path(const vector<pair<T, T>> &path) {
    cout << path.size() - 1 << "\n";
    for (int i = path.size() - 1; i >= 0; --i) {
        cout << path[i].first << " " << path[i].second << "\n";
    }
}

/**
 * Method to implement the A* algorithm.
 *
 * @param M              Encoding of the grid.
 * @param row_pacman     Pacman's starting row.
 * @param col_pacman     Pacman's starting column.
 * @param row_food       Food row.
 * @param col_food       Fool column.
 *
 * @return               The shortest path between Pacman and food, determined with A*.
 *                       If no such path exists, returns an empty path.
 */
vector<pair<int, int>> astar(const vector<vector<char>> &M,
                             int row_pacman, int col_pacman,
                             int row_food, int col_food) {
    vector<pair<int, int>> path;

    // TODO: Implement A*.

    return path;
}

int main() {
    vector<pair<int, int>> path;
    vector<vector<char>> M;
    int row_pacman, col_pacman;
    int row_food, col_food;
    int num_rows, num_cols;
    
    cin >> row_pacman >> col_pacman
        >> row_food >> col_food
        >> num_rows >> num_cols;
    
    M = vector<vector<char>>(num_rows, vector<char>(num_cols, 0));
    
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            cin >> M[i][j];
        }
    }
    
    path = astar(M, row_pacman, col_pacman, row_food, col_food);
    print_path(path);
    
    return 0;
}
