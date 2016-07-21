#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool check_rest_grids(const vector<string>& grid, const vector<string>& p_grid,
                      const size_t line, const size_t found_idx) {
  const auto elem_len = p_grid.front().size();
  // one line in pattern is already checked
  const size_t pattern_size = p_grid.size() - 1;
  size_t pattern_idx = 1;

  for (size_t i = line; i < line + pattern_size; ++i) {
    if (i >= grid.size() || grid[i].substr(found_idx, elem_len) != p_grid[pattern_idx++]) {
      return false;
    }
  }

  return true;
}

string search_for_pattern(const vector<string>& grid,
                          const vector<string>& p_grid) {
  const auto gr_size = grid.size();
  bool pattern_found = false;

  for (size_t i = 0; i < gr_size && !pattern_found; ++i) {
    // index where pattern element was found
    size_t found_idx = 0;

    // search on a grid line for a pattern substring
    while ((found_idx = grid[i].find(p_grid.front(), found_idx)) !=
               string::npos &&
           !pattern_found) {
      pattern_found = check_rest_grids(grid, p_grid, i + 1, found_idx);
      ++found_idx;
    }
  }

  return pattern_found ? string{"YES"} : string{"NO"};
}

int main() {
  unsigned t;

  cin >> t;

  for (size_t i = 0; i < t; ++i) {
    unsigned R, C;

    cin >> R >> C;
    vector<string> grid(R);

    for (auto& e : grid) {
      cin >> e;
    }

    unsigned r, c;

    cin >> r >> c;

    vector<string> pattern_grid(r);

    for (auto& e : pattern_grid) {
      cin >> e;
    }

    cout << search_for_pattern(grid, pattern_grid) << endl;
  }

  return 0;
}
