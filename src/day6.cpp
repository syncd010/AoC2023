#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <cmath>

#include "aoc.h"
#include "utils.h"

namespace aoc6 {

using namespace std;
using namespace aoc;

Result solvePartOne(const string &input) {
  auto params = toVector(input 
    | splitString('\n')
    | views::transform([](auto line) {
      return toVector(line.substr(line.find(":") + 1) | splitNumbers(' '));
    }));

  // Distance is given by d(t) = t(T-t), so the solution is d(t) > D, with T 
  // and D given in the input. Apply quadratic formula to get (minT, maxT)
  // Solutions are the integers in the interval (minT, maxT)
  auto ret = 1;
  for (int i = 0; i < params[0].size(); i++) {
    auto T = params[0][i], D = params[1][i];
    auto minT = (T - sqrt(T*T - 4*D)) / 2,
      maxT = (T + sqrt(T*T - 4*D)) / 2;
      ret *= ceil(maxT) - floor(minT) - 1;
  }

  return ret;
}

Result solvePartTwo(const string &input) {
  auto params = toVector(input 
    | splitString('\n')
    | views::transform([](auto line) {
      auto lineNumbers = line | views::filter(::isdigit);
      return ston<int64_t>(string(lineNumbers.begin(), lineNumbers.end()));
    }));
  auto T = params[0], D = params[1];
  auto minT = (T - sqrt(T*T - 4*D)) / 2,
    maxT = (T + sqrt(T*T - 4*D)) / 2;
  return (int)ceil(maxT) - (int)floor(minT) - 1;
}
} // namespace aoc6
