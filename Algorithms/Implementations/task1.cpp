/**
 * Author: vpetrigo
 * Task:
 * A Discrete Mathematics professor has a class of N students.
 * Frustrated with their lack of discipline, he decides to cancel class if fewer
 * than K students are present when class starts.
 *
 * Given the arrival time of each student, determine if the class is canceled.
 *
 * Input Format
 * The first line of input contains TT, the number of test cases.
 * Each test case consists of two lines. The first line has two space-separated
 * integers, N (students in the class) and K (the cancelation threshold).
 * The second line contains NN space-separated integers (a1,a2,…,aN) describing
 * the arrival times for each student.
 * Note: Non-positive arrival times (ai ≤ 0) indicate the student arrived early
 * or on time; positive arrival times (ai>0) indicate the student arrived ai
 * minutes late.
 *
 * Output Format
 * For each test case, print the word YES if the class is canceled or NO
 * if it is not.
 *
 * Constraints
 * 1≤T≤10
 * 1≤N≤1000
 * 1≤K≤N
 * −100 ≤ ai ≤ 100,where i∈[1,N]−100 ≤ ai ≤ 100, where i∈[1,N]
 * Note
 * If a student arrives exactly on time (ai=0), the student is considered to
 * have entered before the class started.
 *
 * Sample Input
 * 2
 * 4 3
 * -1 -3 4 2
 *
 * 4 2
 * 0 -1 2 1
 *
 * Sample Output
 * YES
 * NO
 */

#include <iostream>
#include <vector>

using namespace std;

bool test_cancellation(const vector<short>& time, const std::size_t threshold) {
  // counter for students get in time
  int in_time = 0;

  for (const auto& t : time) {
    if (t <= 0) {
      ++in_time;
    }
  }

  return (in_time >= threshold);
}

int main() {
  std::size_t test_cases;
  std::size_t num_students;
  std::size_t threshold;

  cin >> test_cases;

  for (size_t i = 0; i < test_cases; ++i) {
    cin >> num_students >> threshold;

    vector<short> students_arrive_time(num_students);

    for (auto& s : students_arrive_time) {
      cin >> s;
    }

    if (test_cancellation(students_arrive_time, threshold)) {
      cout << "NO";
    }
    else {
      cout << "YES";
    }

    cout << endl;
  }

  return 0;
}
