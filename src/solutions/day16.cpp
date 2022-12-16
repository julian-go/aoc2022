#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "solutions.h"

namespace {

using namespace std;

constexpr int32_t dt_walk = 1;
constexpr int32_t dt_open = 1;

struct Valve {
  string name;
  bool open = false;
  int32_t flow_rate = 0;
  vector<string> neighbors;
};

struct PairHash {
  size_t operator()(const pair<string, int32_t>& v) const
  {
    return (v.second << 16) | (v.first[1] << 8) | (v.first[0]);
  }
};

struct Cave {
  unordered_map<string, shared_ptr<Valve>> valves;
  unordered_map<pair<string, int32_t>, pair<bool, int32_t>, PairHash> values;
};

unique_ptr<Cave> parse(ifstream& in)
{
  unique_ptr<Cave> c = make_unique<Cave>();
  string line;
  string neighbor(2, ' ');
  string tmp;
  while (getline(in, line)) {
    stringstream ss(line);
    shared_ptr<Valve> v = make_shared<Valve>();

    ss.ignore(6);
    ss >> v->name;
    ss.ignore(15);
    ss >> v->flow_rate;
    ss >> tmp >> tmp >> tmp >> tmp >> tmp;
    while (!ss.eof()) {
      ss >> neighbor[0];
      ss >> neighbor[1];
      v->neighbors.push_back(neighbor);
      ss.ignore(2);
    }

    // If the flow rate is zero, just pretend its already open
    v->open = v->flow_rate == 0;

    c->valves[v->name] = v;
  }

  return c;
}

inline pair<bool, int32_t> remainingValue(Cave& c, const string& valve,
                                          int32_t time_remaining,
                                          int32_t indent)
{
  if (c.values.contains(make_pair(valve, time_remaining))) {
    //for (int i = 0; i < indent; ++i) {
    //  cout << "    ";
    //}
    //cout << valve << " -> " << time_remaining << " (returned early)" << std::endl;
    return c.values.at(make_pair(valve, time_remaining));
  }

  const shared_ptr<Valve> v = c.valves.at(valve);
  //for (int i = 0; i < indent; ++i) {
  //  cout << "    ";
  //}
  //cout << valve << " -> " << time_remaining << std::endl;

  const bool can_open = time_remaining > dt_open + 1 && !v->open;
  const bool can_walk = time_remaining > dt_walk + dt_open + 1;
  const bool can_open_and_walk =
      time_remaining > dt_walk + 2 * dt_open + 1 && !v->open;

  int32_t walk_value = 0;
  if (can_walk) {
    // there is still time to open another valve and benefit, otherwise no value
    for (const string& neighbor : v->neighbors) {
      walk_value =
          max(walk_value,
              remainingValue(c, neighbor, time_remaining - dt_walk, indent + 1)
                  .second);
    }
  }

  int32_t open_value = 0;
  if (can_open_and_walk) {
    if (time_remaining > dt_walk + dt_open + dt_open) {
      for (const string& neighbor : v->neighbors) {
        open_value =
            max(open_value,
                remainingValue(c, neighbor, time_remaining - dt_walk - dt_open,
                               indent + 1)
                    .second);
      }
      open_value += (time_remaining - dt_open) * v->flow_rate;
    }
  } else if (can_open) {
    open_value += (time_remaining - dt_open) * v->flow_rate;
  }

  if (open_value >= walk_value) {
    c.values[make_pair(valve, time_remaining)] = make_pair(true, open_value);
    return make_pair(true, open_value);
  } else {
    c.values[make_pair(valve, time_remaining)] = make_pair(true, walk_value);
    return make_pair(true, walk_value);
  }
}

inline Solution part1(ifstream& in)
{
  unique_ptr<Cave> c = parse(in);
  string current_valve = "AA";
  int32_t total_pressure = 0;
  int32_t delta_pressure = 0;
  int32_t time_remaining = 30;

  while (time_remaining > 0) {
    const shared_ptr<Valve> v = c->valves.at(current_valve);
    int32_t value = 0;
    bool open = false;
    string next_valve;
    for (const string& neighbor : v->neighbors) {
      auto action_value =
          remainingValue(*c, neighbor, time_remaining - dt_walk, 0);
      if (action_value.second > value) {
        open = action_value.first;
        value = action_value.second;
        next_valve = neighbor;
      }
    }

    total_pressure += delta_pressure;
    time_remaining--;

    {
      if (next_valve.empty()) {
        cout << std::format("T{}. Did not move. Released {} pressure.",
                            time_remaining + 1, delta_pressure)
             << std::endl;
      } else {
        current_valve = next_valve;
        cout << std::format("T{}. Moved to {}. Released {} pressure.",
                            time_remaining + 1, next_valve, delta_pressure)
             << std::endl;
      }
    }

    total_pressure += delta_pressure;
    time_remaining--;

    {
      if (open) {
        cout << std::format("T{}. Opened {}. Released {} pressure.",
                            time_remaining + 1, next_valve, delta_pressure)
             << std::endl;
      }
    }

    if (open) {
      c->valves.at(next_valve)->open = true;
      delta_pressure += c->valves.at(next_valve)->flow_rate;
    }

    c->values.clear();
  }

  return to_string(total_pressure);
}

inline Solution part2(ifstream& in) { return to_string(0); }

}  // namespace

Solution day16(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}