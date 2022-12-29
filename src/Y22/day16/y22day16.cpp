#include <algorithm>
#include <bitset>
#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Graph.h"
#include "y22.h"

namespace {

using namespace std;

struct ParsingResults {
  string name;
  int32_t flow_rate;
  vector<string> neighbors;
};

vector<ParsingResults> parse(ifstream& in)
{
  vector<ParsingResults> results;
  string line;
  string neighbor(2, '.');
  string tmp;

  // First get the indices for the valves
  while (getline(in, line)) {
    stringstream ss(line);
    results.emplace_back();
    ss.ignore(6);
    ss >> results.back().name;
    ss.ignore(15);
    ss >> results.back().flow_rate;
    ss >> tmp >> tmp >> tmp >> tmp >> tmp;
    while (!ss.eof()) {
      ss >> neighbor[0];
      ss >> neighbor[1];
      results.back().neighbors.push_back(neighbor);
      ss.ignore(2);
    }
  }

  return results;
}

Graph getGraph(vector<ParsingResults> data)
{
  Graph g;
  g.edges_.resize(data.size(), data.size());
  g.node_values_.resize(data.size());
  g.edges_.fill(1e6);
  for (size_t i = 0; i < data.size(); ++i) {
    g.nodes_[data[i].name] = i;
    g.node_values_[i] = data[i].flow_rate;
  }

  for (auto& d : data) {
    int32_t x = g.nodes_[d.name];
    g.edges_.at(x, x) = 0;
    for (auto& n : d.neighbors) {
      int32_t y = g.nodes_[n];
      g.edges_.at(x, y) = 1;
    }
  }

  g.reduce();

  std::vector<std::string> rm_nodes;
  for (auto& d : data) {
    if (d.flow_rate == 0 && d.name != "AA") {
      rm_nodes.push_back(d.name);
    }
  }
  g.removeNodes(rm_nodes);

  return g;
}

struct DpState {
  struct Hash {
    size_t operator()(const DpState& s) const
    {
      using std::hash;
      return (((hash<int16_t>()(s.current_valve) ^
                (hash<int16_t>()(s.elephant_valve) << 1)) >>
               1) ^
              (hash<int16_t>()(s.time_remaining) << 1) ^
              (hash<std::bitset<60>>()(s.opened_valves) >> 1));
    }
  };

  bool operator==(const DpState& rhs) const
  {
    return current_valve == rhs.current_valve &&
           elephant_valve == rhs.elephant_valve &&
           time_remaining == rhs.time_remaining &&
           opened_valves == rhs.opened_valves;
  }

  int16_t current_valve;
  int16_t elephant_valve;
  int16_t time_remaining;
  std::bitset<60> opened_valves;
};

int32_t dp(const Graph& graph,
           std::unordered_map<DpState, int32_t, DpState::Hash>& cache,
           DpState state)
{
  if (state.time_remaining <= 1) {
    return 0;  // we dont have time to benefit from opening a valve
  }

  if (cache.contains(state)) {
    return cache[state];
  }
  
  const DpState last_state = state;
  const int16_t current_valve = state.current_valve;
  const int16_t time_remaining =
      state.opened_valves[current_valve]
          ? state.time_remaining
          : state.time_remaining - 1;  // we open here
  state.opened_valves[current_valve] = true;
  int32_t open_value = time_remaining * graph.node_values_[current_valve];
  int32_t next_value = 0;

  for (size_t i = 0; i < graph.nodes_.size(); ++i) {
    if (i == current_valve) {
      continue;  // no point in walking here
    }
    if (state.opened_valves[i] == true) {
      continue;  // already open, dont walk there
    }
    state.current_valve = i;
    state.time_remaining = time_remaining - graph.edges_.at(current_valve, i);
    next_value = max(next_value, dp(graph, cache, state));
  }

  cache[last_state] = open_value + next_value;
  return open_value + next_value;
}

inline std::string part1(ifstream& in)
{
  vector<ParsingResults> data = parse(in);
  Graph g = getGraph(data);

  std::unordered_map<DpState, int32_t, DpState::Hash> cache;
  DpState initial;
  initial.current_valve = g.nodes_["AA"];
  initial.elephant_valve = 0;
  initial.time_remaining = 30;
  initial.opened_valves[initial.current_valve] = true;

  int32_t value = dp(g, cache, initial);

  return to_string(value);
}

inline std::string part2(ifstream& in) { return to_string(0); }

}  // namespace

string y22day16(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}