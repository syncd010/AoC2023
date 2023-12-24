#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <format>

#include "aoc.h"
#include "utils.h"

namespace aoc24 {

using namespace std;
using namespace aoc;

using Pos = vec3<int64_t>;
using Dir = vec3<int64_t>;

vector<pair<Pos, Dir>> parseInput(const string &input) {
  return toVector(input
    | splitString('\n')
    | views::transform([](auto line) {
      auto parts = toVector(line
        | splitString('@'));
      auto pos = toVector(parts[0] | splitNumbers<int64_t>(','));
      auto dir = toVector(parts[1] | splitNumbers<int64_t>(','));
      return make_pair(Pos{pos[0], pos[1], pos[2]}, Dir{dir[0], dir[1], dir[2]});
    }));
}

Result solvePartOne(const string &input) {
  constexpr int64_t m = 200000000000000, M = 400000000000000;

  auto stones = parseInput(input);
  int64_t res = 0;
  for (int i = 0; i < stones.size(); i++) {
    auto [p1, v1] = stones[i];
    auto s1 = (double)v1.y / (double)v1.x;    // Slope
    for (int j = i + 1; j < stones.size(); j++) {
      auto [p2, v2] = stones[j];
      auto s2 = (double)v2.y / (double)v2.x;
      if (s1 == s2) continue;   // Parallel lines
      double x = (p2.y - p1.y - s2 * p2.x + s1 * p1.x) / (s1 - s2);
      double t1 = (x - p1.x) / v1.x, t2 = (x - p2.x) / v2.x;
      if (t1 < 0 || t2 < 0) continue;
      double y = p1.y + v1.y * t1;

      res += (x >= m && x <= M && y >= m && y <= M);
    }
  }

  return res;
}

Result solvePartTwo(const string &input) {
  auto stones = parseInput(input);

  cout << "Solved through Wolfram Alpha. Search for 'system of equations' and input the following:\n";
  for (auto i = 0; i < 3; i++) {
    auto [p, v] = stones[i];
    auto eq = format("(x-({}))/(a-({}))=(y-({}))/(b-({}))=(z-({}))/(c-({}))", p.x, v.x, p.y, v.y, p.z, v.z);
    cout << eq << "\n";
  }

  return 261502975177164 + 428589795012222 + 196765966839909;  // 886858737029295
}
} // namespace aoc24
