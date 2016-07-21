#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n;

  cin >> n;

  vector<vector<unsigned>> vertices(n);
  for (size_t i = 0; i < n; ++i) {
    unsigned v, u;

    cin >> v >> u;
    vertices[v].emplace_back(u);
    vertices[u].emplace_back(v);
  }

  return 0;
}
