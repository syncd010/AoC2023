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

namespace aoc15 {

using namespace std;
using namespace aoc;

int64_t dayHash(const string_view s) {
  return accumulate(s.begin(), s.end(), (int64_t)0, [](auto prev, auto c) { return (c == '\n') ? prev : ((prev + c) * 17 % 256); });
}

Result solvePartOne(const string &input) {
  auto steps = input
    | splitString(',')
    | views::transform(dayHash);
  return accumulate(steps.begin(), steps.end(), (int64_t)0);
}

// Returns the components of the given step
inline tuple<string_view, char, int>destructure(const string_view step) {
  size_t idx = 0;
  for (idx = 0; idx < step.size(); idx++) 
    if (step[idx] == '-' || step[idx] == '=') break;
  return make_tuple(step.substr(0, idx), step[idx], (idx + 1 >= step.size()) ? 0 : ston<int>(step.substr(idx + 1)));
}

Result solvePartTwo(const string &input) {
  auto steps = input
    | splitString(',');
  auto boxes = vector<vector<pair<string_view, int>>>(256);

  for (auto s : steps) {
    auto [label, op, val] = destructure(s);
    auto &box = boxes[dayHash(label)];
    auto it = ranges::find_if(box, [&label](auto p) { return p.first == label; });
    if (op == '=') {
      if (it != box.end()) {
        it->second = val;
      } else {
        box.push_back(make_pair(label, val));
      }
    } else if (it != box.end()) {
        box.erase(it);
    }
  }

  auto res = int64_t{ 0 };
  for (int i = 0; i < boxes.size(); i++) {
    for (int j = 0; j < boxes[i].size(); j++) {
      res += (i+1) * (j+1) * boxes[i][j].second;
    }
  }

  return res;
}
} // namespace aoc15
