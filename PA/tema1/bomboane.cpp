#include <bits/stdc++.h>
#include <vector>

using namespace std;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    const int MOD = 1000000007;

    int n;
    int m;
    vector<pair<int, int>> children;

    void read_input() {
        ifstream fin("bomboane.in");
        fin >> n;
        fin >> m;
        int x, y;
        /* Push an empty pair so the children start at 1. */
        children.push_back(make_pair(-1, -1));
        for (int i=1; i <= n; i++) {
            fin >> x;
            fin >> y;
            children.push_back(make_pair(x, y));
        }
        fin.close();
    }

     /* The lines of the dp matrix represent the children and the columns
      * represent how many pieces of candy need to be handed to them so far.
      * The cells of the matrix represent the number of ways you can split 
      * the candy between the children at a point in time. When completing a
      * cell we look back in the matrix and add in how many ways we can split
      * the current number of candy - the number of candy we gave to the 
      * currentthild, between the rest of the children.
      * This solution takes O(n * m^2) time and O(n^2) space. 
      */
     int get_result() {
        vector<vector<int>> dp(n + 1, vector<int> (m + 1, 0));
        /* This is the base case. */
        for (int i = children[1].first; i <= children[1].second && i <= m; i++)
            dp[1][i] = 1;

        cout << endl;
        for (int i = 2; i <= n; i++) {
            for (int j = 0; j <= m; j++) {
                for (int k = children[i].first;
                            k <= j && k <= children[i].second; k++) {
                    /* If we cannot give that many candy to the current child
                     * then it is pointless to add the value to the sum.
                     */
                     dp[i][j] = (dp[i][j] % MOD +
                               dp[i-1][j - k] % MOD) % MOD;
                }
            }
        }
        return dp[n][m];
}


    void print_output(int result) {
        ofstream fout("bomboane.out");
        fout << result;
        fout.close();
    }
};

int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
}
