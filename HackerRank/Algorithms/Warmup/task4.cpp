/**
 * Author: vpetrigo
 * Task:
 * Given a square matrix of size N×N, calculate the absolute difference between
 * the sums of its diagonals.
 *
 * Input Format
 * The first line contains a single integer, N. The next N lines denote the
 * matrix's rows, with each line containing NN space-separated integers
 * describing the columns.
 *
 * Output Format
 * Print the absolute difference between the two sums of the matrix's
 * diagonals as a single integer.
 *
 * Sample Input
 * 3
 * 11 2 4
 * 4 5 6
 * 10 8 -12
 *
 * Sample Output
 * 15
 */

#include <iostream>
#include <vector>

using namespace std;

// it is meaned that an input matrix is NxN size
int diagnoal_diff(const vector<vector<int>> &matrix) {
  int n = matrix.size();
  int diag_idx = 0;
  int diff = 0;

  while (diag_idx < n) {
    diff += matrix[diag_idx][diag_idx] - matrix[diag_idx][n - diag_idx - 1];
    ++diag_idx;
  }

  if (diff < 0) {
    diff = -diff;
  }

  return diff;
}

int main() {
  int n;
  cin >> n;
  vector<vector<int>> matrix(n, vector<int>(n));

  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      cin >> matrix[i][j];
    }
  }

  cout << diagnoal_diff(matrix) << endl;

  return 0;
}
