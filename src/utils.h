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
 * Simple 2D position / direction encapsulation
*/
template<typename T>
struct vec2 {
  T x, y;

  friend constexpr vec2 operator+(vec2 p, vec2 o) { return p += o; }
  friend constexpr vec2 operator-(vec2 p, vec2 o) { return p -= o; }
  friend constexpr vec2 operator*(vec2 p, T factor) { return p *= factor; }

  constexpr vec2 &operator+=(const vec2 &other) {
    x += other.x; y += other.y;
    return *this;
  }

  constexpr vec2 &operator-=(const vec2 &other) {
    x -= other.x; y -= other.y;
    return *this;
  }

  constexpr vec2 &operator*=(const T factor) {
    x *= factor; y *= factor;
    return *this;
  }

  bool operator==(const vec2 &) const = default;
  auto operator<=>(const vec2 &) const = default;

  double distanceTo(vec2 other) {
    T dx = x - other.x, dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  T manhattanDistanceTo(vec2 other) {
    T dx = x - other.x, dy = y - other.y;
    return std::abs(dx) + std::abs(dy);
  }
};

template<typename T>
std::ostream & operator<<(std::ostream &os, const vec2<T> & c) {
	os << "[" << c.x << ", " << c.y << "]";
	return os;
}

template<typename T>
struct vec2Hash {
    std::size_t operator()(const vec2<T>& v) const noexcept {
      return (v.x + v.y) * (v.x + v.y + 1) / 2 + v.y;
    }
};

/**
 * Transposes a board, returns a new board
 * 
 * @param orig Board. Vector of string or string_view
 * @return A new board transposed
*/
template<typename T>
std::vector<std::string> transposeBoard(const std::vector<T> &orig) {
  std::vector<std::string> dest = std::vector(orig[0].size(), std::string(orig.size(), ' '));

  for (int y = 0; y < orig.size(); y++) {
    for (int x = 0; x < orig[y].size(); x++) {
      dest[x][y] = orig[y][x];
    }
  }
  return dest;
}

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
 * Returns whether the position is inside the board
 * 
 * @param board Board
 * @param pos Position to check
 * @return Whether the position is inside the board
*/
template<typename T, typename U>
inline bool insideBoard(const std::vector<T> &board, vec2<U> pos) {
  U h = board.size(), w = board[0].size();
  return inBounds(pos.x, 0, w) && inBounds(pos.y, 0, h);
}

} // namespace aoc

/**
 * Prints a board
 * 
 * @param board Board
 * @param message Message to print before
*/
template<typename T>
inline void displayBoard(const std::vector<T> &board, const std::string_view message = "") {
  if (!message.empty()) std::cout << message << "\n";
  for (const T &line : board) std::cout << line << "\n";
}

#endif
