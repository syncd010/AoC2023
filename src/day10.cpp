#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <cmath>
#include <utility>

#include "aoc.h"
#include "utils.h"

namespace aoc10 {

using namespace std;
using namespace aoc;

using Position = Vector2<int>;

// Directions and Invalid marker
constexpr auto 
  NORTH = Position(0, -1),
  SOUTH = Position(0, 1),
  EAST = Position(-1, 0),
  WEST = Position(1, 0),
  INVALID = Position(-1, -1);

// Inverts a position, useful for entry/exit switches on cells
inline Position rotate180(Position &pos) { return Position(pos.x * -1, pos.y * -1); }

// Returns the Start 'S' position on the maze
Position findStartPos(const auto &maze) {
  for (int y = 0; y < maze.size(); y++) {
    for (int x = 0; x < maze[y].size(); x++) {
      if (maze[y][x] == 'S') {
        return Position{x, y};
      }
    }
  }
  return INVALID;
}

// Finds the biggest loop on the maze
vector<Position> findMaxLoop(const auto &maze) {
  const vector<Position> dirs{ EAST, WEST, NORTH, SOUTH };
  // Exit points for each cell type
  const unordered_map<char, pair<Position, Position>> exits = {
    {'|', { NORTH, SOUTH} },
    {'-', { EAST, WEST} },
    {'J', { EAST, NORTH} },
    {'L', { WEST, NORTH} },
    {'F', { WEST, SOUTH} },
    {'7', { EAST, SOUTH} },
  };

  Position startPos = findStartPos(maze);
  vector<Position> maxLoop{};
  // Try each direction from the start position
  for (auto d : dirs) {
    auto entryDir = rotate180(d);
    auto currPos = startPos + d;
    if (!insideBoard(maze, currPos)) continue;

    // Follow maze until returning to start position
    vector<Position> currLoop{currPos};
    while (currPos != startPos) {
      // Handle dead ends '.'
      if (!exits.contains(maze[currPos.y][currPos.x])) break;

      const pair<Position, Position> &dirs = exits.at(maze[currPos.y][currPos.x]);
      // Exit dir is the opposite of entry dir
      Position exitDir = 
        (entryDir == dirs.first) ? dirs.second : 
          (entryDir == dirs.second) ? dirs.first : INVALID;
      // Break if no suitable exit found, this cell isn't a continuation of the previous one
      if (exitDir == INVALID) break;

      // Set up next pos and dir, save curr pos
      auto nextPos = currPos + exitDir;
      if (!insideBoard(maze, nextPos)) break;
      currPos = nextPos;
      entryDir = rotate180(exitDir);
      currLoop.push_back(currPos);
    }
    if (currPos == startPos && currLoop.size() > maxLoop.size()) {
      // New max loop
      maxLoop = currLoop;
    }
  }
  return maxLoop;
}

Result solvePartOne(const string &input) {
  auto rg = splitStringBy<string>(input, '\n');
  auto maze = vector(rg.begin(), rg.end());

  auto maxLoop = findMaxLoop(maze);
  return (int64_t)ceil(maxLoop.size() / 2);
}


Result solvePartTwo(const string &input) {
  auto rg = splitStringBy<string>(input, '\n');
  auto maze = vector(rg.begin(), rg.end());

  auto maxLoop = findMaxLoop(maze);
  if (maxLoop.size() == 0) return monostate();

  // Create a maze copy with just the main loop
  int h = maze.size(), w = maze[0].size();
  auto scratch = vector(h, string(w, '.'));
  for (auto p : maxLoop) {
    scratch[p.y][p.x] = maze[p.y][p.x];
  }

  // Deduce the start position character
  const unordered_map<char, pair<Position, Position>> exits = {
    {'|', { NORTH, SOUTH} },
    {'-', { EAST, WEST} },
    {'J', { EAST, NORTH} },
    {'L', { WEST, NORTH} },
    {'F', { WEST, SOUTH} },
    {'7', { EAST, SOUTH} },
  };
  auto startPos = maxLoop.back();
  auto startExitDirs = make_pair(maxLoop[maxLoop.size() - 2] - startPos, maxLoop.front() - startPos);
  char startChar;
  for (auto e : exits) {
    if ((e.second.first == startExitDirs.first && e.second.second == startExitDirs.second) ||
        (e.second.second == startExitDirs.first && e.second.first == startExitDirs.second)) {
      startChar = e.first;
      break;
    }
  }
  scratch[startPos.y][startPos.x] = startChar;

  // Count inside cells. Loop on lines and when a wall is found invert the 
  // inside indicator. Only invert on | and one of the corner pairs [F, 7] or
  // [L, J] otherwise we could be double inverting the indicator 
  bool in = false;
  int64_t inCount = 0;
  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      switch (scratch[y][x]) {
      case '|': case 'F': case '7':
        in = !in;
        break;
      case '.':
        inCount += in;
        break;
      }
      // scratch[y][x] = (in && scratch[y][x] == '.') ? 'I' : scratch[y][x];
    }
  }

  return inCount;
}

} // namespace aoc10
