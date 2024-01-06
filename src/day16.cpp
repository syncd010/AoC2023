#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <format>

#include "aoc.h"
#include "utils.h"

namespace aoc16 {

using namespace std;
using namespace aoc;

using Pos = vec2<int>;
using Dir = vec2<int>;

// Directions
constexpr auto 
  NORTH = Dir(0, -1),
  SOUTH = Dir(0, 1),
  EAST = Dir(1, 0),
  WEST = Dir(-1, 0);

int64_t countEnergized(const vector<string_view> &grid, Pos startPos, Dir startDir) {
  auto h = (int)grid.size(), w = (int)grid[0].size();
  // char visitedGrid[h][w]{};
  auto visitedGrid = vector(h, vector<char>(w, 0));
  // Keep a list of rays currently traveling
  auto rays = vector<pair<Pos, Dir>>{ make_pair(startPos, startDir) };
  auto energized = int64_t{ 0 };
  while (!rays.empty()) {
    auto [pos, dir] = rays.back();
    rays.pop_back();
    // Bit vector with different bits set for different directions
    char dirBits = 1 << ((dir.x + 1) * 2 + (dir.y + 1));

    // Outside board or already visited in the same direction
    if (!inBounds(pos.x, 0, w) || !inBounds(pos.y, 0, h) || (visitedGrid[pos.y][pos.x] & dirBits)) 
      continue;

    // Follow empty spaces, in the same dir
    while (inBounds(pos.x, 0, w) && inBounds(pos.y, 0, h)) {
      auto c = grid[pos.y][pos.x];
      // Count energized if not visited and mark visited
      if (!visitedGrid[pos.y][pos.x]) energized++;
      visitedGrid[pos.y][pos.x] |= dirBits;

      // Keep going if possible
      if (c == '.' || (c == '-' && dir.y == 0) || (c == '|' && dir.x == 0)) {
        pos += dir;
      } else {
        break;
      }
    }

    if (!inBounds(pos.x, 0, w) || !inBounds(pos.y, 0, h)) continue;
    auto c = grid[pos.y][pos.x];
    // We're at a junction, change direction of ray
    if (c == '\\') {
      auto newDir = Dir(dir.y, dir.x);
      rays.push_back(make_pair(pos + newDir, newDir));
    } else if (c == '/') {
      auto newDir = Dir(-dir.y, -dir.x);
      rays.push_back(make_pair(pos + newDir, newDir));
    } else if (c == '|') {
      auto newDir1 = Dir(0, 1), newDir2 = Dir(0, -1);
      rays.push_back(make_pair(pos + newDir1, newDir1));
      rays.push_back(make_pair(pos + newDir2, newDir2));
    } else if (c == '-') {
      auto newDir1 = Dir(1, 0), newDir2 = Dir(-1, 0);
      rays.push_back(make_pair(pos + newDir1, newDir1));
      rays.push_back(make_pair(pos + newDir2, newDir2));
    }
  }
  return energized;
}

Result solvePartOne(const string &input) {
  auto grid = toVector(input | splitString('\n'));
  return countEnergized(grid, Pos{0, 0}, EAST);
}


Result solvePartTwo(const string &input) {
  auto grid = toVector(input | splitString('\n'));

  // Entry points along border
  auto entries = vector<pair<Pos, Dir>>{};
  int h = grid.size(), w = grid[0].size();
  for (int y = 0; y < h; y++) {
    entries.push_back(make_pair(Pos{0, y}, EAST));
    entries.push_back(make_pair(Pos{w - 1, y}, WEST));
  }
  for (int x = 0; x < w; x++) {
    entries.push_back(make_pair(Pos{x, 0}, SOUTH));
    entries.push_back(make_pair(Pos{x, h - 1}, NORTH));
  }

  return foldLeft(entries, (int64_t)0, [&grid](auto m, auto entry) { 
      return max(m, countEnergized(grid, entry.first, entry.second)); 
    });
}
} // namespace aoc16
