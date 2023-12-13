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

int64_t calcCombinations(string_view record, vector<int> groups, int recPos = 0, int groupPos = 0) {
  int thisGroup = groups[groupPos];
  int spacer = accumulate(groups.begin() + groupPos, groups.end(), 0) + groups.size() - 1 - groupPos;
  int64_t combinations = 0;
  for (int pos = recPos; pos <= record.size() - spacer; pos++) {
    bool fits = record.find('.', pos) >= pos + thisGroup && 
        (pos + thisGroup == record.size() || record[pos+thisGroup] != '#');

    if (fits) {
      if (groupPos == groups.size() - 1) {
        // Check if we should have matched further ahead
        bool onesAhead = record.find('#', pos + thisGroup) != string::npos;
        if (!onesAhead) combinations++;
      } else {
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
      cache.clear();
      size_t c = calcCombinations(pair.first, pair.second, 0, 0);
      // cout << pair.first << " : " << c << "\n";
      return c;
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
      cache.clear();
      // Expand input by 5
      string records{pair.first};
      vector<int> groups{pair.second};
      for (int i = 0; i < 4; i++) {
        records.append("?");
        records.append(pair.first);
        groups.insert(groups.end(), pair.second.begin(), pair.second.end());
      }

      int64_t c = calcCombinations(records, groups);
      return c;
    });
  return accumulate(rg.begin(), rg.end(), static_cast<int64_t>(0));
}
} // namespace aoc12
