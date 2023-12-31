#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <tuple>

#include "aoc.h"
#include "utils.h"

namespace aoc18 {

using namespace std;
using namespace aoc;

using Instruction = tuple<char, int, string_view>;
using Dir = vec2<int64_t>;
using Pos = vec2<int64_t>;

vector<Instruction> parseInput(const string &input) {
  return toVector(input
    | splitString('\n')
    | views::transform([](string_view line) {
      return make_tuple(line.at(0), ston<int>(line.substr(2, line.find(' ', 2) - 2)), line.substr(line.find('(', 2) + 1, 7));
    }));
}

vector<Pos> getVertices(const vector<Instruction> &inst) {
  auto dirMap = unordered_map<char, Dir>{ { 'R', Dir{1, 0} }, { 'L', Dir{-1, 0} }, { 'D', Dir{0, 1} }, { 'U', Dir{0, -1} }, };

  auto pos = Pos{0, 0};
  auto vertices = vector<Pos>{};
  for (auto [dir, steps, color] : inst) {
    pos += dirMap[dir] * steps;
    vertices.push_back(pos);
  }
  return vertices;
}

int64_t getArea(vector<Pos> vertices) {
  // Shoelace formula
  auto sz = vertices.size();
  int64_t shoelaces = 0, boundary = 0;
  for (int i = 0; i < sz; i++) {
    shoelaces += vertices[i].x * vertices[(i + 1) % sz].y - vertices[(i + 1) % sz].x * vertices[i].y;
    boundary += vertices[i].manhattanDistanceTo(vertices[(i + 1) % sz]);
  }
  shoelaces = abs(shoelaces) / 2;
  // Pick's
  int64_t interior = shoelaces - boundary / 2 + 1;
  return boundary + interior;
}

Result solvePartOne(const string &input) {
  return getArea(getVertices(parseInput(input)));
}

Result solvePartTwo(const string &input) {
  auto inst = parseInput(input);

  auto colorMap = unordered_map<char, char>{ { '0', 'R' }, { '1', 'D' }, { '2', 'L' }, { '3', 'U' }, };
  auto newInst = vector<Instruction>{};
  for (auto [dir, steps, color] : inst) {
    newInst.push_back(Instruction(colorMap[color.back()], ston<int>(color.substr(1, 5), 16), color));
  }
  return getArea(getVertices(newInst));
}
} // namespace aoc18
