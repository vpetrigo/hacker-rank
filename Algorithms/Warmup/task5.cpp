/**
 * Author: vpetrigo
 * Task:
 * Given an array of integers, calculate which fraction of the elements are
 * positive, negative, and zeroes, respectively. Print the decimal value of
 * each fraction.
 *
 * Input Format
 * The first line, N, is the size of the array.
 * The second line contains NN space-separated integers describing the array
 * of numbers (A1,A2,A3,⋯,AN).
 *
 * Output Format
 * Print each value on its own line with the fraction of positive numbers first,
 * negative numbers second, and zeroes third.
 *
 * Sample Input
 * 6
 * -4 3 -9 0 4 1
 *
 * Sample Output
 * 0.500000
 * 0.333333
 * 0.166667
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

using namespace std;

// we have to calculate fraction of how many numbers are in the certain group
// zero, positive, negative
constexpr size_t groups = 3;

enum FRACTION_GROUP { POSITIVE = 0, NEGATIVE, ZERO };

array<double, groups> calc_fraction(const vector<int> &elems) {
  auto q_elems = elems.size();
  array<double, groups> fractions;

  for (const auto &e : elems) {
    if (e > 0) {
      ++fractions[POSITIVE];
    } else if (e < 0) {
      ++fractions[NEGATIVE];
    } else {
      ++fractions[ZERO];
    }
  }

  for (auto &fr : fractions) {
    fr /= q_elems;
  }

  return fractions;
}

int main() {
  int n;
  cin >> n;
  vector<int> elems(n);

  for (auto &e : elems) {
    cin >> e;
  }

  auto result = calc_fraction(elems);

  cout << fixed;
  for (const auto &e : result) {
    cout << e << endl;
  }

  return 0;
}
