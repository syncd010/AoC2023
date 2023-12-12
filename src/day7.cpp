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
    typeScore = useJokers ? scoreWithJokers(h) : score(h);
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
    return lhs.typeScore < rhs.typeScore ||
      (lhs.typeScore == rhs.typeScore && lhs.compareHand < rhs.compareHand);
  }

private:
  // Auxiliary fields
  int typeScore;
  string compareHand;

  static int score(string_view hand) {
    // Calculate hand typeScore, by checking how many different cards there are
    map<char, int> cardCounts;
    for (char c : hand) cardCounts[c]++;
    return accumulate(hand.begin(), hand.end(), 0, [&cardCounts](int p, char c) { return p + cardCounts[c]; } );
  }

  static int scoreWithJokers(string_view hand) {
    // Get the unique cards to try in the hand, except the joker
    auto uniqueChars = set(hand.begin(), hand.end());
    uniqueChars.erase('J');
    // Degenerate case of hand with all Jokers
    if (uniqueChars.empty()) return score(hand);

    // Substitute jokers and get the maximum score
    int maxScore = 0;
    for (char c : uniqueChars) {
      string newHand{hand};
      replace(newHand.begin(), newHand.end(), 'J', c);
      maxScore = max(maxScore, score(newHand));
    }
    return maxScore;
  }
};

// Parse input into a vector of hands
vector<Hand> parseInput(const string &input, bool useJokers) {
  auto rg = splitStringBy(input, '\n')
    | views::transform([useJokers](auto line) {
      size_t pos = line.find(' ');
      return Hand(line.substr(0, pos), ston<int>(line.substr(pos+1)), useJokers);
    });
  return vector<Hand>(rg.begin(), rg.end());
}

Result solve(const string &input, bool useJokers) {
  auto hands = parseInput(input, useJokers);
  sort(hands.begin(), hands.end());

  // having enumerate would be nice...
  int ret = 0;
  for (int i = 0; i < hands.size(); i++) {
    ret += (i + 1) * hands[i].bid;
  }
  return ret;
}

Result solvePartOne(const string &input) {
  return solve(input, false);
}

Result solvePartTwo(const string &input) {
  return solve(input, true);
}
} // namespace aoc7
