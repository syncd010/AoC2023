#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <set>

#include "aoc.h"
#include "utils.h"

namespace aoc7 {

using namespace std;
using namespace aoc;

class Hand {
public:
  string_view hand;
  int bid;

  Hand(string_view h, int b, bool useJokers = false) : hand{h}, bid{b} {
    type = useJokers ? calcTypeWithJokers(h) : calcType(h);
    // Substitute AKQJT for chars in lexicographic order, so that we can
    // later simply compare 2 strings and get a ordering out of them
    map<char, char> valueMap{{'A', 'e'}, {'K', 'd'}, {'Q', 'c'}, {'J', useJokers ? '0' : 'b'}, {'T', 'a'}};
    ranges::copy(hand 
      | views::transform([&valueMap](char c) {
        return valueMap.contains(c) ? valueMap[c] : c;
      }),
      back_inserter(compareHand));
  }

  // Comparison operator for posterior sort
  inline friend bool operator< (const Hand& lhs, const Hand& rhs) { 
    return lhs.type < rhs.type ||
      (lhs.type == rhs.type && lhs.compareHand < rhs.compareHand);
  }

private:
  // Auxiliary fields
  int type;
  string compareHand;

  static int calcType(string_view hand) {
    // Calculate hand type, by checking how many different cards there are
    map<char, int> cardCounts;
    for (char c : hand) cardCounts[c]++;

    int cnt;
    switch (cardCounts.size()) {
    case 1: // Five of a kind
       return 6;
    case 2: // Four of a kind or full house
      cnt = cardCounts.begin()->second;
      return (cnt == 4 || cnt == 1) ? 5 : 4;
    case 3: // Three of a kind or two pair
      cnt = ranges::max(views::values(cardCounts));
      return (cnt == 3) ? 3 : 2;
    case 4: // One pair
      return 1;
    default:
      return 0;
    }
  }

  static int calcTypeWithJokers(string_view hand) {
    // Calculate hand type with jokers, by repeatedly substituting jokers
    // by other cards in the hand and return the one with greater value
    vector<string> toConvert = {string(hand)}, converted{};

    while (!toConvert.empty()) {
      auto curr = toConvert.back();
      toConvert.pop_back();
      int pos = curr.find('J');
      if (pos != string::npos) {
        auto uniqueChars = set(curr.begin(), curr.end());
        if (uniqueChars.size() == 1 && *uniqueChars.begin() == 'J') {
          // Degenerate case of all Jokers
          converted.push_back("00000");
        } else {
          for (auto c : uniqueChars) {
            if (c == 'J') continue;
            auto newCurr = curr;
            newCurr[pos] = c;
            toConvert.push_back(newCurr);
          }
        }
      } else {
        // No jokers
        converted.push_back(curr);
      }
    }
    // Get the maximum of the converted hands
    return ranges::max(converted | views::transform([] (string &h) { return calcType(h); }));
  }
};

// Parse input into a vector of hands
vector<Hand> parseInput(const string &input, bool useJokers) {
  auto rg = splitStringBy(input, '\n')
    | views::filter([](auto line) { return !line.empty(); })
    | views::transform([useJokers](auto line) {
      size_t pos = line.find(' ');
      return Hand(line.substr(0, pos), ston<int>(line.substr(pos+1)), useJokers);
    });
  return vector<Hand>(rg.begin(), rg.end());
}

int solve(const string &input, bool useJokers) {
  auto hands = parseInput(input, useJokers);
  sort(hands.begin(), hands.end());

  // having enumerate would be nice...
  int ret = 0;
  for (int i = 0; i < hands.size(); i++) {
    ret += (i + 1) * hands[i].bid;
  }
  return ret;
}

int solvePartOne(const string &input) {
  return solve(input, false);
}

int solvePartTwo(const string &input) {
  return solve(input, true);
}
} // namespace aoc7
