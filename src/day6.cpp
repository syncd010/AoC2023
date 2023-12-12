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

int solvePartOne(const string &input) {
  auto parsed = splitStringBy(input, '\n')
    | views::filter([](auto line) { return !line.empty(); })
    | views::transform([](auto line) {
      return splitStringToNumbers<int>(line.substr(line.find(":")+1), ' ');
    });
  auto params = vector(parsed.begin(), parsed.end());

  // Function is quadratic x(T - x) = D, with T and D given in the input
  // Integer values > x- and < x+ are the solutions
  int ret = 1;
  for (int i = 0; i < params[0].size(); i++) {
    auto T = params[0][i], D = params[1][i];
    auto min_T = (T - sqrt(T*T - 4*D)) / 2,
      max_T = (T + sqrt(T*T - 4*D)) / 2;
      ret *= ceil(max_T) - floor(min_T) - 1;
  }

  return ret;
}

int solvePartTwo(const string &input) {
  auto parsed = splitStringBy(input, '\n')
    | views::filter([](auto line) { return !line.empty(); })
    | views::transform([](auto line) {
      auto line_n = line | views::filter(::isdigit);
      return cston<long>(string(line_n.begin(), line_n.end()));
    });
  auto params = vector(parsed.begin(), parsed.end());
  auto T = params[0], D = params[1];
  auto min_T = (T - sqrt(T*T - 4*D)) / 2,
    max_T = (T + sqrt(T*T - 4*D)) / 2;
  return ceil(max_T) - floor(min_T) - 1;
}
} // namespace aoc6
