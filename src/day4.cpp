#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <cmath>

#include "aoc.h"
#include "utils.h"

namespace aoc4 {

using namespace std;
using namespace aoc;

// Returns a range of how many numbers are winners for each card
auto parseInput(const string_view &input) {
  return splitStringBy(input, '\n')     // Split lines
    | views::transform([](const string_view &str) {
      // Get the winner and card numbers
      int start = str.find(':') + 1, mid = str.find('|', start);
      auto winners = splitStringToNumbers<int>(str.substr(start, mid - start), ' ');
      auto numbers = splitStringToNumbers<int>(str.substr(mid + 1), ' ');
      ranges::sort(winners);  // For efficiency
      // Loop on numbers, accumulating if it's in winners
      return accumulate(numbers.begin(), numbers.end(), 0, [&winners](int prev, int n) {
        return (ranges::binary_search(winners, n)) ? prev + 1 : prev;
      });
    });
}

int solvePartOne(const string &input) {
  auto nums = parseInput(input)
    | views::common;
  return accumulate(nums.begin(), nums.end(), 0, [](int prev, int exp) {
    // Rules for part one
    return prev + ((exp == 0) ? 0 : pow(2, exp - 1));
  });
}

int solvePartTwo(const string &input) {
  auto nums_rg = parseInput(input);
  vector nums(nums_rg.begin(), nums_rg.end());
  vector cards(distance(nums.begin(), nums.end()), 1);

  // Propagate numbers to following cards
  for (int i = 0; i < nums.size(); i++) {
    for (int j = 0; j < nums[i] && i + 1 + j < cards.size(); j++) {
      cards[i + 1 + j] += cards[i];
    }
  }

  return accumulate(cards.begin(), cards.end(), 0);
}
} // namespace aoc4
