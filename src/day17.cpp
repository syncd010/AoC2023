#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <deque>
#include <queue>
#include <unordered_set>
#include <format>

#include "aoc.h"
#include "utils.h"

namespace aoc17 {

using namespace std;
using namespace aoc;

using Grid = vector<vector<int>>;
using Pos = vec2<int>;
using Dir = vec2<int>;

Grid parseInput(const string &input) {
  return toVector(input
    | splitString('\n')
    | views::transform([](auto line) {
      return toVector(line | views::transform([](auto c) { return c - '0'; }));
      })
  );
}

struct State {
  Pos pos;
  Dir dir;
  int cost;
  // int prev = -1;

  bool operator==(const State &other) const {
    return (pos == other.pos) &&
      (dir == other.dir);
  };
  auto operator<(const State &other) const {
    return cost < other.cost;
  };
};

std::ostream & operator<<(std::ostream &os, const State &s) {
	os << "Pos: " << s.pos << ", Dir: " << s.dir << ", Cost: " << s.cost;
	return os;
}

// Rotates clockwise/counterclockwise
inline Dir rotate(const Dir &pos, int dir = 1) { 
  return Dir(pos.y * dir, pos.x * (-dir));
}

auto successors(const State &state, const auto &grid, int minSteps, int maxSteps) {
  vector<State> successors{};
  // Generate all possible successors: turns between minSteps and maxSteps
  int cost = state.cost;
  auto pos = state.pos + state.dir;
  for (int i = 1; i <= maxSteps && insideBoard(grid, pos); i++, pos += state.dir) {
    cost += grid[pos.y][pos.x];
    if (i < minSteps) continue;
    // Add turns
    auto turnLeft = rotate(state.dir, 1), turnRight = rotate(state.dir, -1);
    successors.push_back(State(pos, turnLeft, cost));
    successors.push_back(State(pos, turnRight, cost));
  }
  return successors;
};

inline int dirIdx(Dir d) { return abs(d.x) * 2 + (d.x + 1) / 2 + (d.y + 1) / 2; };

Result solve(const string &input, int minSteps = 1, int maxSteps = 3) {
  auto grid = parseInput(input);
  auto h = grid.size(), w = grid[0].size();

  Pos finalPos{(int)w - 1, (int)h - 1};
  State startEast{Pos{0, 0}, Dir{1, 0}, 0}, startSouth{Pos{0, 0}, Dir{0, 1}, 0};
  auto comparisonFn = [](const State &s1, const State &s2) noexcept {
    return s1.cost > s2.cost;     // Minimum cost
  };
  priority_queue<State, vector<State>, decltype(comparisonFn)> frontier { comparisonFn, vector{startSouth, startEast}};
  auto reached = vector(h, vector(w, vector<int>(4, { INT32_MAX })));
  // Djikstra search
  while (!frontier.empty()) {
    auto current = frontier.top();
    frontier.pop();
      // Check if we can reach the current node with strictly smaller cost
    if (current.cost > reached[current.pos.y][current.pos.x][dirIdx(current.dir)]) continue;
    if (current.pos == finalPos) {
      return current.cost;
    }
    for (auto next : successors(current, grid, minSteps, maxSteps)) {
      // Check if we can already reach the next node with smaller cost
      auto &prevCost = reached[next.pos.y][next.pos.x][dirIdx(next.dir)];
      if (next.cost >= prevCost) continue;
      prevCost = next.cost;
      frontier.push(next);
    }
  }
  return monostate();
}

Result solvePartOne(const string &input) {
  return solve(input, 1, 3);
}

Result solvePartTwo(const string &input) {
  return solve(input, 4, 10);
}
} // namespace aoc17
