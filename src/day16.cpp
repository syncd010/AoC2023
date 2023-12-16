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

using Pos = Vector2<int>;
using Dir = Vector2<int>;

// Directions and Invalid marker
constexpr auto 
  NORTH = Dir(0, -1),
  SOUTH = Dir(0, 1),
  EAST = Dir(1, 0),
  WEST = Dir(-1, 0);

int64_t countEnergized(const vector<string_view> &grid, Pos startPos, Dir startDir) {
  // Direction changes map
  const unordered_map<char, unordered_map<Dir, vector<Dir>, Vector2Hash<int>>> dirChanges = {
    {'|', { { NORTH, { NORTH } }, { SOUTH, { SOUTH } }, { EAST, { NORTH, SOUTH } }, { WEST, { NORTH, SOUTH } } } },
    {'-', { { NORTH, { EAST, WEST } }, { SOUTH, { EAST, WEST } }, { EAST, { EAST } }, { WEST, { WEST } } } },
    {'\\', { { NORTH, { WEST } }, { SOUTH, { EAST } }, { EAST, { SOUTH } }, { WEST, { NORTH } } } },
    {'/', { { NORTH, { EAST } }, { SOUTH, { WEST } }, { EAST, { NORTH } }, { WEST, { SOUTH } } } },
    {'.', { { NORTH, { NORTH } }, { SOUTH, { SOUTH } }, { EAST, { EAST } }, { WEST, { WEST } } } },
  };

  // This keeps for each cell the directions it has been traveled
  auto gridDirs = vector(grid.size(), vector(grid[0].size(), vector<Dir>{}));

  // Keep a list of rays currently traveling
  vector<pair<Pos, Dir>> rays{ make_pair(startPos, startDir) };
  while (!rays.empty()) {
    auto [pos, dir] = rays.back();
    rays.pop_back();

    // Check if cell already visited in the same direction
    auto currCell = gridDirs[pos.y][pos.x];
    if (find(currCell.begin(), currCell.end(), dir) != currCell.end()) continue;

    // Handle empty spaces, following the same dir
    while (insideBoard(grid, pos) && (grid[pos.y][pos.x] == '.')) {
      gridDirs[pos.y][pos.x].push_back(dir);
      pos = pos + dir;
    }
    if (!insideBoard(grid, pos)) continue;
    // We are at a junction, get new dirs and generate new rays
    gridDirs[pos.y][pos.x].push_back(dir);
    vector<Dir> newDirs = dirChanges.at(grid[pos.y][pos.x]).at(dir);
    for (auto d : newDirs) {
      Pos newPos = pos + d;
      if (insideBoard(grid, newPos)) {
        rays.push_back(make_pair(newPos, d));
      }
    }
  }

  int64_t energized = 0;
  for (int y = 0; y < gridDirs.size(); y++) {
    for (int x = 0; x < gridDirs[y].size(); x++) {
      energized += (gridDirs[y][x].size() > 0);
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
  for (int y = 0; y < grid.size(); y++) {
    entries.push_back(make_pair(Pos{0, y}, EAST));
    entries.push_back(make_pair(Pos{0, (int)grid.size() - y - 1}, WEST));
  }
  for (int x = 0; x < grid[0].size(); x++) {
    entries.push_back(make_pair(Pos{x, 0}, SOUTH));
    entries.push_back(make_pair(Pos{(int)grid[0].size() - x - 1, 0}, NORTH));
  }

  return accumulate(entries.begin(), entries.end(), (int64_t)0, 
    [&grid](auto m, auto entry) { return max(m, countEnergized(grid, entry.first, entry.second)); });
}
} // namespace aoc16
