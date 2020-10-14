#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include<bits/stdc++.h> 
using namespace std;

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int n, sum; 
    cin >> n >> sum;
    
    int coins[n];
    for (int i = 0; i < n ; i++) {
        cin >> coins[i];
    }
    
    int dp[sum + 1];
    for (int i = 0; i <= sum; i ++) {
        dp[i] = INT_MAX;
    }

    dp[0] = 0; //base case

    for (int s = 1; s <= sum; s++) 
    { 
        // Go through all coins smaller than i 
        for (int k = 0; k < n; k++) 
          if (coins[k] <= s) 
          { 
              int sub_res = dp[s - coins[k]]; 
              if (sub_res != INT_MAX && sub_res + 1 < dp[s]) 
                  dp[s] = sub_res + 1; 
          }
    } 
    
    if (dp[sum] != INT_MAX) cout << dp[sum];
    else cout << -1;
    
    return 0;
}
