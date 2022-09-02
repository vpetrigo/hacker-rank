/**
 * Author: vpetrigo
 * Task:
 * Watson gives two integers (AA and BB) to Sherlock and asks if he can count
 * the number of square integers between AA and BB (both inclusive).
 *
 * Note: A square integer is an integer which is the square of any integer.
 * For example, 1, 4, 9, and 16 are some of the square integers as they are
 * squares of 1, 2, 3, and 4, respectively.
 *
 * Input Format
 * The first line contains T, the number of test cases. T test cases follow,
 * each in a new line.
 * Each test case contains two space-separated integers denoting A and B.
 *
 * Output Format
 * For each test case, print the required answer in a new line.
 *
 * Constraints
 * 1 ≤ T ≤ 100
 * 1 ≤ A ≤ B ≤ 10^9
 *
 * Sample Input
 * 2
 * 3 9
 * 17 24
 *
 * Sample output
 * 2
 * 0
 */
#include <iostream>
#include <cmath>
#include <cstddef>

using namespace std;

int calc_squares(const double A, const double B) {
  double left_bound, right_bound;
  int sq_counter = 0;

  // get bounds
  left_bound = ceil(sqrt(A));
  right_bound = floor(sqrt(B));

  if (left_bound <= right_bound) {
    // calculate number of numbers in a bounds
    sq_counter = right_bound - left_bound + 1;
  }

  return sq_counter;
}

int main() {
  int n;

  cin >> n;

  for (size_t i = 0; i < n; ++i) {
    double A, B;

    cin >> A >> B;
    cout << calc_squares(A, B) << endl;
  }

  return 0;
}
