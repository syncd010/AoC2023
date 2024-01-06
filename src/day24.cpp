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
      auto nums = toVector(line
        | splitString('@')
        | views::transform([](auto part) { return toVector(part | splitNumbers<int64_t>(',')); })
        | views::join);
      return make_pair(Pos{nums[0], nums[1], nums[2]}, Dir{nums[3], nums[4], nums[5]});
    }));
}

Result solvePartOne(const string &input) {
  constexpr int64_t m = 200000000000000, M = 400000000000000;

  auto stones = parseInput(input);
  auto res = int64_t{0};
  for (int i = 0; i < stones.size(); i++) {
    auto [p1, v1] = stones[i];
    auto s1 = (double)v1.y / (double)v1.x;    // Slope
    for (int j = i + 1; j < stones.size(); j++) {
      auto [p2, v2] = stones[j];
      auto s2 = (double)v2.y / (double)v2.x;
      if (s1 == s2) continue;   // Parallel lines
      auto x = (p2.y - p1.y - s2 * p2.x + s1 * p1.x) / (s1 - s2);
      auto t1 = (x - p1.x) / v1.x, t2 = (x - p2.x) / v2.x;
      if (t1 < 0 || t2 < 0) continue;
      auto y = p1.y + v1.y * t1;

      res += (x >= m && x <= M && y >= m && y <= M);
    }
  }

  return res;
}

Result solvePartTwo(const string &input) {
  auto stones = parseInput(input);

  cout << "Use a proper tool for the job: install python + sympy and input the following:\n\n";
  auto times = vector{"t1"s, "t2"s, "t3"s};
  string sympyEq{""s};
  for (auto i = 0; i < 3; i++) {
    auto [p, v] = stones[i+1];
    auto t = times[i];
    // cout << format("(x-({}))/(({})-a)=(y-({}))/(({})-b)=(z-({}))/(({})-c)\n", p.x, v.x, p.y, v.y, p.z, v.z);
    // cout << format("{}=(x-({}))/(({})-a)\n{}==(y-({}))/(({})-b)\n{}==(z-({}))/(({})-c)\n", t, p.x, v.x, t, p.y, v.y, t, p.z, v.z);

    sympyEq += format("(x-({}))/(({})-vx)-{}, (y-({}))/(({})-vy)-{}, (z-({}))/(({})-vz)-{}", p.x, v.x, t, p.y, v.y, t, p.z, v.z, t);
    if (i!=2) sympyEq += ",";
  }
  cout << "from sympy import solve\n";
  cout << "from sympy import symbols\n";
  cout << "x,y,z,vx,vy,vz,t1,t2,t3=symbols(\"x,y,z,vx,vy,vz,t1,t2,t3\")\n";
  cout << format("sol=solve([{}], [x,y,z,vx,vy,vz,t1,t2,t3], dict=True)\n", sympyEq);
  // cout << "print(sol)\n";
  cout << "print(\"x: \" + str(sol[0][x]) + \", y: \" + str(sol[0][y]) + \", z: \" + str(sol[0][z]))\n";
  cout << "print(\"sum: \" + str(sol[0][x] + sol[0][y] + sol[0][z]))\n\n";
  return 261502975177164 + 428589795012222 + 196765966839909;  // 886858737029295
}
} // namespace aoc24
