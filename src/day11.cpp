#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "aoc.h"
#include "utils.h"

namespace aoc11 {

using namespace std;
using namespace aoc;

using Pos = vec2<int64_t>;

constexpr int64_t EXP_FACTOR = 100;

vector<int64_t> findEmptyRows(const auto &board) {
  auto empty = vector<int64_t>{};
  for (auto i = 0; i < board.size(); i++) {
    if (ranges::all_of(board[i], [](char c) { return c == '.';})) {
      empty.push_back(i);
    }
  }
  return empty;
}

Result solve(const string &input, int64_t expandFactor = 1) {
  auto board = toVector(input | splitString('\n'));
  auto h = board.size(), w = board[0].size();
  // Find empty rows and columns
  auto emptyRows = findEmptyRows(board), 
       emptyCols = findEmptyRows(transposeBoard(board));

  // Find positions
  auto positions = vector<Pos>{};
  for (auto y = 0; y < h; y++) {
    for (auto x = 0; x < w; x++) {
      if (board[y][x] == '#') positions.push_back(Pos(x, y));
    }
  }

  // Expand positions with the empty rows and columns
  // Note: making this a vector instead of a range is 10x faster later on
  auto expandedPositions = toVector(positions
    | views::transform([&emptyRows, &emptyCols, expandFactor](auto p) {
      auto expanded = p;
      for (auto n: emptyRows) if (p.y > n) expanded.y += expandFactor;
      for (auto n: emptyCols) if (p.x > n) expanded.x += expandFactor;
      return expanded;
    }));

  // Calculate Manhattan Distance between pairs of positions
  auto res = int64_t{0};
  for (auto i = 0; i < expandedPositions.size() - 1; i++) {
    for (auto j = i+1; j < expandedPositions.size(); j++) {
      res += expandedPositions[i].manhattanDistanceTo(expandedPositions[j]);
    }
  }

  return res;
}

Result solvePartOne(const string &input) {
  return solve(input, 1);
}

Result solvePartTwo(const string &input) {
  return solve(input, 999999);
}
} // namespace aoc11
