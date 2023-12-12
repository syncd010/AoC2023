#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "aoc.h"
#include "utils.h"

using namespace std;
using namespace aoc;

int main(const int argc, const char *const argv[]) {

  const vector<string_view> args(argv + 1, argv + argc);

  bool useTestFile = false;
  string day{}, filePath{};

  for (const auto &arg : args) {
    if (arg == "-t") {
      useTestFile = true;
    } else if (day.empty()) {
      day = arg;
    } else {
      filePath = arg;
    }
  }

  if (day.empty() || !all_of(day.begin(), day.end(), ::isdigit)) {
    cout << "usage: aoc day [-t] [file]\n"
         << "\tday - Day to run\n"
         << "\t-t - Test switch, to use 'data/input{day}Test' file for the "
            "data if none specified\n"
         << "\tfile - data file, use 'data/input{day}' if none specified";
    return EXIT_FAILURE;
  }

  if (filePath.empty()) {
    filePath = "data/input" + day + (useTestFile ? "Test" : "");
  }

  ifstream fs(filePath);
  if (!fs.is_open()) {
    cout << "Error: cannot open input file: " + filePath;
    return EXIT_FAILURE;
  }

  // Read whole file
  stringstream buffer;
  buffer << fs.rdbuf();
  fs.close();
  const auto input = buffer.str();

  switch (cston<int>(day)) {
  default:
    cout << "Day " << day << " not implemented.";
    return EXIT_FAILURE;

// Case for handling each day
#define AOC_DAY_CASE(macro_day)                                                \
  case cston<int>(#macro_day):                                                      \
    cout << "Part one: " << aoc##macro_day::solvePartOne(input) << "\n";     \
    cout << "Part two: " << aoc##macro_day::solvePartTwo(input) << "\n";     \
    break;

    // Generate cases for all days
    REPEAT_FOR_DAY(AOC_DAY_CASE)
  }

  return EXIT_SUCCESS;
}
