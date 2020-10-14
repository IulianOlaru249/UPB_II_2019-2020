#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    long long n;
    cin >> n;
    
    long long clas = 1000000007;
    
    long long v[n];
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
    }
    
    long long pare[n + 1];
    long long impare[n + 1];
    pare[0] = 0;
    impare[0] = 0;
    
    for (int i = 1; i <= n; i++) {
        if (v[i] % 2 == 0) {
            pare[i] = (((pare[i - 1] % clas) + (pare[i - 1] % clas)) % clas + 1 % clas ) % clas;
            impare[i] = ((impare[i - 1] % clas ) + (impare[i - 1] % clas) ) % clas;
        }
        else {
            pare[i] = ((pare[i - 1] % clas) + (impare[i - 1] % clas)) % clas;
            impare[i] = (((pare[i - 1] % clas) + (impare[i - 1] % clas)) % clas + 1 % clas ) % clas;
        }
    }
    
    cout << pare[n];
    return 0;
}
