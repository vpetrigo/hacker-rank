/**
 * Author: vpetrigo
 * Task:
 * Given an integer, N, traverse its digits (d1,d2,...,dn) and determine how
 * many digits evenly divide N (i.e.: count the number of times N divided by
 * each digit di has a remainder of 00). Print the number of evenly divisible
 * digits.
 *
 * Note: Each digit is considered to be unique, so each occurrence of the same
 * evenly divisible digit should be counted (i.e.: for N=111,
 * the answer is 3).
 *
 * Input Format
 *
 * The first line is an integer, T, indicating the number of test cases.
 * The T subsequent lines each contain an integer, N.
 *
 * Constraints
 * 1≤T≤15
 * 0<N<10^9
 *
 * Output Format
 *
 * For every test case, count and print (on a new line) the number of digits in
 * N that are able to evenly divide N.
 *
 * Sample Input
 * 2
 * 12
 * 1012
 *
 * Sample Output
 * 2
 * 3
 */
#include <iostream>
#include <vector>
#include <cstddef>

using namespace std;

int even_digits(const int number) {
  int even_counter = 0;
  int num_remainder = number;

  while (num_remainder) {
    // extract the first digit
    int digit = num_remainder % 10;

    if (digit != 0 && number % digit == 0) {
      ++even_counter;
    }
    // shift an input number for one digit
    num_remainder /= 10;
  }

  return even_counter;
}

int main() {
  int n;

  cin >> n;

  while (n--) {
    int number;

    cin >> number;
    cout << even_digits(number) << endl;
  }
  return 0;
}
