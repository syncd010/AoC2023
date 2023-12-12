#ifndef _AOC_H
#define _AOC_H

#include <string>

#define REPEAT_FOR_DAY(macro)   \
  macro(1)  \
  macro(2)  \
  macro(3)  \
  macro(4)  \
  macro(5)  \
  macro(6)  \
  macro(7)  \
  // Additional days here

#define AOC_DAY_FUNCTION_DECLARATION(day)           \
  namespace aoc##day {                              \
    int solvePartOne(const std::string &input);     \
    int solvePartTwo(const std::string &input);     \
  }

REPEAT_FOR_DAY(AOC_DAY_FUNCTION_DECLARATION)

#endif