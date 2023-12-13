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

using Position = Vector2<int64_t>;

constexpr int64_t EXP_FACTOR = 100;

vector<int64_t> findEmptyRows(const auto &board) {
  vector<int64_t> empty{};
  for (size_t i = 0; i < board.size(); i++) {
    if (ranges::all_of(board[i], [](char c) { return c == '.';})) {
      empty.push_back(i);
    }
  }
  return empty;
}

Result solve(const string &input, int64_t expandFactor = 1) {
  auto rg = splitStringBy<string>(input, '\n');
  auto board = vector(rg.begin(), rg.end());
  size_t h = board.size(), w = board[0].size();

  // Find empty rows and columns
  vector<int64_t> emptyRows = findEmptyRows(board), emptyCols = findEmptyRows(transposeBoard(board));

  // Find positions
  vector<Position> positions{};
  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      if (board[y][x] == '#') positions.push_back(Position(x, y));
    }
  }

  // Expand positions with the empty rows and columns
  auto expandedPositions = positions
    | views::transform([&emptyRows, &emptyCols, expandFactor](auto p) {
      auto expanded = p;
      for (auto n: emptyRows) if (p.y > n) expanded.y += expandFactor;
      for (auto n: emptyCols) if (p.x > n) expanded.x += expandFactor;
      return expanded;
    });

  // Calculate Manhattan Distance between pairs of positions
  int64_t res = 0;
  for (size_t i = 0; i < expandedPositions.size() - 1; i++) {
    for (size_t j = i+1; j < expandedPositions.size(); j++) {
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
