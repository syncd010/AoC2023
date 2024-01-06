#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "aoc.h"
#include "utils.h"

namespace aoc19 {

using namespace std;
using namespace aoc;

struct Rule {
  char lhs, op;
  int rhs;
  string_view next;
};

using Workflows = unordered_map<string_view, vector<Rule>>;
using Part = unordered_map<char, int>;

pair<Workflows, vector<Part>> parseInput(const string &input) {
  auto splits = input 
    | splitString('\n', true);

  // Somewhat messy but it is what it is
  auto workflows = Workflows{};
  auto parts = vector<Part>{};
  bool inRules = true;
  for (auto line : splits) {
    if (line.empty()) {
      if (inRules) inRules = false;
      continue;
    }
    if (inRules) {
      // Parsing rules section of the input
      auto sep = line.find('{');
      auto name = line.substr(0, sep);
      auto spec = line.substr(sep + 1, line.size() - sep - 2);
      auto rules = vector<Rule>{};
      for (auto ruleStr : spec | splitString(',')) {
        auto sep = ruleStr.find(':');
        if (sep != ruleStr.npos) {
          rules.push_back(Rule{ruleStr[0], ruleStr[1], ston<int>(ruleStr.substr(2, sep - 2)), ruleStr.substr(sep + 1)});
        } else {
          rules.push_back(Rule{' ', ' ', 0, ruleStr});
        }
      }
      workflows[name] = rules;
    } else {
      // Parsing parts section of the input
      auto part = Part{};
      for (auto p : line.substr(1, line.size() - 2) | splitString(',')) {
        part[p[0]] = ston<int>(p.substr(2));
      }
      parts.push_back(part);
    }
  }
  return make_pair(workflows, parts);
}

// Evaluate an operation
inline bool eval(int lhs, char op, int rhs) {
  switch (op) {
    case ' ' : return true;
    case '>' : return lhs > rhs;
    case '<' : return lhs < rhs;
    default : return lhs == rhs;
  }
}

Result solvePartOne(const string &input) {
  auto [workflows, parts] = parseInput(input);

  // Pass each part through the workflow collecting the accepted ones
  auto accepted = vector<Part>{};
  for (const auto &part : parts) {
    bool finished = false;
    auto wf = workflows["in"];
    while(!finished) {
      for (const auto &rule : wf) {
        // Eval rule on the part
        if (rule.op == ' ' || eval(part.at(rule.lhs), rule.op, rule.rhs)) {
          if (rule.next == "A") accepted.push_back(part);
          finished = (rule.next == "A" || rule.next == "R");
          if (!finished) wf = workflows[rule.next];
          break;
        }
      }
    }
  }

  return foldLeft(accepted | views::join | views::values, (int64_t)0, std::plus());
}

// Represent an hypercube
using Hypercube = unordered_map<char, pair<int, int>>;

// Splices the hypercube along the rule dimension
pair<Hypercube, Hypercube> spliceCube(Hypercube cube, Rule rule) {
  auto cubeInv = cube;
  auto &limit = cube[rule.lhs], &limitInv = cubeInv[rule.lhs];
  if (rule.op == '>') {
    limit.first = max(limit.first, rule.rhs + 1);
    limitInv.second = min(limitInv.second, rule.rhs + 1);
  } else if (rule.op == '<') {
    limit.second = min(limit.second, rule.rhs);
    limitInv.first = max(limitInv.first, rule.rhs);
  }
  return make_pair(cube, cubeInv);
}

Result solvePartTwo(const string &input) {
  auto [workflows, parts] = parseInput(input);
  auto frontier = vector<pair<string_view, Hypercube>>{ { 
  {"in", {
    { 'x', { 1, 4001 } },
    { 'm', { 1, 4001 } },
    { 'a', { 1, 4001 } },
    { 's', { 1, 4001 } },
  } } } };

  // Loop through unspliced 
  int64_t combs = 0;
  while (!frontier.empty()) {
    auto [wfId, hypercube] = frontier.back();
    frontier.pop_back();

    if (wfId == "R") continue;
    if (wfId == "A") {
      combs += foldLeft(hypercube | views::values, (int64_t)1, [](auto prev, auto v) { return prev * (v.second - v.first); } );
      continue;
    }

    // For each rule on the workflow, splice the hypercube, sending the first 
    // splice (that obeys the rule) to the frontier and the other (inverted)
    // continues checking the other rules
    for (const auto &rule : workflows[wfId]) {
      if (rule.op == ' ') {
        frontier.push_back(make_pair(rule.next, hypercube));
        break;
      }

      auto [hc1, hc2] = spliceCube(hypercube, rule);
      frontier.push_back(make_pair(rule.next, hc1));
      hypercube = hc2;
    }
  }

  return combs;
}
} // namespace aoc19
