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

using Position = vec2<int>;

// Standard directions and invalid marker
constexpr auto 
  NORTH = Position(0, -1),
  SOUTH = Position(0, 1),
  EAST = Position(1, 0),
  WEST = Position(-1, 0);

constexpr char ROCK = 'O', CUBE = '#', EMPTY = '.';

vector<string> tiltBoard(const vector<string> &board, const Position dir) {
  int h = board.size(), w = board[0].size();
  auto newBoard = board;

  // Loop according to dir: starting from the start or end, so that we don't overwrite positions
  for (int y = (dir.y == 1) ? h - 1 : 0; y >= 0 && y < h ; y += (dir.y == 1) ? -1 : 1) {
    for (int x = (dir.x == 1) ? w - 1: 0; x >= 0 && x < w ; x += (dir.x == 1) ? -1 : 1) {
      if (newBoard[y][x] == ROCK) {
        Position pos = Position(x, y) + dir;
        // Loop until a cube is found or end reached
        while (insideBoard(newBoard, pos) && newBoard[pos.y][pos.x] == EMPTY) {
          pos += dir;
        }
        // Clear previous position, put rock on final position. Must be in sync with the loop direction
        newBoard[y][x] = EMPTY;
        newBoard[pos.y - dir.y][pos.x - dir.x] = ROCK;
      }
    }
  }
  return newBoard;
}

int64_t scoreBoard(const auto &board) {
  int h = board.size(), w = board[0].size();  
  int64_t res = 0;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      res += (board[y][x] == ROCK) * (h - y);
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

Result solvePartOne(const string &input) {
  auto rg = splitStringBy<string>(input, '\n');
  auto board = vector(rg.begin(), rg.end());

  return scoreBoard(tiltBoard(board, NORTH));
}

// Cache of boards and the loop index that generated them
static unordered_map<string, int>cache;

Result solvePartTwo(const string &input) {
  auto rg = splitStringBy<string>(input, '\n');
  auto board = vector(rg.begin(), rg.end());

  vector<Position> dirs{NORTH, WEST, SOUTH, EAST};
  int64_t steps = 1000000000 * dirs.size();
  bool cycleFound = false;
  for (int64_t i = 0, cycleIdx = 0; i < steps; i++, cycleIdx = (cycleIdx + 1) % dirs.size()) {
    board = tiltBoard(board, dirs[cycleIdx]);
    if (cycleFound) continue;   // We're just looping on the remainder steps
    auto key = flattenBoard(board);
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
