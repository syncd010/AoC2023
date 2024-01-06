#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>

#include "aoc.h"
#include "utils.h"

using namespace std;
using namespace aoc;

int main(const int argc, const char *const argv[]) {

  const auto args = vector<string_view>(argv + 1, argv + argc);

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

  auto fs = ifstream(filePath);
  if (!fs.is_open()) {
    cout << "Error: cannot open input file: " + filePath;
    return EXIT_FAILURE;
  }

  // Read whole file
  auto buffer = stringstream{};
  buffer << fs.rdbuf();
  fs.close();
  const auto input = buffer.str();

  auto execSolve = [](auto solveFunc, const string &input, const string &partDesc) {

    auto t1 = chrono::high_resolution_clock::now();
    auto res = solveFunc(input);
    auto t2 = chrono::high_resolution_clock::now();
    auto ms = (double)duration_cast<chrono::microseconds>(t2 - t1).count() / 1000;

    if (holds_alternative<monostate>(res)) {
      cout << partDesc << ": Didn't return a value\n";
    } else if (holds_alternative<string>(res)) {
      cout << format("{} ({:.2f}ms): {}\n", partDesc, ms, get<string>(res));
    } else if (holds_alternative<int64_t>(res)) {
      cout << format("{} ({:.2f}ms): {}\n", partDesc, ms, get<int64_t>(res));
    }
  };

  switch (ston<int>(day)) {
  default:
    cout << "Day " << day << " not implemented.";
    return EXIT_FAILURE;

// Case for handling each day
#define AOC_DAY_CASE(macro_day)                                              \
  case ston<int>(#macro_day):                                                \
    execSolve(aoc##macro_day::solvePartOne, input, "Part one");                     \
    execSolve(aoc##macro_day::solvePartTwo, input, "Part two");                     \
    break;

    // Generate cases for all days
    REPEAT_FOR_DAY(AOC_DAY_CASE)
  }

  return EXIT_SUCCESS;
}
