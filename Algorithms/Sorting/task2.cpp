/**
 * Author: vpetrigo
 * Task:
 * Insert element into sorted list
 * Given a sorted list with an unsorted number ee in the rightmost cell, can
 * you write some simple code to insert ee into the array so that it remains
 * sorted?
 *
 * Print the array every time a value is shifted in the array until the array is
 * fully sorted. The goal of this challenge is to follow the correct order of
 * insertion sort.
 * Guideline: You can copy the value of ee to a variable and consider its cell
 * "empty". Since this leaves an extra cell empty on the right, you can shift
 * everything over until V can be inserted. This will create a duplicate of 
 * each value, but when you reach the right spot, you can replace it with ee.
 *
 * Input Format
 * There will be two lines of input:
 * Size - the size of the array
 * Arr - the unsorted array of integers
 *
 * Output Format
 * On each line, output the entire array every time an item is shifted in it.
 *
 * Constraints
 * 1 ≤ Size ≤ 1000
 * −10000 ≤ e ≤ 10000,e ∈ Arr
 *
 * Sample Input
 * 5
 * 2 4 6 8 3
 * Sample Output
 * 2 4 6 8 8
 * 2 4 6 6 8
 * 2 4 4 6 8
 * 2 3 4 6 8
 */
#include <iostream>
#include <vector>
#include <cstddef>

using namespace std;

void print_arr(const vector<int>& arr) {
  for (const auto& e : arr) {
    cout << e << ' ';
  }
  cout << endl;
}

void last_sort(vector<int>& arr) {
  int to_sort = arr.back();
  auto arr_len = arr.size();

  for (size_t i = arr_len - 1; i < arr_len; --i) {
    if (to_sort < arr[i - 1]) {
      arr[i] = arr[i - 1];
    }
    else {
      arr[i] = to_sort;
      break;
    }
    print_arr(arr);
  }

  print_arr(arr);
}

int main() {
  size_t size;

  cin >> size;

  vector<int> arr(size);

  for (auto& e : arr) {
    cin >> e;
  }

  last_sort(arr);

  return 0;
}
