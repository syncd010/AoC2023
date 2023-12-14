#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <unordered_map>

#include "aoc.h"
#include "utils.h"

namespace aoc12 {

using namespace std;
using namespace aoc;

static unordered_map<int64_t, int64_t>cache;

// https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function
inline int64_t cantorPairingFunction(int64_t k1, int64_t k2) {
  return (k1 + k2) * (k1 + k2 + 1) / 2 + k2;
}

// Calculates the possible combinations starting on recPos and groupPos
int64_t calcCombinations(string_view record, vector<int> groups, int recPos = 0, int groupPos = 0) {
  int thisGroup = groups[groupPos];
  // Estimate the space needed at the end for the remaining groups
  int spacer = accumulate(groups.begin() + groupPos, groups.end(), 0) + groups.size() - 1 - groupPos;
  int64_t combinations = 0;
  // Loop on each character, checking if the current group can be placed there
  for (int pos = recPos; pos <= record.size() - spacer; pos++) {
    bool fits = record.find('.', pos) >= pos + thisGroup && 
        (pos + thisGroup == record.size() || record[pos+thisGroup] != '#');

    if (fits) {
      // Current group fits on current pos. If it's the last one count one more combination, 
      // otherwise recurse with remaining groups from the current pos
      if (groupPos == groups.size() - 1) {
        // Last group, check if we should have matched further ahead - any # remaining?
        bool poundsRemaining = record.find('#', pos + thisGroup) != string::npos;
        if (!poundsRemaining) combinations++;
      } else {
        // Recurse, but check cache first
        int nextRecPos = pos + thisGroup + 1, nextGroupPos = groupPos + 1;
        auto cacheKey = cantorPairingFunction(nextRecPos, nextGroupPos);
        if (!cache.contains(cacheKey)) {
          cache[cacheKey] = calcCombinations(record, groups, nextRecPos, nextGroupPos);
        }
        combinations += cache[cacheKey];
      }
    }
    // Must fit at most in this pos
    if (record[pos] == '#') break;
  }

  return combinations;
}

Result solvePartOne(const string &input) {
  auto rg = splitStringBy(input, '\n')
    | views::transform([](string_view str) {
      size_t sep = str.find(' ');
      return make_pair(str.substr(0, sep), splitStringToNumbers<int>(str.substr(sep + 1), ','));
    })
    | views::transform([](auto pair) {
      cache.clear();    // Clear cache on each line
      return calcCombinations(pair.first, pair.second);
    });

  return accumulate(rg.begin(), rg.end(), static_cast<int64_t>(0));
}

Result solvePartTwo(const string &input) {
  auto rg = splitStringBy(input, '\n')
    | views::transform([](string_view str) {
      size_t sep = str.find(' ');
      return make_pair(str.substr(0, sep), splitStringToNumbers<int>(str.substr(sep + 1), ','));
    })
    | views::transform([](auto pair) {
      cache.clear();    // Clear cache on each line
      // Expand input by 5
      string records{pair.first};
      vector<int> groups{pair.second};
      for (int i = 0; i < 4; i++) {
        records.append("?");
        records.append(pair.first);
        groups.insert(groups.end(), pair.second.begin(), pair.second.end());
      }

      return calcCombinations(records, groups);
    });
  return accumulate(rg.begin(), rg.end(), static_cast<int64_t>(0));
}
} // namespace aoc12
