#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>

#include "aoc.h"
#include "utils.h"

namespace aoc25 {

using namespace std;
using namespace aoc;

using Graph = unordered_map<int, vector<int>>;

// Parses input into a graph. Changes vertices names to ints for performance
Graph parseInput(const string &input) {
  Graph graph;
  auto lines = toVector(input | splitString('\n'));
  unordered_map<string_view, int> mapIds{};

  int nextId = 0;
  auto lookupOrInsert = [&mapIds, &nextId](string_view s) {
    if (!mapIds.contains(s)) mapIds[s] = nextId++;
    return mapIds[s];
  };

  for (auto line: lines) {
    auto p = line.find(':');
    auto edges = toVector(line.substr(p + 2) | splitString(' '));
    auto v1 = lookupOrInsert(line.substr(0, p));
    if (!graph.contains(v1)) graph[v1] = {};
    for (auto v : edges) {
      auto v2 = lookupOrInsert(v);
      graph[v1].push_back(v2);
      if (!graph.contains(v2)) graph[v2] = {};
      graph[v2].push_back(v1);
    }
  }
  return graph;
}

// Implements Karger's algorithm until 2 nodes remain.
// Returns the flow between the 2 cuts and their sizes
tuple<int, int, int> kargers(Graph graph) {
  const int sz = graph.size();
  auto vertCounts = vector(sz, 1);

  // Contract graph until 2 vertices remain    
  while(graph.size() > 2) {
    auto v1 = next(graph.begin(), (rand() % graph.size()))->first;
    auto v2 = graph[v1][(rand() % graph[v1].size())];

    // Contracts edge v1-v2, rerouting all edges from v2 to v1
    graph[v1] = toVector(graph[v1] | views::filter([v2](auto v) { return v != v2; }));
    for (auto v : graph[v2]) {
      if (v == v1) continue;
      auto it = find(graph[v].begin(), graph[v].end(), v2);
      *it = v1;
      graph[v1].push_back(v);
    }
    graph.erase(v2);
    vertCounts[v1] = vertCounts[v1] + vertCounts[v2];
  }
  return make_tuple(graph.begin()->second.size(), vertCounts[graph.begin()->first], vertCounts[next(graph.begin(), 1)->first]);
}

Result solvePartOne(const string &input) {
  auto graph = parseInput(input);
  srand(time(0));
  // Cut until flow == 3
  while (true) {
    auto [flow, cut1Count, cut2Count] = kargers(graph);
    if (flow <= 3) {
      return cut1Count * cut2Count;
    }
  }
  return monostate();
}

// Returns a path from src to dst
vector<int> bfsPath(const Graph &graph, int src, int dst) {
  unordered_map<int, int> visited{ { src, -1 } };
  vector<int> frontier{src};
  int sp = 0;
  while (sp < frontier.size()) {
    auto curr = frontier[sp++];
    if (curr == dst) break;
    for (auto v : graph.at(curr)) {
      if (!visited.contains(v)) {
        visited[v] = curr;
        frontier.push_back(v);
      }
    }
  }
  if (!visited.contains(dst)) {
    // Didn't find a path to dst, return explored
    return frontier;
  }
  vector<int> path{};
  do {
    path.push_back(dst);
    dst = visited[dst];
  } while (dst != -1);
  return path;
}

// Removes flow edges from the path from src to sink
// Returns the number of nodes reachable from src after the removal
int removeFlow(Graph graph, int src, int sink, int flow) {
    for(int i = 0; i < flow; i++) {
      auto path = bfsPath(graph, src, sink);
      auto prev = path[0];
      for(auto curr : path | views::drop(1)) {
        graph[curr] = toVector(graph[curr] | views::filter([prev](auto v) { return v != prev; }));
        graph[prev] = toVector(graph[prev] | views::filter([curr](auto v) { return v != curr; }));
        prev = curr;
      }
    }
    // Compute the reachable nodes in residual graph
    auto reachable = bfsPath(graph, src, -1);
    return reachable.size();
}

Result solvePartTwo(const string &input) {
  srand(time(0));
  auto graph = parseInput(input);
  int sz = graph.size();
  auto reachable = sz;
  auto src = graph.begin()->first;
  while (reachable == sz) {
    // Randomize sink node, or assume that it is the farthest from source
    auto sink = next(graph.begin(), (rand() % sz))->first;
    // auto sink = bfsPath(graph, src, -1).back();
    reachable = removeFlow(graph, src, sink, 3);
  }
  return reachable * (sz - reachable);
}
} // namespace aoc25
