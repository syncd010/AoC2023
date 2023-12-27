#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <format>

#include "aoc.h"
#include "utils.h"

namespace aoc14 {

using namespace std;
using namespace aoc;

using Dir = vec2<int>;
using Pos = vec2<int>;

// Standard directions and invalid marker
constexpr auto 
  NORTH = Dir(0, -1),
  SOUTH = Dir(0, 1),
  EAST = Dir(1, 0),
  WEST = Dir(-1, 0);

constexpr char ROCK = 'O', CUBE = '#', EMPTY = '.';

vector<string> tiltBoard(vector<string> board, const Dir dir) {
  int h = board.size(), w = board[0].size();
  bool tiltHorizontal = (dir.y == 0);

  // Loop according to dir: starting from the start or end, so that we don't overwrite positions
  int dy = (dir.y == 1) ? -1 : 1, dx = (dir.x == 1) ? -1 : 1;
  int sy = (dir.y == 1) ? h - 2 : 1, sx = (dir.x == 1) ? w - 2: 1;
  // Save the last empty position for each row or column
  vector<Pos> lastEmpty(tiltHorizontal ? h : w);
  for (int i = 0; i < lastEmpty.size(); i++) {
    lastEmpty[i] = tiltHorizontal ? Pos(sx, i) : Pos(i, sy);
  }
  // Loop up/down
  for (int y = sy; y >= 0 && y < h; y += dy) {
    // Loop left/right
    for (int x = sx; x >= 0 && x < w; x += dx) {
      int emptyIdx = tiltHorizontal ? y : x;
      if (board[y][x] == CUBE) {
        // Update last empty position
        lastEmpty[emptyIdx].y = y - dir.y;
        lastEmpty[emptyIdx].x = x - dir.x;
      } else if (board[y][x] == ROCK) {
        // Clear and place rock on last empty position
        board[y][x] = EMPTY;
        board[lastEmpty[emptyIdx].y][lastEmpty[emptyIdx].x] = ROCK;
        lastEmpty[emptyIdx] -= dir;
      }

      // if (board[y][x] == ROCK) {
      //   Pos pos = Pos(x, y) + dir;
      //   // Loop until a cube is found or end reached
      //   while (board[pos.y][pos.x] == EMPTY) pos += dir;
      //   // Clear previous position, put rock on final position. Must be in sync with the loop direction
      //   board[y][x] = EMPTY;
      //   board[pos.y - dir.y][pos.x - dir.x] = ROCK;
      // }
    }
  }
  return board;
}

int64_t scoreBoard(const auto &board) {
  int h = board.size() - 2, w = board[0].size() - 2;
  int64_t res = 0;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      res += (board[y+1][x+1] == ROCK) * (h - y);
    }
  }
  return res;
}

// Flatten board to string, to be used as key in cache
inline string flattenBoard(const auto &board) {
  ostringstream flattened;
  copy(board.begin(), board.end(), ostream_iterator<string>(flattened));
  return flattened.str();
}

auto surroundBoard(const auto &board) {
  size_t h = board.size(), w = board[0].size();  
  vector<string> newBoard{h + 2, string(w + 2, '#')};
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      newBoard[y+1][x+1]= board[y][x];
    }
  }
  return newBoard;
}

Result solvePartOne(const string &input) {
  auto board = surroundBoard(toVector(input | splitString<string>('\n')));
  return scoreBoard(tiltBoard(board, NORTH));
}

Result solvePartTwo(const string &input) {
  auto board = surroundBoard(toVector(input | splitString<string>('\n')));
  // Cache of boards and the loop index that generated them
  unordered_map<string, int>cache;

  const vector<Dir> dirs{NORTH, WEST, SOUTH, EAST};
  const int sz = dirs.size();
  int64_t steps = (int64_t)1000000000 * sz;
  bool cycleFound = false;
  string key;
  for (int64_t i = 0; i < steps; i++) {
    board = tiltBoard(board, dirs[i % sz]);
    if (cycleFound) continue;   // We're just looping on the remainder steps
    key = flattenBoard(board);
    if (cache.contains(key)) {  // Cycle found, increase counter to remaining steps
      cycleFound = true;
      i = steps - (steps - cache[key]) % (i - cache[key]);
      continue;
    }
    cache[key] = i;
  }

  return scoreBoard(board);
}
} // namespace aoc14
