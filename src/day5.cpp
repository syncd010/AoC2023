#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
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

pair<vector<int64_t>, vector<AlmanacMap>> parseInput(const string &input) {
  auto lines = input | splitString('\n');
  auto seeds = toVector(lines.front().substr(6) | splitNumbers<int64_t>(' '));

  auto allMaps = vector<AlmanacMap>{};
  for (auto l : lines | views::drop(1)) {
    auto tokenPos = l.find(" map:");
    if (tokenPos != string::npos) {
      auto newMap = AlmanacMap(l.substr(0, tokenPos), {});
      allMaps.push_back(newMap);
    } else {
      auto newMappings = toVector(l | splitNumbers<int64_t>(' '));
      allMaps.back().mappings.push_back(newMappings);
    }
  }
  return make_pair(seeds, allMaps);
}

Result solvePartOne(const string &input) {
  auto [seeds, allMaps] = parseInput(input);

  // Pass each seed through the maps, converting it
  auto mappedSeeds = seeds | views::transform([&allMaps](auto seed) {
      for (auto singleMap : allMaps) {
        for (auto m : singleMap.mappings) {
          auto start = m[1], len = m[2], end = start + len, convert = m[0];
          if (seed >= start && seed < end) {
            seed += convert - start;
            break;
          }
        }
      }
      return seed;
    });

  return foldLeft(mappedSeeds, mappedSeeds.front(), ranges::min);
}

Result solvePartTwo(const string &input) {
  auto [auxSeeds, allMaps] = parseInput(input);

  auto seeds = vector<pair<int64_t, int64_t>>{}, 
       mappedSeeds = vector<pair<int64_t, int64_t>>{};
  // Join seeds in pairs [start, len)
  for (int i = 0; i < auxSeeds.size(); i += 2) {
    seeds.push_back(make_pair(auxSeeds[i], auxSeeds[i+1]));
  }

  // Iterate over maps now
  for (auto singleMap : allMaps) {

    // Use seeds as a queue
    while (!seeds.empty()) {
      auto newSeed = seeds.back();
      seeds.pop_back();
      auto [seedStart, seedLen] = newSeed;
      auto seedEnd = seedStart + seedLen;

      // Convert a single seed. If necessary, partition it and queue the segments not converted
      for (auto m : singleMap.mappings) {
        auto mapStart = m[1], mapLen = m[2], mapEnd = mapStart + mapLen, mapConvert = m[0];
        if (seedStart < mapStart && seedEnd > mapStart) {
          // Partition first segment and queue it, convert middle segment
          seeds.push_back(make_pair(seedStart, mapStart - seedStart));
          newSeed = make_pair(mapConvert, min(mapLen, seedEnd - mapStart));
          // If final segment exists, queue it
          if (seedEnd > mapEnd) seeds.push_back(make_pair(mapEnd, seedEnd - mapEnd));
          break;
        }

        if (seedStart >= mapStart && seedStart < mapEnd) {
          // Convert first segment and if second segment exists, queue it
          newSeed = make_pair(mapConvert + seedStart - mapStart, min(seedLen, mapEnd - seedStart));
          if (seedEnd > mapEnd) seeds.push_back(make_pair(mapEnd, seedEnd - mapEnd));
          break;
        }
      }
      mappedSeeds.push_back(newSeed);
    }

    // Finished this map converting all seeds, swap them to go to the next map
    swap(seeds, mappedSeeds);
  }

  return foldLeft(seeds, get<0>(seeds.front()), [](auto m, auto seed) {
    return min(m, get<0>(seed));
  });

  return 0;
}
} // namespace aoc5
