#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
using namespace std;

bool isPalindrome(string word) {
    int len = word.size();
    for (int i = len; i>=1; i--) {
        if (word[i] != word[(i - len) * -1])
			return false;

	return true;
    }
}



int main() {
cout << isPalindrome("madam");
}