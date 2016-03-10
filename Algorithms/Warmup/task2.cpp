/**
 * Author: vpetrigo
 * Task:
 * You are given an array of integers of size N. Can you find the sum of the
 * elements in the array?
 *
 * Input
 * The first line of input consists of an integer N. The next line contains
 * N space-separated integers representing the array elements.
 * Sample:
 * 6
 * 1 2 3 4 10 11
 * Output
 * Output a single value equal to the sum of the elements in the array.
 * For the sample above you would just print 3131 since
 * 1+2+3+4+10+11=311+2+3+4+10+11=31.
 */

#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main() {
  int n;
  vector<int> elems;

  cin >> n;
  elems.resize(n);

  for (auto &e : elems) {
    cin >> e;
  }

  cout << accumulate(elems.begin(), elems.end(), 0) << endl;

  return 0;
}
