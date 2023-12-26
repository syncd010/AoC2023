#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "aoc.h"
#include "utils.h"

namespace aoc3 {

using namespace std;
using namespace aoc;

vector<int> findNums(const string &input, bool partTwoRules = false) {
  auto engine = toVector(input | splitString('\n'));
  int w = engine[0].size(), h = engine.size();
  vector<int> res{};

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (isdigit(engine[y][x]) || engine[y][x] == '.' || (partTwoRules && engine[y][x] != '*'))
        continue;

      // Visit adjacent cells, finding values
      vector<int> adjacent{};
      for (int dy = -1; dy <= 1; dy++) {
        if (y+dy < 0 || y + dy >= h) continue;
        for (int dx = -1; dx <= 1; dx++) {
          if (x+dx < 0 || x + dx >= w) continue;
          if (!isdigit(engine[y+dy][x+dx])) continue;
          // Found a digit, get it
          int idx = x + dx, n = 0;
          while (idx - 1 >= 0 && isdigit(engine[y+dy][idx - 1])) idx--;
          while (idx < w && isdigit(engine[y+dy][idx])) {
            n = n*10 + (engine[y+dy][idx] - '0');
            idx++;
          }
          adjacent.push_back(n);
          dx = idx - 1 - x;
        }
      }
      if (partTwoRules && adjacent.size() != 2) continue;
      res.insert(res.end(), adjacent.begin(), adjacent.end());
    }
  }
  return res;
}

Result solvePartOne(const string &input) {
  auto nums = findNums(input);
  return accumulate(nums.begin(), nums.end(), 0);
}

Result solvePartTwo(const string &input) {
  auto nums = findNums(input, true);
  // Accumulate pairs
  int res = 0;
  for (int i = 0; i < nums.size(); i += 2) {
    res += nums[i] * nums[i+1];
  }
  return res;
}
} // namespace aoc1
