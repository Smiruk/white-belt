#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;
/*Если исходная стоимость товара больше A рублей, 
*то на неё устанавливается скидка в X процентов. 
*Если исходная стоимость товара больше B рублей, то на неё устанавливается скидка в Y процентов.
N = исходная стоимость
Y = cкидка1
X = скидка2
На вход программе даны пять вещественных чисел: N, A, B, X, Y (A < B) - где N - исходная стоимость товара. Выведите стоимость покупки товара с учётом скидки.
*/ 

int main() {
int N, A, B, X, Y;
cin >> N >> A >> B >> X >> Y;
if (N > A) {
float endprice = N *= (1 - X/100);
cout << endprice;
}
