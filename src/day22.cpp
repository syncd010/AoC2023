#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <queue>

#include "aoc.h"
#include "utils.h"

namespace aoc22 {

using namespace std;
using namespace aoc;

using Brick = pair<vec3<int>, vec3<int>>;

vector<Brick> parseInput(const string &input) {
  return toVector(input 
    | splitString('\n') 
    | views::transform([](string_view line) { 
      auto v = toVector(line 
        | splitString('~')
        | views::transform([](string_view end) {
          auto v = toVector(end | splitNumbers(','));
          return vec3<int>(v[0], v[1], v[2]);
        }));
      auto v1 = vec3<int>(min(v[0].x, v[1].x), min(v[0].y, v[1].y), min(v[0].z, v[1].z));
      auto v2 = vec3<int>(max(v[0].x, v[1].x), max(v[0].y, v[1].y), max(v[0].z, v[1].z));
      return make_pair(v1, v2);
     }));
}

auto settleDown(vector<Brick> &bricks) {
  int wellsz = 1 + foldLeft(bricks, 0, [](int prev, auto &b) { return max(prev, max(b.second.x, b.second.y)); });
  auto well = vector(wellsz, vector<pair<int, int>>(wellsz, {0, -1}));
  auto supportsBellow = vector(bricks.size(), vector<int>{});
  for (int i = 0; i < bricks.size(); i++) {
    int maxHeight = 0;
    for (int y = bricks[i].first.y; y <= bricks[i].second.y; y++) {
      for (int x = bricks[i].first.x; x <= bricks[i].second.x; x++) {
        auto [height, supporterId] = well[y][x];
        if (height < maxHeight) continue;

        if (height > maxHeight) {
          maxHeight = height;
          supportsBellow[i].clear();
        } 
        if (supporterId != -1 && (ranges::find(supportsBellow[i], supporterId) == supportsBellow[i].end())) {
          supportsBellow[i].push_back(supporterId);
        }
      }
    }

    auto brickHeight = bricks[i].second.z - bricks[i].first.z;
    auto wellFill = make_pair(maxHeight + 1 + brickHeight, i);
    for (int y = bricks[i].first.y; y <= bricks[i].second.y; y++) {
      for (int x = bricks[i].first.x; x <= bricks[i].second.x; x++) {
        well[y][x] = wellFill;
      }
    }
    bricks[i].first.z = maxHeight + 1; 
    bricks[i].second.z = maxHeight + 1 + brickHeight;
  }
  auto supportsAbove = vector(bricks.size(), vector<int>{});
  for (int i = 0; i < bricks.size(); i++) {
    for (int j : supportsBellow[i]) {
      supportsAbove[j].push_back(i);
    }
  }
  return make_pair(supportsBellow, supportsAbove);
}

Result solvePartOne(const string &input) {
  auto bricks = parseInput(input);
  sort(bricks.begin(), bricks.end(), [](const auto &b1, const auto &b2) {
    return min(b1.first.z, b1.second.z) < min(b2.first.z, b2.second.z);
  });
  auto [supportsBellow, supportsAbove] = settleDown(bricks);

  // Mark bricks that singly support another
  auto singleSupporters = vector<bool>(bricks.size(), false);
  for (auto s : supportsBellow) {
    if (s.size() == 1) singleSupporters[s.front()] = true;
  }
  return ranges::count(singleSupporters, false);
}

Result solvePartTwo(const string &input) {
  auto bricks = parseInput(input);
  sort(bricks.begin(), bricks.end(), [](const auto &b1, const auto &b2) {
    return min(b1.first.z, b1.second.z) < min(b2.first.z, b2.second.z);
  });
  auto [supportsBellow, supportsAbove] = settleDown(bricks);

  // Mark each brick as fallen and process a queue of fallen bricks. Mark bricks 
  // supported by them ones as fallen if all of their supports have fallen
  int count = 0;
  auto fallen = vector<int>(bricks.size());
  for (int i = 0; i < bricks.size(); i++) {
    ranges::fill(fallen, 0);
    fallen[i] = 1;
    auto q = queue<int>({i});
    while (!q.empty()) {
      auto curr = q.front();
      q.pop();
      for (auto supported : supportsAbove[curr]) {
        if (ranges::all_of(supportsBellow[supported], [&fallen](auto s) { return fallen[s] == 1; })) {
          fallen[supported] = 1;
          q.push(supported);
        }
      }
    }
    count += accumulate(fallen.begin() + i + 1, fallen.end(), 0);
  }
  return count;
}
} // namespace aoc22
