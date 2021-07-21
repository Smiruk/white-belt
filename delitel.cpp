#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;

int main() {
int a, b;
cin >> a >> b;
int i = a;
while (true) {
    if (a % i == 0) {
        if (b % i == 0) {
            cout << i;
            break;
        }
        else
            --i;
        

    }
    else 
        --i;

}
}