#ifndef _AOC_H
#define _AOC_H

#include <string>
#include <variant>
#include <cstdint>

#define REPEAT_FOR_DAY(macro)   \
  macro(1)  \
  macro(2)  \
  macro(3)  \
  macro(4)  \
  macro(5)  \
  macro(6)  \
  macro(7)  \
  macro(8)  \
  macro(9)  \
  macro(10)  \
  macro(11)  \
  macro(12)  \
  macro(13)  \
  macro(14)  \
  macro(15)  \
  macro(16)  \
  macro(17)  \
  macro(18)  \
  macro(19)  \
  macro(20)  \
  macro(21)  \
  macro(22)  \
  macro(23)  \
  macro(24)  \
  macro(25)  \
  // Additional days here

using Result = std::variant<std::monostate, std::int64_t, std::string>;

#define AOC_DAY_FUNCTION_DECLARATION(day)           \
  namespace aoc##day {                              \
    Result solvePartOne(const std::string &input);     \
    Result solvePartTwo(const std::string &input);     \
  }

REPEAT_FOR_DAY(AOC_DAY_FUNCTION_DECLARATION)

#endif