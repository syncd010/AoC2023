#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "aoc.h"
#include "utils.h"

namespace aoc1 {
using namespace std;
using namespace aoc;

Result solvePartOne(const string &input) {
  auto nums = input
    | views::split('\n') 
    | views::transform([](auto line) {
        auto digits = line 
            | views::filter(::isdigit);
        return digits.empty() ? 0 : stoi(string() + digits.front() + digits.back());
    }) 
    | views::common;
  
  return accumulate(nums.begin(), nums.end(), 0);
}

Result solvePartTwo(const string &input) {
  auto digits = map<string, char> 
    {{"one", '1'},   {"two", '2'},   {"three", '3'}, 
     {"four", '4'},  {"five", '5'}, {"six", '6'}, 
     {"seven", '7'}, {"eight", '8'}, {"nine", '9'}};

  // Substitute names for digits
  auto converted = string{input};
  for (auto d : digits) {
    auto pos = 0;
    while ((pos = converted.find(d.first, pos)) != std::string::npos) {
      converted.insert(converted.begin() + pos + 1, 1, d.second);
      pos += 2;
    }
  }
  return solvePartOne(converted);
}
} // namespace aoc1
