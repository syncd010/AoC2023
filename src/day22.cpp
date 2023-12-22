#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

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
      auto rg = line 
        | splitString('~')
        | views::transform([](string_view end) {
          auto v = toVector(end | splitNumbers<int>(','));
          return vec3(v[0], v[1], v[2]);
        });
      return make_pair(*(rg.begin()), *(++rg.begin()));
     }));
}

constexpr bool intersects(const Brick &b1, const Brick &b2) {
  return !((b1.first.x > b2.second.x) || (b1.second.x < b2.first.x)) &&
         !((b1.first.y > b2.second.y) || (b1.second.y < b2.first.y)) &&
         !((b1.first.z > b2.second.z) || (b1.second.z < b2.first.z));
}

int stepDown(vector<Brick> &bricks, int ignoreBrick = -1) {
  int countMoves = 0;

  for (int i = 0; i < bricks.size(); i++) {
    if (i == ignoreBrick) continue;
    if (min(bricks[i].first.z, bricks[i].second.z) == 1) continue;
    bool canMove = true;
    int drop = 1;
    bricks[i].first.z -= drop;
    bricks[i].second.z -= drop;

    for (int j = i - 1; j >= 0; j--) {
      if (j == ignoreBrick) continue;
      if (intersects(bricks[i], bricks[j])) {
        canMove = false;
        break;
      }
    }
    if (!canMove) {
      bricks[i].first.z += 1; 
      bricks[i].second.z += 1;
    } else {
      countMoves++;
    }
  }
  return countMoves;
}

Result solvePartOne(const string &input) {
  auto bricks = parseInput(input);
  sort(bricks.begin(), bricks.end(), [](const auto &b1, const auto &b2) {
    return min(b1.first.z, b1.second.z) < min(b2.first.z, b2.second.z);
  });

  int countMoves;
  do {
    countMoves = stepDown(bricks);
  } while (countMoves > 0);

  int count = 0;
  for (int i = 0; i < bricks.size(); i++) {
    auto newBricks = bricks;
    int countMoves = stepDown(newBricks, i);
    count += countMoves == 0;
  }

  return count;
}

Result solvePartTwo(const string &input) {
  auto bricks = parseInput(input);
  sort(bricks.begin(), bricks.end(), [](const auto &b1, const auto &b2) {
    return min(b1.first.z, b1.second.z) < min(b2.first.z, b2.second.z);
  });

  int countMoves;
  do {
    countMoves = stepDown(bricks);
  } while (countMoves > 0);

  int count = 0;
  for (int i = 0; i < bricks.size(); i++) {
    auto newBricks = bricks;
    int countMoves = stepDown(newBricks, i);
    count += countMoves;
  }

  return count;
}
} // namespace aoc22
