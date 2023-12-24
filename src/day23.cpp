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
auto gridToGraph(const vector<string> &grid) {
  int h = grid.size(), w = grid[0].size();
  const auto dirs = vector{EAST, WEST, SOUTH, NORTH};
  const auto forbidden = unordered_map<char, Pos>{ { '>', WEST }, { '<', EAST }, { 'v', NORTH }, { '^', SOUTH } };

  auto successors = [&grid, &dirs, &forbidden](const Pos &origin) {
    vector<pair<Pos, int>> succ{};

    for (auto pathDir : dirs) {
      auto pos = origin + pathDir;
      if (!insideBoard(grid, pos) ||
          (grid[pos.y][pos.x] == WALL) ||
          ((grid[pos.y][pos.x] != EMPTY) && (pathDir == forbidden.at(grid[pos.y][pos.x]))))
         continue;
      // if (!insideBoard(grid, pos) ||
      //     (grid[pos.y][pos.x] == WALL))
      //    continue;
      auto steps = 1;
      bool junctionFound = false;
      while (!junctionFound) {
        auto possibleDirs = toVector(dirs 
          | views::filter([&grid, &forbidden, &pos, &pathDir](Dir d) {
              auto proposedPos = pos + d;
              // return 
              //   (insideBoard(grid, proposedPos) && 
              //    (proposedPos != pos - pathDir) &&
              //    (grid[proposedPos.y][proposedPos.x] != WALL));
              return 
                (insideBoard(grid, proposedPos) && 
                (proposedPos != pos - pathDir) &&
                ((grid[proposedPos.y][proposedPos.x] == EMPTY) ||
                 ((grid[proposedPos.y][proposedPos.x] != WALL) &&
                  (d != forbidden.at(grid[proposedPos.y][proposedPos.x])))));
            }));
        junctionFound = !(possibleDirs.size() == 1);
        if (!junctionFound) {
          pathDir = possibleDirs.front();
          pos += pathDir;
          steps++;
        }
      }
      succ.push_back(make_pair(pos, steps));
    }
    return succ;
  };

  deque<Pos> frontier({Pos{ 1, 0 }});
  unordered_set<Pos, PosHash> explored;
  unordered_map<Pos, vector<pair<Pos, int>>, PosHash> graph{};
  while (!frontier.empty()) {
    auto current = frontier.front();
    frontier.pop_front();
    graph[current] = successors(current);
    explored.insert(current);
    for (auto [pos, steps] : graph[current]) {
      if (!explored.contains(pos)) {
        frontier.push_back(pos);
      }
    }
  }
  return graph;

  // Transform to 1D
  // unordered_map<int, vector<pair<int, int>>> idxGraph{};
  // for (const auto [pos, val] : graph) {
  //   vector<pair<int, int>> newVal{};
  //   for (auto [pos, steps] : val) {
  //     newVal.push_back(make_pair(pos.toIdx(w), steps));
  //   }
  //   idxGraph[pos.toIdx(w)] = newVal;
  // }
  // return idxGraph;
}

struct Path {
  int steps;
  vector<Pos> path;
};

int64_t longestPath(const vector<string> &grid) {
  Pos start{1, 0}, goal{ (int)grid[0].size() - 2, (int)grid.size() - 1 };
  int h = grid.size(), w = grid[0].size();
  auto graph = gridToGraph(grid);

  // cout << format("Graph has {} vertices\n", graph.size());

  // Generate all paths
  deque<Path> frontier({Path(0, { start }) });
  vector<Path> paths{};
  while (!frontier.empty()) {
    const auto current = frontier.front();
    frontier.pop_front();
    if (current.path.back() == goal) {
      paths.push_back(current);
      continue;
    }

    for (auto [nextPos, nextSteps] : graph[current.path.back()]) {
      // Detect loops
      if (find(current.path.begin(), current.path.end(), nextPos) != current.path.end()) {
        continue;
      }
      Path newPath{current};
      newPath.steps += nextSteps;
      newPath.path.push_back(nextPos);
      frontier.push_back(newPath);
    }
  }
  // cout << format("Explored {} paths\n", paths.size());

  return foldLeft(paths, 0, [](auto prev, auto s) { return max(prev, s.steps); });
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
