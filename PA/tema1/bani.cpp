#include <bits/stdc++.h>
#include <vector>

using namespace std;

const int kMod = 1e9 + 7;

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    const int MOD = 1000000007;

    int instr_set;
    int n;

    void read_input() {
        ifstream fin("bani.in");
        fin >> instr_set;
        fin >> n;
        fin.close();
    }

    /* Compute a^b in O(log n) time. */
    int fast_pow(int base, int exponent) {
        if (!exponent) {
            return 1;
        }

        int aux = 1;
        while (exponent != 1) {
            if (exponent % 2 == 0) {
                base = (1LL * base * base) % MOD;
                exponent /= 2;
            } else {
                aux = (1LL * aux  * base) % MOD;
                exponent--;
            }
        }

        return aux = (1LL * aux  * base) % MOD;
    }

    /* This solution takes O(n + log n) time and O(5n) space. */
    int get_result() {
        if (instr_set == 1) {
            /* The first bancnote can be chosen in 5 ways.
             * The second can only be chosen in 2 ways,
             * the third also in 2 ways ans so on. Therefore
             * there are 5 * 2 * ... * 2 ways to arrange the
             * bancnotes, with as many 2 as the number of bancnotes
             * minus the first one.
             * => number_of_ways = 5 * 2^(n - 1).
             * This solution takes O(log n) time and O(1) space.
             */
            return (1LL * 5 * fast_pow(2, n - 1)) % MOD;
        } else {
            /* The lines of the dp matrix represent the type of
             * bancnote available. The columns represent how many
             * bancnotes have been used already. When completing a cell,
             * we look back in the matrix, on the positions of available
             * bancnotes. Then add how many bancnotes that can appear before 
             * the current one from the previos column
             * to the current position. This way, in the end, by summing
             * the values in the last column, we will get the total number
             * of ways to arrange the bancnotes.
             * This solution takes O(n) time and O(5n) space.
             */
            vector<vector<int>> dp(6, vector<int> (n + 1, 0));

            /* The first bancnote can be whichever. */
            for (int i = 1; i <= 5; i++)
                dp[i][1] = 1 % MOD;

            for (int i = 2; i <= n ; i++) {
                /* A bancnote of 10 can only follow a 50, 100 or 500 */
                dp[1][i] = ((dp[1][i] % MOD + dp[2][i - 1] % MOD) % MOD +
                    (dp[3][i - 1] % MOD + dp[5][i - 1] % MOD) % MOD) % MOD;
                /* A bancnote of 50 can only follow a 10 or 200 */
                dp[2][i] = ((dp[2][i] % MOD + dp[1][i - 1] % MOD) % MOD +
                    dp[4][i - 1] % MOD) % MOD;
                /* A bancnote of 100 can only follow a 10, 100 or 200 */
                dp[3][i] = ((dp[3][i] % MOD + dp[1][i - 1] % MOD) % MOD +
                    (dp[3][i - 1] % MOD + dp[4][i - 1] % MOD) %MOD) %MOD;
                /* A bancnote of 200 can only follow a 50 or 500 */
                dp[4][i] = ((dp[4][i] % MOD + dp[2][i - 1] % MOD) % MOD +
                    dp[5][i - 1] % MOD) % MOD;
                /* A bancnote of 500 can only follow a 200 */
                dp[5][i] = (dp[5][i] % MOD + dp[4][i - 1] % MOD) % MOD;
            }

            int ways = 0;
            for (int i = 1; i <= 5; i++)
            ways = (ways % MOD + dp[i][n] % MOD) % MOD;
            return ways;
        }
    }

    void print_output(int result) {
        ofstream fout("bani.out");
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
