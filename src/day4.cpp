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
auto parseInput(const string &input) {
  return input 
    | splitString('\n')
    | views::transform([](string_view str) {
      // Get the winner and card numbers
      int start = str.find(':') + 1, mid = str.find('|', start);
      auto winners = toVector(str.substr(start, mid - start) | splitNumbers(' '));
      auto numbers = toVector(str.substr(mid + 1) | splitNumbers(' '));
      // Intersect numbers and winners
      ranges::sort(winners);
      return foldLeft(numbers, 0, [&winners](int prev, int n) {
        return (ranges::binary_search(winners, n)) ? prev + 1 : prev;
      });
    });
}

Result solvePartOne(const string &input) {
  auto winnerCounts = parseInput(input);
  return foldLeft(winnerCounts, 0, [](int prev, int exp) {
    // Rules for part one
    return prev + ((exp == 0) ? 0 : (int64_t)pow(2, exp - 1));
  });
}

Result solvePartTwo(const string &input) {
  auto winnerCounts = toVector(parseInput(input));
  auto cards = vector(winnerCounts.size(), 1);

  // Propagate numbers to following cards
  for (int i = 0; i < winnerCounts.size(); i++) {
    for (int j = 0; j < winnerCounts[i] && i + 1 + j < cards.size(); j++) {
      cards[i + 1 + j] += cards[i];
    }
  }

  return accumulate(cards.begin(), cards.end(), 0);
}
} // namespace aoc4
