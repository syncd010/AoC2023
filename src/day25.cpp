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

using Graph = unordered_map<string, vector<string>>;

Graph parseInput(const string &input) {
  Graph graph;
  auto lines = toVector(input | splitString('\n'));

  for (auto line: lines) {
    auto p = line.find(':');
    auto edges = toVector(line.substr(p + 2) | splitString<string>(' '));
    auto v1 = string(line.substr(0, p));
    if (!graph.contains(v1)) graph[v1] = vector<string>{};
    for (auto v2 : edges) {
      graph[v1].push_back(v2);
      if (!graph.contains(v2)) graph[v2] = vector<string>{};
      graph[v2].push_back(v1);
    }
  }
  return graph;
}

void contract(Graph &graph, const string &v1, const string &v2, const string &vNew) {
  for (auto v : graph[v1]) {
    if (v == v2) continue;
    graph[v].erase(find(graph[v].begin(), graph[v].end(), v1));
    graph[v].push_back(vNew);
    graph[vNew].push_back(v);
  }

  for (auto v : graph[v2]) {
    if (v == v1) continue;
    graph[v].erase(find(graph[v].begin(), graph[v].end(), v2));
    graph[v].push_back(vNew);
    graph[vNew].push_back(v);
  }
}

Result solvePartOne(const string &input) {
  auto originalGraph = parseInput(input);

  default_random_engine generator(time(0));
  uniform_int_distribution<int> dist(0, originalGraph.size() * 10);

  bool partitioned = false;
  int64_t res = 0;
  while (!partitioned) {
    auto graph{originalGraph};
    auto rgKeys = graph | views::keys;
    auto vertices = unordered_set(rgKeys.begin(), rgKeys.end());
    auto rgCounts = rgKeys | views::transform([](auto k) { return make_pair(k, 1);});
    unordered_map<string, int> vertCounts(rgCounts.begin(), rgCounts.end());

    while(vertices.size() > 2) {
      auto v1 = vertices.extract(next(vertices.begin(), (dist(generator) % vertices.size()))).value();
      auto v2 = graph[v1][(dist(generator) % graph[v1].size())];
      vertices.erase(v2);

      auto vNew = to_string(graph.size());
      graph[vNew] = vector<string>{};
      contract(graph, v1, v2, vNew);
      vertices.insert(vNew);
      vertCounts[vNew] = vertCounts[v1] + vertCounts[v2];
    }
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
