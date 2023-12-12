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

// Return input parsed as a pair <path, tree as a map>
pair<string_view, unordered_map<string_view, pair<string_view, string_view>>> parseInput(const string &input) {
  auto rg = splitStringBy(input, '\n')
    | views::filter([](auto l) { return !l.empty(); });
  auto lines = vector(rg.begin(), rg.end());

  string_view path = lines[0];
  unordered_map<string_view, pair<string_view, string_view>> tree{};

  for (string_view l : lines | views::drop(1)) {
    size_t defPos = l.find(" = ");
    string_view key{l.substr(0, defPos)}, values{l.substr(defPos + 3)};
    size_t splitPos = values.find(", ");
    string_view left{values.substr(1, splitPos - 1)}, right{values.substr(splitPos + 2, values.length() - splitPos - 3)};

    tree[key] = make_pair(left, right);
  }

  return make_pair(path, tree);
}

Result solvePartOne(const string &input) {
  auto [path, tree] = parseInput(input);

  int steps = 0;
  string_view nextNode{"AAA"}, node = nextNode;
  // Loop on path until final node found
  for (int i = 0; node != "ZZZ"; i = (i+1) % path.length(), steps++, node = nextNode) {
    nextNode = (path[i] == 'L') ? tree[node].first : tree[node].second;
    // Simple sanity check, to avoid obvious loops
    if (nextNode == node) {
      cout << "\nDirect loop detected, bailing out...\n";
      return -1;
    }
  }
  return steps;
}

Result solvePartTwo(const string &input) {
  auto [path, tree] = parseInput(input);

  // Get start nodes
  auto rg = tree
    | views::filter([](auto p) { return p.first.back() == 'A'; })
    | views::transform([](auto p) { return p.first; });
  vector<string_view> nodes(rg.begin(), rg.end());

  vector<int> loops{};    // Loop counts for each node
  for (auto node : nodes) {
    vector<string_view> visited{node};
    // Loop on path until a loop is found
    for (int i = 0; ; i = (i+1) % path.length()) {
        node = (path[i] == 'L') ? tree[node].first : tree[node].second;
        // If possible end node, check if there's a loop
        if (node.back() == 'Z') {
          auto it = find(visited.begin(), visited.end(), node);
          if (it != visited.end()) {
            // Loop found, save the number of steps
            loops.push_back(distance(it, visited.end()));
            break;
          }
        }
        visited.push_back(node);
    }
  }
  return accumulate(loops.begin(), loops.end(), 1LL, lcm<int64_t, int64_t>);
}
} // namespace aoc8
