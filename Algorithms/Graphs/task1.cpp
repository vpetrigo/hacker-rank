/**
 * Author: vpetrigo
 * Task:
 * Given an undirected graph consisting of N nodes (labelled 1 to N) where a
 * specific given node S represents the start position and an edge between any
 * two nodes is of length 6 units in the graph.
 *
 * It is required to calculate the shortest distance from start position
 * (Node S) to all of the other nodes in the graph.
 *
 * Note 1: If a node is unreachable , the distance is assumed as −1.
 * Note 2: The length of each edge in the graph is 6 units.
 *
 * Input Format
 * The first line contains T, denoting the number of test cases.
 * First line of each test case has two integers N, denoting the number of nodes
 * in the graph and M, denoting the number of edges in the graph.
 * The next M lines each consist of two space separated integers x y, where x
 * and y denote the two nodes between which the edge exists.
 * The last line of a testcase has an integer S, denoting the starting position.
 *
 * Constraints
 * 1 ≤ T ≤ 10
 * 2 ≤ N ≤ 1000
 * 1 ≤ M ≤ (N×(N−1))/2
 * 1 ≤ x,y,S ≤ N
 *
 * Output Format
 * For each of T test cases, print a single line consisting of N−1
 * space-separated integers, denoting the shortest distances of the N-1 nodes
 * from starting position SS. This will be done for all nodes same as in the
 * order of input 1 to N.
 *
 * For unreachable nodes, print −1.
 *
 * Sample Input
 * 2
 * 4 2
 * 1 2
 * 1 3
 * 1
 * 3 1
 * 2 3
 * 2
 *
 * Sample Output
 * 6 6 -1
 * -1 6
 */
#include <iostream>
#include <vector>
#include <deque>

using namespace std;

vector<int> bfs(const vector<vector<unsigned>>& vertices, const size_t s) {
  constexpr unsigned edge_weight = 6;
  vector<int> distance(vertices.size(), -1);
  deque<unsigned> queue;

  distance[s] = 0;
  queue.push_back(s);

  while (!queue.empty()) {
    auto v = queue.front();

    queue.pop_front();
    for (size_t i = 0; i < vertices[v].size(); ++i) {
      if (distance[vertices[v][i]] == -1) {
        distance[vertices[v][i]] = distance[v] + edge_weight;
        queue.push_back(vertices[v][i]);
      }
    }
  }

  return distance;
}

int main() {
  size_t t;

  cin >> t;

  for (size_t i = 0; i < t; ++i) {
    size_t n, m;

    cin >> n >> m;
    vector<vector<unsigned>> vertices(n);

    for (size_t j = 0; j < m; ++j) {
      unsigned v, u;

      cin >> v >> u;
      vertices[v - 1].emplace_back(u - 1);
      vertices[u - 1].emplace_back(v - 1);
    }

    size_t s;

    cin >> s;
    auto bfs_result = bfs(vertices, s - 1);

    for (const auto& e : bfs_result) {
      if (e != 0) {
        cout << e << ' ';
      }
    }
    cout << endl;
  }

  return 0;
}
