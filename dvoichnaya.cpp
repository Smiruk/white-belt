#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

int main() {
vector<int> array;
int a;
cin >> a;
int i = a;
while (i > 1) {
    if (i % 2 == 0) {
        array.push_back(0);
        i = i / 2;
}
    else if (i % 2 != 0) {
        array.push_back(1);
        i = i/2;
    }
}
if (i == 1)
    array.push_back(1);
else if (i == 0)
    array.push_back(0);

reverse(array.begin(), array.end() );
    for (int N : array)
        cout << N;

}