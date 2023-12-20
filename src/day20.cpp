#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <queue>

#include "aoc.h"
#include "utils.h"

namespace aoc20 {

using namespace std;
using namespace aoc;

// Represents a gate with its state, type and inputs/outputs
struct Gate {
  string_view id;
  char type;
  bool state;
  vector<string_view> inputs, outputs;
};

using Circuit = unordered_map<string_view, Gate>;

// Represents a pulse
struct Pulse {
  string_view sourceId, destId;
  bool value;
};

// Creates a map of the circuit
Circuit parseInput(const string &input) {
  auto lines = toVector(input | splitString('\n'));
  Circuit gates{};

  // Create gates and output circuit
  for (auto line : lines) {
    auto splits = toVector(line | splitString(" -> "s));
    auto id = (splits[0][0] == '%' || splits[0][0] == '&') ? splits[0].substr(1) : splits[0];
    auto outputs = toVector(splits[1] | splitString(", "s));
    gates[id] = Gate(id, splits[0][0], false, {}, outputs);
  }

  // Fill gate inputs
  for (auto [sourceId, gate] : gates) {
    for (auto destId : gate.outputs) {
      if (!gates.contains(destId)) {
        gates[destId] = Gate(destId, ' ', false, {}, {});
      }
      gates[destId].inputs.push_back(sourceId);
    }
  }

  return gates;
}

// Processes a pulse, updating the circuit. Returns whether there was a switch
bool processPulse(const Pulse &pulse, Circuit &gates) {
  Gate &sourceGate = gates[pulse.sourceId], &destGate = gates[pulse.destId];
  bool initialState = destGate.state, 
    switched = true;

  if (destGate.type == '%') {
    // Flip Flop, switch on down
    switched = (sourceGate.state == false);
    destGate.state = (switched)? !initialState : initialState;
  } else if (destGate.type == '&') {
    // NAND
    initialState = true;
    for (auto id : destGate.inputs) {
      initialState = initialState & gates[id].state;
    }
    destGate.state = !initialState;
  } else {
    // Passthrough, not relevant
    destGate.state = sourceGate.state;
  }
  return switched;
}

// Runs a cycle of a button push
// Updates the circuit with the state and returns the pulses generated
vector<Pulse> pushTheButton(Circuit &gates) {
  queue<string_view> frontier({"broadcaster"sv});
  vector<Pulse> pulses{};
  while (!frontier.empty()) {
    auto sourceId = frontier.front();
    frontier.pop();
    if (!gates.contains(sourceId)) continue;

    for (auto destId : gates.at(sourceId).outputs) {
      Pulse pulse{sourceId, destId, gates[sourceId].state};
      pulses.push_back(pulse);
      bool switched = processPulse(pulse, gates);
      if (switched) frontier.push(destId);
    }
  }
  return pulses;
}

Result solvePartOne(const string &input) {
  auto gates = parseInput(input);

  // 1000 button pushes
  int64_t low = 0, high = 0;
  for (int p = 0; p < 1000; p++) {
    low++;
    auto pulses = pushTheButton(gates);
    for (auto s : pulses) {
      low += !s.value;
      high += s.value;
    }
  }
  return low * high;
}

Result solvePartTwo(const string &input) {
  auto gates = parseInput(input);

  // Test data doesn't contain RX
  if (!gates.contains("rx")) return 0;

  // By inspection of the input:
  // RX is the result of a single NAND gate "LL"
  // LL gets its input from 4 gates, which i assume form 4 distinct chains from 
  // the start. LL will be on (and RX off) only when all 4 inputs are on, so,
  // calculate the length of the 4 loops (which is prime) and return its mcm
  unordered_map<string_view, int> rxChains{};
  for (auto s : gates[gates["rx"].inputs.front()].inputs) {
    rxChains[s] = 0;
  }

  bool finished = false;
  int step = 0;
  while (!finished) {
    auto pulses = pushTheButton(gates);
    step++;
    // Check if we got an on state on any of the relevant gates and store the step
    for (auto s : pulses) {
      if (rxChains.contains(s.sourceId) && s.value && rxChains[s.sourceId] == 0) {
        rxChains[s.sourceId] = step;
      }
    }
    finished = all_of(rxChains.begin(), rxChains.end(), [](auto v) { return v.second != 0; });
  }

  // LCM of length
  return foldLeft(rxChains | views::values, (int64_t)1, lcm<int64_t, int64_t>);
}
} // namespace aoc20
