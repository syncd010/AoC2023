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

constexpr auto 
  NORTH = Position(0, -1),
  SOUTH = Position(0, 1),
  EAST = Position(-1, 0),
  WEST = Position(1, 0),
  INVALID = Position(-1, -1);

inline Position rotate180(Position &pos) { return Position(pos.x * -1, pos.y * -1); }

struct PositionHash {
    std::size_t operator () (const Position &p) const {
        return hash<int>{}(p.x) ^ (hash<int>{}(p.y) << 1);
    }
};

struct State {
  Position position, dir;
};

inline bool insideMaze(Position pos, const auto &maze) {
  int h = maze.size(), w = maze[0].size();
  return inBounds(pos.x, 0, w) && inBounds(pos.y, 0, h);
}

State next(const auto &maze, State state) {
  // The entry and exit points for each cell type
  static const unordered_map<char, pair<Position, Position>> flow = {
    {'|', { NORTH, SOUTH} },
    {'-', { EAST, WEST} },
    {'J', { EAST, NORTH} },
    {'L', { WEST, NORTH} },
    {'F', { WEST, SOUTH} },
    {'7', { EAST, SOUTH} },
  };

  char curr = maze[state.position.y][state.position.x];
  if (!flow.contains(curr)) return State{INVALID, INVALID};

  const pair<Position, Position> &entryDirs = flow.at(curr);

  Position exitDir = 
    (entryDirs.first == state.dir) ? entryDirs.second : 
      (entryDirs.second == state.dir) ? entryDirs.first : INVALID;

  if (exitDir != INVALID) {
    auto exitPosition = state.position + exitDir;
    if (insideMaze(exitPosition, maze)) {
      return State{exitPosition, rotate180(exitDir)};
    }
  }
  return State{INVALID, INVALID};
}

bool isEndPosition(const auto &maze, Position position) {
  return position != INVALID && insideMaze(position, maze) && 
    maze[position.y][position.x] == 'S';
}

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

pair<int64_t, Position> findMaxLoop(const auto &maze, Position startPos) {
  vector<Position> dirs{ EAST, WEST, NORTH, SOUTH };
  int64_t maxLen = 0;
  Position maxDir = INVALID;
  for (auto d : dirs) {
    auto state = State(startPos + d, rotate180(d));
    if (!insideMaze(state.position, maze)) state.position = INVALID;

    int64_t len = 0;
    while (state.position != INVALID && 
          !isEndPosition(maze, state.position)) {
      state = next(maze, state);
      len++;
    }
    if (state.position != INVALID && len > maxLen) {
      maxLen = len + 1;
      maxDir = d;
    }
  }
  return make_pair(maxLen, maxDir);
}

Result solvePartOne(const string &input) {
  auto rg = splitStringBy<string>(input, '\n');
  auto maze = vector(rg.begin(), rg.end());

  auto [maxLen, maxDir] = findMaxLoop(maze, findStartPos(maze));

  return (int64_t)ceil(maxLen / 2);
}

inline void displayMaze(auto &maze) {
  for (string_view l : maze) cout << l << "\n";
}

inline void markPosition(auto &maze, Position pos, char mark) {
  if (insideMaze(pos, maze) && maze[pos.y][pos.x] == '.') maze[pos.y][pos.x] = mark;
}

Result solvePartTwo(const string &input) {
  auto rg = splitStringBy<string>(input, '\n');
  auto maze = vector(rg.begin(), rg.end());
  auto startPos = findStartPos(maze);
  auto [maxLen, maxDir] = findMaxLoop(maze, startPos);
  int h = maze.size(), w = maze[0].size();

  auto scratch = vector(h, string(w, '.'));
  const unordered_map<Position, pair<Position, Position>, PositionHash> limits {
    {NORTH, {EAST, WEST}},
    {WEST, {NORTH, SOUTH}},
    {SOUTH, {WEST, EAST}},
    {EAST, {SOUTH, NORTH}},
  };
  auto state = State(startPos + maxDir, rotate180(maxDir));
  while (!isEndPosition(maze, state.position)) {
    scratch[state.position.y][state.position.x] = maze[state.position.y][state.position.x];
    State oldState = state;
    state = next(maze, state);
    auto m = limits.at(state.dir);
    markPosition(scratch, state.position + m.first, 'O');
    markPosition(scratch, state.position + m.second, 'I');

    if (oldState.dir != state.dir) {
      auto m = limits.at(state.dir);
      markPosition(scratch, oldState.position + m.first, 'O');
      markPosition(scratch, oldState.position + m.second, 'I');
    }
  }
  scratch[state.position.y][state.position.x] = maze[state.position.y][state.position.x];

  int res = 0;
  char inside = 'I';
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (scratch[y][x] == inside) {
        if (inBounds(x-1, 0, w) && scratch[y][x-1] == '.') scratch[y][x-1] = inside;
        if (inBounds(x+1, 0, w) && scratch[y][x+1] == '.') scratch[y][x+1] = inside;
        if (inBounds(y-1, 0, h) && scratch[y-1][x] == '.') scratch[y-1][x] = inside;
        if (inBounds(y+1, 0, h) && scratch[y+1][x] == '.') scratch[y+1][x] = inside;
      }
    }
  }
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (scratch[y][x] == inside) {
        res++;
      }
    }
  }
  // displayMaze(scratch);

  return res;
}
} // namespace aoc10
