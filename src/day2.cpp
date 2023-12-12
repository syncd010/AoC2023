#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <regex>
#include <tuple>

#include "aoc.h"
#include "utils.h"

namespace aoc2 {
using namespace std;
using namespace aoc;

struct SingleDraw {
  int num;
  string color;
};

struct Game {
  int id;
  vector<vector<SingleDraw>> draws;
};

vector<Game> parseInput(const string &input) {
  auto games = input 
    | views::split('\n')  // Split by line
    | views::transform([](auto lineRg) {  // Parse each line, return a Game
      if (lineRg.empty()) return Game(0, {});
      // Get the game id
      auto n_start = ranges::find(lineRg, ' ') + 1;
      auto n_end = ranges::find(lineRg, ':');
      int gameId = stoi(string(n_start, n_end));

      auto gameDraws = ranges::subrange(n_end + 1, lineRg.end()) // Rest of the string
        | views:: split(';')  // Split by draw separator
        | views::transform([] (auto drawRg) {   // Parse each draw, return vector<SingleDraw>
            regex re(" (\\d+) (blue|red|green)");
            sregex_iterator matchBegin(drawRg.begin(), drawRg.end(), re), matchEnd{};
            vector<SingleDraw> res{};
            for (auto it = matchBegin; it != matchEnd; it++) {
              res.push_back(SingleDraw(stoi((*it)[1]), (*it)[2]));
            }
            return res;
        });
        return Game(gameId, vector(gameDraws.begin(), gameDraws.end()));
    });
  return vector(games.begin(), games.end());
}

Result solvePartOne(const string &input) {
  auto possibleGames = parseInput(input)
    | views::filter([](Game g) {   // Filter possible games, according to the rules
      const map<string, int> possible = {{"red", 12}, {"blue", 14}, {"green", 13}};
      return g.id != 0 &&
        ranges::all_of(
          g.draws | views::join,    // Flatten the draws
          [&possible](SingleDraw d) {
            return d.num <= possible.at(d.color);
          }
        );
    })
    | views::transform(&Game::id)   // Get the game id
    | views::common;
  return accumulate(possibleGames.begin(), possibleGames.end(), 0);
}

Result solvePartTwo(const string &input) {
  auto power = parseInput(input)
    | views::transform([](Game g) {   // Get the power of each game
      map<string, int> max_d = {{"red", 0}, {"blue", 0}, {"green", 0}};
      for (SingleDraw d : g.draws | views::join) {    // Flatten draws
        max_d[d.color] = max(max_d[d.color], d.num);
      }
      return max_d["red"] * max_d["blue"] * max_d["green"];
    })
    | views::common;
  return accumulate(power.begin(), power.end(), 0);
}
} // namespace aoc1
