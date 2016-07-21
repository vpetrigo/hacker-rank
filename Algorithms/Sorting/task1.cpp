/**
 * Author: vpetrigo
 * Task:
 * Sample Challenge
 * This is a simple challenge to get things started. Given a sorted array (ar)
 * and a number (V), can you print the index location of V in the array?
 * The next section describes the input format. You can often skip it, if you
 * are using included methods.
 *
 * Input Format
 * There will be three lines of input:
 * V - the value that has to be searched.
 * n - the size of the array.
 * ar - nn numbers that make up the array.
 * Output Format
 * Output the index of V in the array.
 *
 * The next section describes the constraints and ranges of the input. You
 * should check this section to know the range of the input.
 *
 * Constraints
 * 1≤n≤1000
 * −1000 ≤ V ≤ 1000, V ∈ ar
 * It is guaranteed that VVwill occur in arar exactly once.
 * This "sample" shows the first input test case. It is often useful to go
 * through the sample to understand a challenge.
 *
 * Sample Input
 * 4
 * 6
 * 1 4 5 7 9 12
 * Sample Output
 * 1 
 */
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  int V, n;

  cin >> V >> n;
  vector<int> arr(n);

  for (auto& e : arr) {
    cin >> e;
  }

  cout << distance(arr.begin(), lower_bound(arr.begin(), arr.end(), V)) << endl;

  return 0;
}
