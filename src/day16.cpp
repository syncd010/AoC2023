#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "aoc.h"
#include "utils.h"

namespace aoc16 {

using namespace std;
using namespace aoc;

using Pos = vec2<int>;
using Dir = vec2<int>;

// Directions and Invalid marker
constexpr auto 
  NORTH = Dir(0, -1),
  SOUTH = Dir(0, 1),
  EAST = Dir(1, 0),
  WEST = Dir(-1, 0);

int64_t countEnergized(const vector<string_view> &grid, Pos startPos, Dir startDir) {
  // Direction changes map
  const unordered_map<char, unordered_map<Dir, vector<Dir>, vec2Hash<int>>> dirChanges = {
    {'|', { { NORTH, { NORTH } }, { SOUTH, { SOUTH } }, { EAST, { NORTH, SOUTH } }, { WEST, { NORTH, SOUTH } } } },
    {'-', { { NORTH, { EAST, WEST } }, { SOUTH, { EAST, WEST } }, { EAST, { EAST } }, { WEST, { WEST } } } },
    {'\\', { { NORTH, { WEST } }, { SOUTH, { EAST } }, { EAST, { SOUTH } }, { WEST, { NORTH } } } },
    {'/', { { NORTH, { EAST } }, { SOUTH, { WEST } }, { EAST, { NORTH } }, { WEST, { SOUTH } } } },
    {'.', { { NORTH, { NORTH } }, { SOUTH, { SOUTH } }, { EAST, { EAST } }, { WEST, { WEST } } } },
  };

  // This keeps for each cell the directions it has been traveled
  auto visited = vector(grid.size(), vector(grid[0].size(), vector<Dir>{}));

  // Keep a list of rays currently traveling
  vector<pair<Pos, Dir>> rays{ make_pair(startPos, startDir) };
  while (!rays.empty()) {
    auto [pos, dir] = rays.back();
    rays.pop_back();

    // Check if cell already visited in the same direction
    if (auto cell = visited[pos.y][pos.x];
        find(cell.begin(), cell.end(), dir) != cell.end()) 
        continue;

    // Handle empty spaces, following the same dir
    while (insideBoard(grid, pos) && (grid[pos.y][pos.x] == '.')) {
      visited[pos.y][pos.x].push_back(dir);
      pos += dir;
    }
    if (!insideBoard(grid, pos)) continue;
    // We are at a junction, get new dirs and generate new rays
    visited[pos.y][pos.x].push_back(dir);
    vector<Dir> newDirs = dirChanges.at(grid[pos.y][pos.x]).at(dir);
    for (auto d : newDirs) {
      if (Pos newPos = pos + d; insideBoard(grid, newPos)) {
        rays.push_back(make_pair(newPos, d));
      }
    }
  }

  int64_t energized = 0;
  for (int y = 0; y < visited.size(); y++) {
    for (int x = 0; x < visited[y].size(); x++) {
      energized += (visited[y][x].size() > 0);
    }
  }

  return energized;
}

Result solvePartOne(const string &input) {
  auto rg = splitStringBy(input, '\n');
  auto grid = vector(rg.begin(), rg.end());

  return countEnergized(grid, Pos{0, 0}, EAST);
}

Result solvePartTwo(const string &input) {
  auto rg = splitStringBy(input, '\n');
  auto grid = vector(rg.begin(), rg.end());

  vector<pair<Pos, Dir>> entries{};
  int h = grid.size(), w = grid[0].size();
  for (int y = 0; y < h; y++) {
    entries.push_back(make_pair(Pos{0, y}, EAST));
    entries.push_back(make_pair(Pos{w - 1, y}, WEST));
  }
  for (int x = 0; x < w; x++) {
    entries.push_back(make_pair(Pos{x, 0}, SOUTH));
    entries.push_back(make_pair(Pos{x, h - 1}, NORTH));
  }

  return accumulate(entries.begin(), entries.end(), (int64_t)0, 
    [&grid](auto m, auto entry) { return max(m, countEnergized(grid, entry.first, entry.second)); });
}
} // namespace aoc16
