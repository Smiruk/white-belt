#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

map<char, int> BuildCharCounters(const string& word) {
  map<char, int> counters;
  for (char c : word)
    ++counters[c];
  return counters;
}

int main() {
int n;
cin >> n;
string word1, word2;

for (int i = n; i!=0; i--) {
cin >> word1 >> word2;
if (BuildCharCounters(word1) == BuildCharCounters(word2))
    cout << "YES" << endl;
else
    cout << "NO" << endl;
}
}