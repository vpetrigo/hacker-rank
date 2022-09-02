/**
 * Author: vpetrigo
 * Task:
 * Given a time in AM/PM format, convert it to military (24-hour) time.
 * Note: Midnight is 12:00:00AM on a 12-hour clock and 00:00:00 on a 24-hour clock.
 * Noon is 12:00:00PM on a 12-hour clock and 12:00:00 on a 24-hour clock.
 *
 * Input Format
 * A time in 12-hour clock format (i.e.: hh:mm:ssAM or hh:mm:ssPM),
 * where 01 ≤ h ≤ 12.
 *
 * Output Format
 * Convert and print the given time in 24-hour format, where 0 ≤ h ≤ 23.
 *
 * Sample Input
 * 07:05:45PM
 * Sample Output
 * 19:05:45
 */

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <iomanip>

using namespace std;

constexpr size_t time_parts = 3;
constexpr size_t hours_index = 0;
constexpr int noon_hours = 12;

// split string into array in the format [HH, MM, SS]
array<unsigned, time_parts> split_time(const string& am_pm) {
  array<unsigned, time_parts> times;
  constexpr char delim = ':';
  size_t prev_delim_pos = 0;
  size_t cur_delim_pos = 0;
  size_t time_idx = 0;
  auto input_len = am_pm.size();

  while (prev_delim_pos < input_len) {
    cur_delim_pos = am_pm.find(delim, prev_delim_pos);

    if (cur_delim_pos == string::npos) {
      cur_delim_pos = input_len;
    }

    times[time_idx++] = stoi(am_pm.substr(prev_delim_pos, cur_delim_pos - prev_delim_pos));
    prev_delim_pos = cur_delim_pos + 1;
  }

  return times;
}

string convert_time(const string& am_pm) {
  auto times = split_time(am_pm);

  if (am_pm.rfind("PM") != string::npos && times[hours_index] != noon_hours) {
    times[hours_index] += noon_hours;
  }

  if (am_pm.rfind("AM") != string::npos && times[hours_index] == noon_hours) {
    times[hours_index] %= noon_hours;
  }

  ostringstream military_format;

  for (size_t i = 0; i < times.size(); ++i) {
    military_format << setw(2) << setfill('0') << times[i];

    if (i + 1 != times.size()) {
      military_format << ':';
    }
  }

  return military_format.str();
}

int main() {
  string am_pm_time;

  cin >> am_pm_time;
  auto result = convert_time(am_pm_time);
  cout << result << endl;

  return 0;
}
