#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <cmath>
#include <vector>
#include <ostream>

namespace aoc {

/**
 * String-to-number as constexpr
 *
 * @param str String to convert
 * @param base Base to interpret
 * @return Number represented by string
 */
template<typename T>
constexpr T ston(std::string_view str, const int base = 10) {
  T ret = 0;
  bool negative = false;
  for (const auto c : str) {
    switch (c) {
    case '-':
      negative = true;
      continue;
    case '+':
    case ' ':
      continue;
    }
    int digit = 0;
    if ((c >= '0') && (c <= '9')) {
      digit = c - '0';
    } else if ((base > 10) && ((c >= 'a') && (c <= 'z'))) {
      digit = c - 'a' + 10;
    } else if ((base > 10) && ((c >= 'A') && (c <= 'Z'))) {
      digit = c - 'A' + 10;
    } else {
      break;
    }
    if (digit >= base) {
      break;
    }
    ret = ret * base + digit;
  }
  return (negative) ? ret * -1 : ret;
}

/**
 * Splits a string with numbers into a vector of ints
 * 
 * @param str String to split, assumes it has numbers
 * @param delim delimiter to use to separate the numbers
 * @param base Base to interpret numbers
 * @return Vector of ints
*/
template <typename T, typename U>
std::vector<T> splitStringToNumbers(const std::string_view str, const U delim, int base = 10) {
  auto splits = str
    | std::views::split(delim)
    | std::views::filter([](auto rg) { return !rg.empty(); })
    | std::views::transform([](auto rg) {
      return ston<T>(std::string_view(rg.begin(), rg.end()));
    });
    return std::vector(splits.begin(), splits.end());
}

/**
 * Splits a string by a delimiter
 * 
 * @param str String to split
 * @param delim delimiter to use
 * @return Range with string splits, each one converted to string
*/
template<typename R=std::string_view, typename T = char>
constexpr auto splitStringBy(const std::string_view &str, const T &delim, bool keepEmptyLines = false) {
  return str
    | std::views::split(delim)
    | std::views::filter([keepEmptyLines](auto rg) { return keepEmptyLines || !rg.empty(); })
    | std::views::transform([](auto rg){
          return R(rg.begin(), rg.end());
      });
};

/**
 * Checks whether a value is between [min, max)
 * 
 * @param val Value
 * @param min Inclusive minimum
 * @param max Exclusive maximum
 * @return whether val is between [min, max)
*/
template<typename T>
constexpr bool inBounds(T val, T min, T max) {
  return val >= min && val < max;
}

/**
 * Simple 2D position / direction encapsulation
*/
template<typename T>
struct Vector2 {
  T x, y;

  Vector2 operator+(const Vector2 &other) const { return Vector2{x + other.x, y + other.y}; }
  Vector2 operator-(const Vector2 &other) const { return Vector2{x - other.x, y - other.y}; }

  bool operator==(const Vector2 &other) const { return x == other.x && y == other.y; }

  double distanceTo(Vector2 other) {
    T dx = x - other.x, dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  T manhattanDistanceTo(Vector2 other) {
    T dx = x - other.x, dy = y - other.y;
    return std::abs(dx) + std::abs(dy);
  }
};

template<typename T>
std::ostream & operator<<(std::ostream &os, const Vector2<T> & c) {
	os << "[" << c.x << ", " << c.y << "]";
	return os;
}

} // namespace aoc

#endif
