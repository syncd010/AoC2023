#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <numeric>
#include <cstdint>

#include "aoc.h"
#include "utils.h"

namespace aoc8 {

using namespace std;
using namespace aoc;

using Graph = unordered_map<string_view, pair<string_view, string_view>>;

// Return input parsed as a pair <path, graph as a map>
pair<string_view, Graph> parseInput(const string &input) {
  auto lines = splitStringBy(input, '\n');

  string_view path = *lines.begin();
  Graph graph{};
  for (string_view l : lines | views::drop(1)) {
    graph[l.substr(0, 3)] = make_pair(l.substr(7, 3), l.substr(12, 3));
  }

  return make_pair(path, graph);
}

// Traverses the `graph`, starting on `start`, following `path` and stopping when `isEndNode` returns true
int traverse(Graph &graph, string_view start, string_view path, auto isEndNode) {
  int steps = 0;
  string_view nextNode = start, node = nextNode;
  // Loop on path until final node found
  for (int i = 0; !isEndNode(node); i = (i+1) % path.length(), steps++, node = nextNode) {
    nextNode = (path[i] == 'L') ? graph[node].first : graph[node].second;
    // Simple sanity check, to avoid obvious loops
    if (nextNode == node) {
      cout << "Dead end detected, bailing out...\n";
      return -1;
    }
  }
  return steps;
}

Result solvePartOne(const string &input) {
  auto [path, graph] = parseInput(input);

  int steps = traverse(graph, "AAA", path, [](string_view node) { return node == "ZZZ"; });
  if (steps >= 0) 
    return steps;
  else
    return monostate();
}

Result solvePartTwo(const string &input) {
  auto [path, graph] = parseInput(input);

  // Get start nodes
  auto rg = graph
    | views::filter([](auto p) { return p.first.back() == 'A'; })
    | views::transform([](auto p) { return p.first; });
  vector<string_view> nodes(rg.begin(), rg.end());

  vector<int> pathLen{};
  // Get the path length of each start node until the end node
  for (auto node : nodes) {
    pathLen.push_back(traverse(graph, node, path, [](string_view node) { return node.back() == 'Z'; }));
  }
  // Result is the lcm of the path lengths
  return accumulate(pathLen.begin(), pathLen.end(), (int64_t)1, lcm<int64_t, int64_t>);

}
} // namespace aoc8
