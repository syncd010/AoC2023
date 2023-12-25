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
#include <random>

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

// Contracts edge v1-v2 creating the new vertice newV
void contract(Graph &graph, const int v1, const int v2, const int newV) {
  for (auto v : graph[v1]) {
    if (v == v2) continue;
    graph[v].erase(find(graph[v].begin(), graph[v].end(), v1));
    graph[v].push_back(newV);
    graph[newV].push_back(v);
  }

  for (auto v : graph[v2]) {
    if (v == v1) continue;
    graph[v].erase(find(graph[v].begin(), graph[v].end(), v2));
    graph[v].push_back(newV);
    graph[newV].push_back(v);
  }
}

Result solvePartOne(const string &input) {
  auto originalGraph = parseInput(input);
  default_random_engine generator(time(0));
  uniform_int_distribution<int> dist(0, originalGraph.size() * 10);

  const int sz = originalGraph.size();
  bool partitioned = false;
  int64_t res = 0;
  while (!partitioned) {
    auto graph{originalGraph};
    auto rgKeys = graph | views::keys;
    // Set with remaining vertices, not contracted
    auto vertices = unordered_set(rgKeys.begin(), rgKeys.end());
    // Contracted vertices counts to get final result
    auto vertCounts = vector(sz * 2 - 2, 1);

    // Contract graph until 2 vertices remain    
    for(int nextV = sz; vertices.size() > 2; nextV++) {
      auto v1 = vertices.extract(next(vertices.begin(), (dist(generator) % vertices.size()))).value();
      auto v2 = graph[v1][(dist(generator) % graph[v1].size())];
      vertices.erase(v2);
      vertices.insert(nextV);
      graph[nextV] = {};
      contract(graph, v1, v2, nextV);
      vertCounts[nextV] = vertCounts[v1] + vertCounts[v2];
    }
    // Check if we got the min cut
    partitioned = (graph[*vertices.begin()].size() <= 3);
    if (partitioned) {
      res = accumulate(vertices.begin(), vertices.end(), 1, [&vertCounts](auto acc, auto v) { return acc * vertCounts[v];});
    }
  }

  return res;
}

Result solvePartTwo(const string &input) {
  return std::monostate{};
}
} // namespace aoc25
