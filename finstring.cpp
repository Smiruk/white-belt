#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;

int main() {
string s;
cin >> s;
int vivid = 0;
for (int i = 0; i < s.size(); ++i) {
    if (s[i] == 'f') {
        ++i;
        if (s[i] == 'f') {
            cout << i << endl;
            break;
        }
        else
            vivid << -1;
            break;
    }
    else 
        vivid << -2;
    }
    if (vivid != 0)
        cout << vivid;
}
