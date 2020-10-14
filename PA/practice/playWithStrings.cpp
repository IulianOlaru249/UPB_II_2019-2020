#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

bool solve_one(string s1) 
{
    int odds = 0;
    int apar[123] = {0};
    for (char c : s1) {
        apar[(int)c] ++;
    }
    for (int i = 97; i <= 122; i++) {
        if (apar[i] % 2 != 0)
            odds ++;
        if (odds > 1)
            return false;
    }
    return true;
}

bool solve_two(string s1, string s2) 
{
}

int main()
{
    int n, task;
    string s1, s2;
    ifstream fin;
    fin.open("./playwithstrings.in");

    fin >> n;
    for (int i = 0; i < n; i++) {
        fin >> task;
        if (task == 1) {
            fin >> s1;
            //cout << s1 << endl;
            solve_one(s1) ? cout << "True\n" : cout << "False\n";
        }
        else {
            fin >> s1;
            fin >> s2;
            //cout << s1 << " " << s2 << endl;
            solve_two(s1, s2) ? cout << "True\n" : cout << "False\n";
        } 
    }

    fin.close();
}