/**
 * Author: vpetrigo
 * Task:
 * You are given N sticks, where the length of each stick is a positive integer.
 * A cut operation is performed on the sticks such that all of them are reduced
 * by the length of the smallest stick.
 *
 * Suppose we have six sticks of the following lengths:
 * 5 4 4 2 2 8
 * Then, in one cut operation we make a cut of length 2 from each of the six
 * sticks. For the next cut operation four sticks are left (of non-zero length),
 * whose lengths are the following:
 * 3 2 2 6
 *
 * The above step is repeated until no sticks are left.
 * Given the length of N sticks, print the number of sticks that are left
 * before each subsequent cut operations.
 * Note: For each cut operation, you have to recalcuate the length of smallest
 * sticks (excluding zero-length sticks).
 *
 * Input Format
 * The first line contains a single integer N.
 * The next line contains N integers: a0, a1,...aN-1 separated by space,
 * where ai represents the length of ith stick.
 *
 * Output Format
 * For each operation, print the number of sticks that are cut, on separate
 * lines.
 * Constraints
 * 1 ≤ N ≤ 1000
 * 1 ≤ ai ≤ 1000
 *
 * Sample Input #00
 * 6
 * 5 4 4 2 2 8
 * Sample Output #00
 * 6
 * 4
 * 2
 * 1
 *
 * Sample Input #01
 * 8
 * 1 2 3 4 3 3 2 1
 * Sample Output #01
 * 8
 * 6
 * 4
 * 1
 */
#include <iostream>
#include <list>
#include <iterator>
#include <cstddef>

using namespace std;

unsigned get_min_stick(const list<unsigned>& sticks) {
  auto min = sticks.front();

  for (auto it = next(sticks.begin(), 1); it != sticks.end(); ++it) {
    if (*it < min) {
      min = *it;
    }
  }

  return min;
}

void substract_stick(list<unsigned>& sticks, const unsigned stick_len) {
  auto it = sticks.begin();

  while (it != sticks.end()) {
    *it -= stick_len;

    // if stick's length is 0, then exclude it from a list
    if (*it == 0) {
      auto tmp_it = it++;

      sticks.erase(tmp_it);
    }
    else {
      ++it;
    }
  }
}

int main() {
  int n;

  cin >> n;
  list<unsigned> sticks(n);

  for (auto& e : sticks) {
    cin >> e;
  }

  while (!sticks.empty()) {
    cout << sticks.size() << endl;
    auto min_stick_len = get_min_stick(sticks);
    substract_stick(sticks, min_stick_len);
  }

  return 0;
}
