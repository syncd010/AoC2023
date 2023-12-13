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

namespace aoc13 {

using namespace std;
using namespace aoc;

// Checks whether the strings are different by a single position
// returning the position where they differ if so
int diffsBySinglePos(const string &str1, const string &str2) {
  if (str1.size() != str2.size()) return -1;
  int diff = -1;
  for (int i = 0; i < str1.size(); i++) {
    if (str1[i] != str2[i]) {
      if (diff != -1) return -1;  // Previously found a diff
      diff = i;
    }
  }
  return diff;
}

// Tries to find a reflection row on the pattern.
// If allowSingleDiffs = true, allows for single character changes
pair<int, Vector2<int>> findReflectionRow(const vector<string> &pattern, bool allowSingleDiffs = false) {
  int h = pattern.size();
  for (int row = 1; row < h; row++) {
    bool reflected = true;
    Vector2 diffPos(-1, -1);
    // Check if rows are reflected around current one
    for (int i = 0; i < min(row, h - row); i++) {
      if (pattern[row + i] != pattern[row - i - 1]) {
        // Different row, no reflection in principle, but check for single diffs rules
        if (allowSingleDiffs && (diffPos.x == -1)) {
          // Single diffs allowed and no single diff previously detected. 
          // Check if this row has a single diff
          int diffCol = diffsBySinglePos(pattern[row + i], pattern[row - i - 1]);
          if (diffCol != -1) {
            diffPos = Vector2(diffCol, row-i-1);
            continue;
          }
        }
        reflected = false;
        break;
      }
    }
    if (reflected && (!allowSingleDiffs || diffPos.x != -1)) 
      return make_pair(row, diffPos);
  }
  return make_pair(-1, Vector2(-1, -1));
}


Result solve(const string &input, bool allowSingleDiffs = false) {
  auto rg = splitStringBy<string_view, string>(input, "\n\n")
    | views::transform([](auto pattern) {
        auto splits = splitStringBy<string>(pattern, '\n');
        return vector(splits.begin(), splits.end());
      })
    | views::transform([allowSingleDiffs](auto pattern) {
        auto [row, _ignore1] = findReflectionRow(pattern, allowSingleDiffs);
        auto [col, _ignore2] = findReflectionRow(transposeBoard(pattern), allowSingleDiffs);
        return (row != -1) ? row * 100 : col;
    });

  return accumulate(rg.begin(), rg.end(), (int64_t)0);
}

Result solvePartOne(const string &input) {
  return solve(input, false);
}

Result solvePartTwo(const string &input) {
  return solve(input, true);
}
} // namespace aoc13
