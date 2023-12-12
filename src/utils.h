#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>

namespace aoc {

/**
 * String-to-number as constexpr
 *
 * @param str Strint to convert
 * @param base Base to interpret
 * @return Number represented by string
 */
template<typename T>
constexpr T cston(std::string_view str, const int base = 10) {
  T ret = 0;
  bool is_negative = false;
  for (const auto c : str) {
    switch (c) {
    case '-':
      is_negative = true;
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
  return (is_negative) ? ret * -1 : ret;
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
std::vector<T> splitStringToNumbers(std::string_view str, U delim, int base = 10) {
  auto splits = str
    | std::views::split(delim)
    | std::views::filter([](auto rg) {
      if (rg.empty()) return false;
      return true;
    })
    | std::views::transform([](auto rg) {
      return cston<T>(std::string_view(rg.begin(), rg.end()));
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
template<typename T>
constexpr auto splitStringBy(const std::string_view &str, const T &delim) {
    return str
        | std::views::split(delim)
        | std::views::transform([](auto rg){
              return std::string_view(rg.begin(), rg.end());
          });
};

} // namespace aoc

#endif
