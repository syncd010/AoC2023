#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <queue>
#include <unordered_set>
#include <set>

#include "aoc.h"
#include "utils.h"

namespace aoc17 {

using namespace std;
using namespace aoc;

using Grid = vector<vector<int>>;
using Pos = vec2<int>;
using Dir = vec2<int>;

Grid parseInput(const string &input) {
  auto rg = splitStringBy(input, '\n')
    | views::transform([](auto line) {
      auto rg = line | views::transform([](auto c) { return c - '0'; });
      return vector(rg.begin(), rg.end());
      });
  return vector(rg.begin(), rg.end());
}

/**
 * Simple greedy search
 * 
 * @param start Start node
 * @param successorsFn Function that returns successors to a node. Should be (const T &) -> vector(T)
 * @param isGoalFn Function that returns whether a node is the goal. Should be (const T &) -> bool
 * @param comparisonFn Function to order nodes in the priority queue. Should be (const T &) -> bool
*/
template<typename T>
T greedySearch(const T &start, 
               auto successorsFn, 
               auto isGoalFn, 
               auto comparisonFn) {
  int64_t count = 0;
  priority_queue<T, vector<T>, decltype(comparisonFn)> frontier{comparisonFn};
  frontier.push(start);
  unordered_set<T> explored{start};
  while (!frontier.empty()) {
    count ++;
    auto current = frontier.top();
    frontier.pop();
    if (isGoalFn(current)) {
      cout << "Explored: " << count << "\n";
      return current;
    }
    for (auto next : successorsFn(current)) {
      if (explored.contains(next)) continue;
      explored.insert(next);
      frontier.push(next);
    }
  }
  return T{};
}

struct State {
  Pos position;
  Dir direction;
  int straightStepsTaken;
  int cost;

  // auto operator<=>(const State &other) const {
  //   if (cost < other.cost) return -1;
  //   if (cost > other.cost) return 1;
  //   if (*this == other) return 0;
  //   return -1;
  // };

  // bool operator==(const State &other) const = default;
  auto operator<=>(const State &other) const = default;
  bool operator==(const State &other) const {
    return (position == other.position) &&
      (direction == other.direction) &&
      (straightStepsTaken == other.straightStepsTaken);
  };
  // auto operator<(const State &other) const {
  //   return cost < other.cost;
  // };
  // auto operator>(const State &other) const {
  //   return cost > other.cost;
  // };
};

std::ostream & operator<<(std::ostream &os, const State &s) {
	os << "Pos: " << s.position << ", Dir: " << s.direction << ", Steps taken: " << s.straightStepsTaken << ", Cost: " << s.cost;
	return os;
}

// Directions and Invalid marker
constexpr auto 
  EAST = Dir(1, 0),
  WEST = Dir(-1, 0),
  SOUTH = Dir(0, 1),
  NORTH = Dir(0, -1);


Result solvePartOne(const string &input) {
  auto grid = parseInput(input);
  auto h = grid.size(), w = grid[0].size();

  const auto moves = vector{EAST, WEST, SOUTH, NORTH};

  auto successorsFn = [&grid, &moves](const State &state) {
    vector<State> successors{};

  // cout << "Successors of: " << state << "\n";
    for (auto d : moves) {
      auto p = state.position + d;
      if (!insideBoard(grid, p)) continue;
      // Can't turn back
      if (d == Dir(0, 0) - state.direction) continue;
      auto straightStepsTaken = 0;
      if (d == state.direction) {
        // Same direction, keep count of straight steps taken
        if (state.straightStepsTaken == 3) continue;
        straightStepsTaken = state.straightStepsTaken;
      }
      auto newState = State(p, d, ++straightStepsTaken, state.cost + grid[p.y][p.x]);
      // cout << "Adding: " << newState << "\n";
      successors.push_back(newState);
    }
    return successors;
  };

  auto isGoalFn = [h, w](const State &state) {
    return state.position.y == h - 1 && state.position.x == w - 1;
  };

  auto comparisonFn = [](const State &s1, const State &s2) {
    // Get the minimum cost
    return s1.cost > s2.cost;
  };

  State startEast{Pos{0, 0}, Dir{1, 0}, 1, 0};
  State costEast = greedySearch(startEast, successorsFn, isGoalFn, comparisonFn);
  State startSouth{Pos{0, 0}, Dir{0, 1}, 1, 0};
  State costSouth = greedySearch(startSouth, successorsFn, isGoalFn, comparisonFn);
  return min(costEast.cost, costSouth.cost);
}

Result solvePartTwo(const string &input) {
  auto grid = parseInput(input);
  auto h = grid.size(), w = grid[0].size();

  const auto moves = vector{EAST, WEST, SOUTH, NORTH};

  auto successorsFn = [&grid, &moves](const State &state) {
    vector<State> successors{};


    if (state.straightStepsTaken < 4) {
      auto p = state.position + state.direction;
      if (!insideBoard(grid, p)) return successors;
      auto newState = State(p, state.direction, state.straightStepsTaken + 1, state.cost + grid[p.y][p.x]);
      successors.push_back(newState);
      return successors;
    }

    for (auto d : moves) {
      auto p = state.position + d;
      if (!insideBoard(grid, p)) continue;
      // Can't turn back
      if (d == Dir(0, 0) - state.direction) continue;
      auto straightStepsTaken = 0;
      if (d == state.direction) {
        // Same direction, keep count of straight steps taken
        if (state.straightStepsTaken == 10) continue;
        straightStepsTaken = state.straightStepsTaken;
      }
      auto newState = State(p, d, ++straightStepsTaken, state.cost + grid[p.y][p.x]);
      // cout << "Adding: " << newState << "\n";
      successors.push_back(newState);
    }
    return successors;
  };

  auto isGoalFn = [h, w](const State &state) {
    return state.position.y == h - 1 && state.position.x == w - 1 && state.straightStepsTaken >= 4;
  };

  auto comparisonFn = [](const State &s1, const State &s2) {
    // Get the minimum cost
    return s1.cost > s2.cost;
  };

  State startEast{Pos{0, 0}, Dir{1, 0}, 1, 0};
  State costEast = greedySearch(startEast, successorsFn, isGoalFn, comparisonFn);
  State startSouth{Pos{0, 0}, Dir{0, 1}, 1, 0};
  State costSouth = greedySearch(startSouth, successorsFn, isGoalFn, comparisonFn);
  return min(costEast.cost, costSouth.cost);
}
} // namespace aoc17

template<>
struct std::hash<aoc17::State>
{
    std::size_t operator()(const aoc17::State &state) const noexcept {
      std::size_t seed = 0;
      aoc::hash_combine(seed, state.position.x);
      aoc::hash_combine(seed, state.position.y);
      aoc::hash_combine(seed, state.direction.x);
      aoc::hash_combine(seed, state.direction.y);
      aoc::hash_combine(seed, state.straightStepsTaken);
      return seed;
    }
};
