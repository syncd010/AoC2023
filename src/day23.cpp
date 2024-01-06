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
#include <deque>
#include <format>

#include "aoc.h"
#include "utils.h"

namespace aoc23 {

using namespace std;
using namespace aoc;

constexpr char WALL = '#', EMPTY = '.';

using Pos = vec2<int>;
using Dir = vec2<int>;
using PosHash = vec2Hash<int>;

constexpr auto NORTH = Dir(0, -1), SOUTH = Dir(0, 1), EAST = Dir(1, 0), WEST = Dir(-1, 0);

// Transforms the grid to a graph representation
auto gridToGraph(const vector<string> &grid, Pos startPos, Pos goalPos) {
  const auto dirs = vector{EAST, WEST, SOUTH, NORTH};
  const auto forbidden = unordered_map<char, Pos>{ { '>', WEST }, { '<', EAST }, { 'v', NORTH }, { '^', SOUTH } };
  auto validPos = [&forbidden](const vector<string> &grid, Pos pos, Dir dir) {
    return insideBoard(grid, pos) &&
      ((grid[pos.y][pos.x] == EMPTY) ||
      (forbidden.contains(grid[pos.y][pos.x]) && (dir != forbidden.at(grid[pos.y][pos.x]))));
  };

  // Returns the junctions reachable from a given position on the grid
  auto successors = [&grid, &dirs, &validPos](const Pos &origin) {
    auto succ = vector<pair<Pos, int>>{};
    // Follow maze for each direction, starting on origin, until a junction found
    for (auto pathDir : dirs) {
      auto pos = origin + pathDir;
      if (!validPos(grid, pos, pathDir)) continue;
      for (int steps = 1;;steps++) {
        auto possibleDirs = toVector(dirs 
          | views::filter([&grid, &validPos, &pos, &pathDir](Dir d) {
              return (d != -pathDir) && validPos(grid, pos + d, d);
            }));
        if (possibleDirs.size() != 1) {   // Junction found
          succ.push_back(make_pair(pos, steps));
          break;
        }
        pathDir = possibleDirs.front();
        pos += pathDir;
      }
    }
    return succ;
  };

  // Usual exploration, building the graph and translating Positions to ints
  auto posToIdx = unordered_map<Pos, int, PosHash>{ { startPos, 0 }, { goalPos, 1 } };
  auto idxToPos = vector<Pos>{ startPos, goalPos };
  auto frontier = deque<int>{ posToIdx[startPos] };
  auto visited = unordered_set<int>{ };
  auto graph = vector<vector<pair<int, int>>>{ { }, { } };   // Initial elements: startPos, goalPos
  while (!frontier.empty()) {
    auto current = frontier.front();
    frontier.pop_front();
    visited.insert(current);
    graph[current] = toVector(
      successors(idxToPos[current]) 
      | views::transform([&posToIdx, &idxToPos, &graph](auto p) {
          // Translate Positions to ints
          auto [pos, steps] = p;
          if (!posToIdx.contains(pos)) {
            posToIdx[pos] = posToIdx.size();
            idxToPos.push_back(pos);
            graph.push_back({});
          }
          return make_pair(posToIdx[pos], steps);
        }));
    for (const auto &[pos, steps] : graph[current]) {
      if (!visited.contains(pos) && (pos != posToIdx[goalPos])) {
          frontier.push_back(pos);
      }
    }
  }

  // Build adjacency matrix
  auto adjMatrix = vector(graph.size(), vector(graph.size(), 0));
  for (int i = 0; i < graph.size(); i++) {
    for (auto [j, v] : graph[i]) {
      adjMatrix[i][j] = max(adjMatrix[i][j], v);
    }
  }

  return make_tuple(graph, adjMatrix, posToIdx, idxToPos);
}

// Represents a path. Stores the last node, steps taken, 
// visited nodes as bitset and the max possible remaining steps
struct Path {
  int lastNode;
  int steps;
  uint64_t visited;
  int maxRemainingSteps;
};

int64_t longestPath(const vector<string> &grid) {
  auto startPos = Pos{1, 0}, 
       goalPos = Pos{(int)grid[0].size() - 2, (int)grid.size() - 1 };
  auto [graph, adjMatrix, posToIdx, idxToPos] = gridToGraph(grid, startPos, goalPos);
  auto start = posToIdx[startPos], goal = posToIdx[goalPos];

  // Max possible steps through the graph
  auto maxPossibleSteps = 0;
  for (int i = 0; i < adjMatrix.size(); i++) {
    for (int j = 0; j < i; j++) {
      maxPossibleSteps += max(adjMatrix[i][j], adjMatrix[j][i]);
    }
  }

  // Typical DFS to generate all paths
  auto frontier = deque<Path>{Path(start, 0, 0, maxPossibleSteps) };
  auto maxSteps = 0;
  while (!frontier.empty()) {
    const auto current = frontier.front();
    frontier.pop_front();
    if (current.lastNode == goal) {
      maxSteps = max(maxSteps, current.steps);
      continue;
    }
    // If current best if >= maximum possible best, ignore this
    if (current.steps + current.maxRemainingSteps <= maxSteps) continue;

    // Unvisited nodes from the current one
    auto unvisitedNodes = graph[current.lastNode] | 
      views::filter([&current](const auto &p) { return !(current.visited & (1L << p.first)); });

    // Sum steps from unvisited nodes
    auto stepsOnCurrent = 0;
    for (auto [nextNode, nextSteps] : unvisitedNodes ) {
      stepsOnCurrent += adjMatrix[nextNode][current.lastNode];
    }

    for (auto [nextNode, nextSteps] : unvisitedNodes ) {
      // New path, update steps taken, visited and max remaining steps 
      // by subtracting all steps from unvisited nodes to the current one
      Path newPath{nextNode, current.steps + nextSteps, current.visited | (1L << nextNode), current.maxRemainingSteps - stepsOnCurrent};
      frontier.push_front(newPath);
    }
  }
  return maxSteps;
}

Result solvePartOne(const string &input) {
  auto grid = toVector(input | splitString<string>('\n'));
  return longestPath(grid);
}

Result solvePartTwo(const string &input) {
  auto grid = toVector(input 
    | splitString<string>('\n')
    | views::transform([](string line) {
      replace_if(line.begin(), line.end(), [](char c) { return (c == '<' || c == '>' || c == 'v' || c == '^'); }, '.');
      return line;
    }));
  return longestPath(grid);
}
} // namespace aoc23
