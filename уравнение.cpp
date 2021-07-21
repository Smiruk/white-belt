#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;

// Ax² + Bx + C = 0
// 2 5 2
// 2x² + 5x + 2 = 0

int main()
{
float a, b, c;
cin >> a >> b >> c;

float D = b * b - 4 * a * c;

if (a == 0) {
    cout << -c / b;
} else {
    if (D > 0) {
        float x_1 = (-b + sqrt(D)) / (2 * a);
        float x_2 = (-b - sqrt(D)) / (2 * a);
        cout << x_1 << " " << x_2;
    }
    if (D < 0) {
        cout << "D is less than 0";
    }
    if (D == 0) {
        float x_1 = (-b + sqrt(D)) / (2 * a);
        cout << x_1;
}
}
}