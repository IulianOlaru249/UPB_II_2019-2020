int maxim (int a, int b) {
    if (a > b) return a;
    else return b;
}

// Complete the longestCommonSubsequence function below.
vector<int> longestCommonSubsequence(vector<int> v, vector<int> w) {
    int m = v.size();
    int n = w.size();
    
    int dp[m + 1][n + 1]; 

    for (int i=0; i <= m; i++) 
    { 
        for (int j=0; j <=n ; j++) 
        { 
            if (i == 0 || j == 0) {
                dp[i][j] = 0; 
            }
            else if (v[i - 1] == w[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1; 
            } 
            else {
                dp[i][j] = maxim(dp[i - 1][j], dp[i][j - 1]); 
           }
        } 
    } 
 
    int k = dp[m][n]; 
    
    vector<int> secv; 
 
    int i = m, j = n; 
    while (i > 0 && j > 0) 
    {  
        if (v[i-1] == w[j-1]) 
        { 
            secv.push_back(v[i-1]);
            i--; 
            j--; 
            k--; 
        } 

        else if (dp[i-1][j] > dp[i][j-1]) { 
            i--; 
        }
        else {
            j--; 
        }
    }
    
    reverse(secv.begin(), secv.end());
    return secv;
}
