#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <deque>

#include "aoc.h"
#include "utils.h"

namespace aoc21 {

using namespace std;
using namespace aoc;

using Pos = vec2<int>;
using Dir = vec2<int>;

// Standard directions
constexpr auto 
  NORTH = Dir(0, -1),
  SOUTH = Dir(0, 1),
  EAST = Dir(1, 0),
  WEST = Dir(-1, 0);

constexpr char START = 'S', ROCK = '#', EMPTY = '.';

// Returns the active nodes on the grid, with max steps limit
auto countActive(const vector<string_view> &grid, int maxSteps, bool infiniteBoard) {
  auto h = (int)grid.size(), w = (int)grid[0].size();  
  auto startPos = Pos{h / 2, w / 2};
  const auto dirs = vector{EAST, WEST, SOUTH, NORTH};
  auto frontier = deque<pair<Pos, int>>{ {startPos, 0 }};
  auto vh = 2 * (maxSteps + 1), vw = 2 * (maxSteps + 1);
  auto cy = (vh - h) / 2, cx = (vw - w) / 2;
  auto visited = vector(vh, vector(vw, -1));
  visited[cy + startPos.y][cx + startPos.x] = 0;

  auto active = int64_t{ maxSteps % 2 == 0 };
  while (!frontier.empty()) {
    auto [p, step] = frontier.front();
    frontier.pop_front();
    if (++step > maxSteps) break;

    for (const auto &d : dirs) {
      auto nextP = p + d;
      auto x = nextP.x % w, y = nextP.y % h;
      if (x < 0) x += w;
      if (y < 0) y += h;
      if ((infiniteBoard || insideBoard(grid, nextP)) && 
          (grid[y][x] != ROCK) && 
          (visited[cy + nextP.y][cx + nextP.x] == -1)) {
        visited[cy + nextP.y][cx + nextP.x] = step;
        active += ((step % 2) == (maxSteps % 2));
        frontier.push_back(make_pair(nextP, step));
      }
    }
  }
  return active;
}

Result solvePartOne(const string &input) {
  auto grid = toVector(input | splitString<string_view>('\n'));
  return countActive(grid, 64, false);
}

// Lagrange interpolation polynomial
auto lagrangePoly(const vector<int64_t> &xs, const vector<int64_t> &ys) {
  return [&xs, &ys](int64_t x) {
    auto n = xs.size();
    auto sum = int64_t{ 0 };
    for (int j = 0; j < n; j++) {
      auto prod = int64_t{ 1 };
      for (int i = 0; i < n; i++) {
        if (i == j) continue;
        prod *= (x - xs[i])/(xs[j] - xs[i]);
      }
      sum += ys[j] * prod;
    }
    return sum;
  };
}

Result solvePartTwo(const string &input) {
  auto grid = toVector(input | splitString<string_view>('\n'));
  auto h = (int)grid.size(), halfH = h / 2;
  auto xs = vector<int64_t>{h/2, h/2 + h, h/2 + 2*h},
       ys = toVector(xs | views::transform([&grid](auto x) { return countActive(grid, x, true); }));
  
  return lagrangePoly(xs, ys)(26501365);

}

} // namespace aoc21
