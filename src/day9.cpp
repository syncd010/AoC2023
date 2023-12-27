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

namespace aoc9 {

using namespace std;
using namespace aoc;

// Returns a vector of the differences
vector<int64_t> diff(const vector<int64_t> &in) {
  vector<int64_t> out(in.size() - 1);
  for (int i = 0; i < in.size() - 1; i++) {
    out[i] = in[i+1] - in[i];
  }
  return out;
}

// Return next element in sequence, assumes one of the derivatives is constant
int64_t nextInSequence(const vector<int64_t> &sequence) { 
  // Get derivatives until 0, sum last difference
  vector<int64_t> dSeq = sequence;
  int64_t lastDiff = 0;
  while (!ranges::all_of(dSeq, [](int64_t n) { return n == 0; })) {
    dSeq = diff(dSeq);
    lastDiff += dSeq.back();
  }
  return sequence.back() + lastDiff;
}

// Parse input to lines of vector of numbers
auto parseInput(const string &input) {
  return input 
    | splitString('\n')
    | views::transform([](auto s) { return toVector(s | splitNumbers<int64_t>(' ')); });
}

Result solvePartOne(const string &input) {
  auto rg = parseInput(input)
    | views::transform(nextInSequence);
  return accumulate(rg.begin(), rg.end(), 0);
}

Result solvePartTwo(const string &input) {
  // Same as part one with input number reversed
  auto rg = parseInput(input)
    | views::transform([](const vector<int64_t> &sequence) { 
      return nextInSequence(vector(sequence.rbegin(), sequence.rend()));
    });
  return accumulate(rg.begin(), rg.end(), 0);
}
} // namespace aoc9
