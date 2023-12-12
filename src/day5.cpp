#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>
#include <cstdint>

#include "aoc.h"
#include "utils.h"

namespace aoc5 {

using namespace std;
using namespace aoc;

// Aux struct representing a conversion map
struct AlmanacMap {
  string_view name;
  vector<vector<int64_t>> mappings;
};

tuple<vector<int64_t>, vector<AlmanacMap>> parseInput(const string &input) {
  auto lines = input
        | views::split('\n')
        | views::filter([](auto rg) { return !rg.empty(); })
        | views::transform([](auto rg){ return string_view(rg.begin(), rg.end()); });

  auto seedsLine = (*lines.begin()).substr(6);
  vector<int64_t> seeds = splitStringToNumbers<int64_t>(seedsLine, ' ');

  vector<AlmanacMap> allMaps{};
  for (auto l : lines | views::drop(1)) {
    size_t tokenPos = l.find(" map:");
    if (tokenPos != string::npos) {
      auto newMap = AlmanacMap(string_view(l.substr(0, tokenPos)), {});
      allMaps.push_back(newMap);
    } else {
      auto newMappings = splitStringToNumbers<int64_t>(l, ' ');
      allMaps.back().mappings.push_back(newMappings);
    }
  }
  return make_tuple(seeds, allMaps);
}

Result solvePartOne(const string &input) {
  auto parsed = parseInput(input);
  auto seeds = get<0>(parsed);
  auto allMaps = get<1>(parsed);

  vector<int64_t> mappedSeeds{};
  for (auto seed : seeds) {
    // Pass each seed through the maps, converting it
    for (auto singleMap : allMaps) {
      for (auto m : singleMap.mappings) {
        int64_t start = m[1], len = m[2], end = start + len, convert = m[0];
        if (seed >= start && seed < end) {
          seed += convert - start;
          break;
        }
      }
    }
    mappedSeeds.push_back(seed);
  }

  return accumulate(mappedSeeds.begin(), mappedSeeds.end(), mappedSeeds.front(), ranges::min);
}

Result solvePartTwo(const string &input) {
  auto parsed = parseInput(input);
  auto auxSeeds = get<0>(parsed);
  auto allMaps = get<1>(parsed);

  vector<tuple<int64_t, int64_t>> seeds, mappedSeeds{};
  // Join seeds in pairs [start, len)
  for (int i = 0; i < auxSeeds.size(); i += 2) {
    seeds.push_back(make_tuple(auxSeeds[i], auxSeeds[i+1]));
  }

  // Iterate over maps now
  for (auto singleMap : allMaps) {

    // Use seeds as a queue
    while (!seeds.empty()) {
      auto newSeed = seeds.back();
      seeds.pop_back();
      int64_t seedStart = get<0>(newSeed), seedLen = get<1>(newSeed), seedEnd = seedStart + seedLen;

      // Convert a single seed. If necessary, partition it and queue the segments not converted
      for (auto m : singleMap.mappings) {
        int64_t mapStart = m[1], mapLen = m[2], mapEnd = mapStart + mapLen, mapConvert = m[0];
        if (seedStart < mapStart && seedEnd > mapStart) {
          // Partition first segment and queue it, convert middle segment
          seeds.push_back(make_tuple(seedStart, mapStart - seedStart));
          newSeed = make_tuple(mapConvert, min(mapLen, seedEnd - mapStart));
          // If final segment exists, queue it
          if (seedEnd > mapEnd) {
            seeds.push_back(make_tuple(mapEnd, seedEnd - mapEnd));
          }
          break;
        }

        if (seedStart >= mapStart && seedStart < mapEnd) {
          // Convert first segment and if second segment exists, queue it
          newSeed = make_tuple(mapConvert + seedStart - mapStart, min(seedLen, mapEnd - seedStart));
          if (seedEnd > mapEnd) {
            seeds.push_back(make_tuple(mapEnd, seedEnd - mapEnd));
          }
          break;
        }
      }
      mappedSeeds.push_back(newSeed);
    }

    // Finished this map converting all seeds, swap them to go to the next map
    swap(seeds, mappedSeeds);
  }

  return accumulate(seeds.begin(), seeds.end(), get<0>(seeds.front()), [](int64_t m, tuple<int64_t, int64_t> seed) {
    return min(m, get<0>(seed));
  });

  return 0;
}
} // namespace aoc5
