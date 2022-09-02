/**
 * Author: vpetrigo
 * Task:
 * Calvin is driving his favorite vehicle on the 101 freeway. He notices that
 * the check engine light of his vehicle is on, and he wants to service it
 * immediately to avoid any risks. Luckily, a service lane runs parallel to the
 * highway. The length of the service lane is N units. The service lane
 * consists of NN segments of equal length and different width.
 *
 * Calvin can enter to and exit from any segment. Let's call the entry segment
 * as index ii and the exit segment as index jj. Assume that the exit segment
 * lies after the entry segment (i ≤ j) and 0 ≤ i. Calvin has to pass through
 * all segments from index ii to index jj (both inclusive).
 *
 * Paradise Highway
 *
 * Calvin has three types of vehicles - bike, car, and truck - represented by
 * 1, 2 and 3, respectively. These numbers also denote the width of the vehicle.
 *
 * You are given an array width of length N, where width[k] represents
 * the width of the kth segment of the service lane. It is guaranteed that
 * while servicing he can pass through at most 1000 segments, including the
 * entry and exit segments.
 *
 * If width[k] = 1, only the bike can pass through the kth segment.
 * If width[k] = 2, the bike and the car can pass through the kth segment.
 * If width[k] = 3, all three vehicles can pass through the kth segment.
 *
 * Given the entry and exit point of Calvin's vehicle in the service lane,
 * output the type of the largest vehicle which can pass through the service
 * lane (including the entry and exit segments).
 *
 * Input Format
 * The first line of input contains two integers, N and T, where N denotes the
 * length of the freeway and T the number of test cases. The next line has N
 * space-separated integers which represent the widthwidth array.
 *
 * T test cases follow. Each test case contains two integers, i and j,
 * where i is the index of the segment through which Calvin enters the service
 * lane and j is the index of the lane segment through which he exits.
 *
 * Constraints
 * 2 ≤ N ≤ 100000
 * 1 ≤ T ≤ 1000
 * 0 ≤ i < j < N
 * 2 ≤ j − i + 1 ≤ min(N,1000)
 * 1 ≤ width[k] ≤ 3, where 0 ≤ k < N
 *
 * Output Format
 * For each test case, print the number that represents the largest vehicle
 * type that can pass through the service lane.
 * Note: Calvin has to pass through all segments from index i to index j
 * (both inclusive).
 *
 * Sample Input
 * 8 5
 * 2 3 1 2 3 2 3 3
 * 0 3
 * 4 6
 * 6 7
 * 3 5
 * 0 7
 *
 * Sample Output
 * 1
 * 2
 * 3
 * 2
 * 1
 */
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

unsigned max_vehicle(const vector<unsigned>& sl, unsigned en_p, unsigned ex_p) {
  auto min_vehicle = sl[en_p];

  for (auto i = en_p + 1; i <= ex_p; ++i) {
    if (sl[i] < min_vehicle) {
      min_vehicle = sl[i];
    }
  }

  return min_vehicle;
}

int main() {
  int n, k;

  cin >> n >> k;
  vector<unsigned> service_lanes(n);

  for (auto& e : service_lanes) {
    cin >> e;
  }

  for (size_t i = 0; i < k; ++i) {
    unsigned enter_point, exit_point;

    cin >> enter_point >> exit_point;
    cout << max_vehicle(service_lanes, enter_point, exit_point) << endl;
  }

  return 0;
}
