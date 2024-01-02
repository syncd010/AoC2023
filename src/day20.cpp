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
#include <functional>

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
  vector<string_view> inputIds, outputIds;
  // For performance reasons, store references to input/output gates
  vector<reference_wrapper<Gate>> inputGates, outputGates;
};

using Circuit = unordered_map<string_view, Gate>;

// Represents a pulse
struct Pulse {
  Gate &sourceGate, &destGate;
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
  for (const auto &[sourceId, gate] : gates) {
    for (auto destId : gate.outputIds) {
      if (!gates.contains(destId)) {
        gates[destId] = Gate(destId, ' ', false, {}, {});
      }
      gates[destId].inputIds.push_back(sourceId);

      gates[sourceId].outputGates.push_back(gates[destId]);
      gates[destId].inputGates.push_back(gates[sourceId]);
    }
  }
  return gates;
}

// Processes a pulse, updating the circuit. Returns whether there was a switch
bool processPulse(const Pulse &pulse) {
  Gate &sourceGate = pulse.sourceGate, &destGate = pulse.destGate;
  bool switched = true;

  if (destGate.type == '%') {
    // Flip Flop, switch on down
    switched = (sourceGate.state == false);
    if (switched) destGate.state = !destGate.state;
  } else if (destGate.type == '&') {
    // NAND
    auto initialState = true;
    for (auto g : destGate.inputGates) {
      initialState = initialState & g.get().state;
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
  queue<reference_wrapper<Gate>> frontier({ gates.at("broadcaster"sv) });
  vector<Pulse> pulses{};
  while (!frontier.empty()) {
    auto &sourceGate = frontier.front().get();
    frontier.pop();

    for (auto destGate : sourceGate.outputGates) {
      Pulse pulse{sourceGate, destGate, sourceGate.state};
      pulses.push_back(pulse);
      bool switched = processPulse(pulse);
      if (switched) frontier.push(destGate);
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

  // By inspection of the input: RX is the result of a single NAND gate "LL"
  // LL gets its input from 4 gates, which form 4 distinct chains from the start. 
  // LL will be on (and RX off) only when all 4 inputs are on, so calculate the 
  // length of the 4 loops (which is prime) and return its lcm
  unordered_map<string_view, int> rxChains{};
  for (auto s : gates[gates["rx"].inputIds.front()].inputIds) {
    rxChains[s] = 0;
  }

  bool finished = false;
  int step = 0;
  while (!finished) {
    auto pulses = pushTheButton(gates);
    step++;
    // Check if we got an on state on any of the relevant gates and store the step
    for (auto s : pulses) {
      if (s.value && rxChains.contains(s.sourceGate.id) && rxChains[s.sourceGate.id] == 0) {
        rxChains[s.sourceGate.id] = step;
      }
    }
    finished = all_of(rxChains.begin(), rxChains.end(), [](auto v) { return v.second != 0; });
  }

  // LCM of length
  return foldLeft(rxChains | views::values, (int64_t)1, lcm<int64_t, int64_t>);
}
} // namespace aoc20
