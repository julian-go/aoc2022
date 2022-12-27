#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "y22.h"

namespace {

using namespace std;

constexpr int32_t dt_open = 1;

struct Neighbor {
  explicit Neighbor(int16_t id = 0, int16_t dt_walk = 1)
      : id(id), dt_walk(dt_walk)
  {
  }
  int16_t id;
  int16_t dt_walk;
};

struct Valve {
  int16_t id;
  bool open = false;
  int32_t flow_rate = 0;
  vector<Neighbor> neighbors;
};

struct PairHash {
  size_t operator()(const pair<int16_t, int32_t>& v) const
  {
    return (v.first << 32) | v.second;
  }
};

struct Cave {
  using ValveTimeMap = unordered_map<int64_t, pair<bool, int32_t>>;

  vector<Valve> valves;
  ValveTimeMap value_cache;

  // array<vector<int16_t, unordered_map<int32_t, pair<bool, int32_t>>>, 30>
  // cache;
};

void collapseGraph(vector<Valve>& valves)
{
  bool done = false;
  while (!done) {
    done = true;
    for (auto& valve : valves) {
      if (valve.id != 0 && valve.flow_rate == 0) {
        continue;
      }
      // cout << "START Valve " << valve.id << " has neighbors: ";
      // for (auto it = valve.neighbors.begin(); it != valve.neighbors.end();
      //      ++it) {
      //   cout << it->id << "(" << it->dt_walk << "), ";
      // }
      // cout << endl;
      vector<Neighbor> tmp;
      for (auto it = valve.neighbors.begin(); it != valve.neighbors.end();
           ++it) {
        Valve neighbor = valves[it->id];
        if (neighbor.id == valve.id) {
          continue;
        }
        if (neighbor.flow_rate == 0) {
          size_t before_size = tmp.size();
          for_each(neighbor.neighbors.begin(), neighbor.neighbors.end(),
                   [&it](auto& n) { n.dt_walk += it->dt_walk; });
          // Lambda-ception: Copy only those neighbors that are not already
          // present with an equal or shorter walking distance
          copy_if(neighbor.neighbors.begin(), neighbor.neighbors.end(),
                  std::back_inserter(tmp), [&valve](auto& to_copy) {
                    // cout << "Want to add " << to_copy.id << "("
                    //      << to_copy.dt_walk << ")" << std::endl;
                    return find_if(
                               valve.neighbors.begin(), valve.neighbors.end(),
                               [&to_copy](auto& to_check) {
                                 // cout << "         vs " << to_check.id << "("
                                 //      << to_check.dt_walk << ")" <<
                                 //      std::endl;
                                 return (to_check.id == to_copy.id &&
                                         to_check.dt_walk <= to_copy.dt_walk);
                               }) == valve.neighbors.end();
                  });
          done = done && before_size == tmp.size();
        }
        tmp.push_back(*it);
      }
      valve.neighbors = tmp;
      /*if (valve.id == 9) {
        cout << "Valve " << valve.id << " has neighbors: ";
        for (auto it = valve.neighbors.begin(); it != valve.neighbors.end();
             ++it) {
          cout << it->id << "(" << it->dt_walk << "), ";
        }
        cout << endl;
      }*/
    }
  }

  for (auto& valve : valves) {
    //cout << "Valve " << valve.id << " has neighbors: ";
    erase_if(valve.neighbors, [&valves](auto& n) {
      // cout << "checking " << n.id << " with flow rate "
      //      << valves[n.id].flow_rate << endl;
      return valves[n.id].flow_rate == 0;
    });
   /* for (auto it = valve.neighbors.begin(); it != valve.neighbors.end(); ++it) {
      cout << it->id << "(" << it->dt_walk << "), ";
    }
    cout << endl;*/
  }
  //cout << endl;s

  vector<Valve> shortened;
  int32_t num_removed = 0;
  for (int32_t i = 1; i < valves.size(); ++i) {
    if (valves[i].flow_rate == 0) {
      for (int32_t j = 0; j < valves.size(); ++j) {
        for (auto& n : valves[j].neighbors) {
          if (n.id > i - num_removed) n.id--;
        }
        if (j > i) valves[j].id--;
      }
      num_removed++;
    }
  }

  //for (auto& valve : valves) {
  //  cout << "Valve " << valve.id << " has neighbors: ";
  //  for (auto it = valve.neighbors.begin(); it != valve.neighbors.end(); ++it) {
  //    cout << it->id << "(" << it->dt_walk << "), ";
  //  }
  //  cout << endl;
  //}
  //cout << endl;

  erase_if(valves, [](auto& v) { return v.flow_rate == 0 && v.id != 0; });

  //for (auto& valve : valves) {
  //  cout << "Valve " << valve.id << " has neighbors: ";
  //  for (auto it = valve.neighbors.begin(); it != valve.neighbors.end(); ++it) {
  //    cout << it->id << "(" << it->dt_walk << "), ";
  //  }
  //  cout << endl;
  //}
}

unique_ptr<Cave> parse(ifstream& in)
{
  struct ParsingResults {
    string name;
    int32_t flow_rate;
    vector<string> neighbors;
  };

  unique_ptr<Cave> c = make_unique<Cave>();
  vector<ParsingResults> results;
  string line;
  string neighbor(2, ' ');
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
    // cout << results.back().name;
  }
  sort(results.begin(), results.end(),
       [](const auto& l, const auto& r) { return l.name < r.name; });

  vector<Valve> valves;
  for (int i = 0; i < results.size(); ++i) {
    valves.emplace_back();
    valves.back().id = i;
    valves.back().flow_rate = results[i].flow_rate;
    for (const auto& n_name : results[i].neighbors) {
      auto it = find_if(results.begin(), results.end(),
                        [n_name](const auto& r) { return r.name == n_name; });
      const Neighbor n(it - results.begin(), 1);
      valves.back().neighbors.push_back(n);
    }
  }

  collapseGraph(valves);

  c->valves = valves;

  return c;
}

pair<bool, int32_t> remainingValue(Cave& c, const int16_t valve,
                                   std::vector<bool> open,
                                   int8_t time_remaining, int32_t indent)
{
  Valve& v = c.valves.at(valve);
  std::vector<bool> open_old = open;
  int64_t hash = static_cast<int64_t>(time_remaining) << 56;
  hash |= static_cast<int64_t>(valve) << 40;
  for (int32_t i = 0; i < open.size(); ++i) {
    if (open_old[i]) {
      hash |= (1ULL << i);
    }
  }
  if (all_of(open.begin(), open.end(), [](bool b) { return b; })) {
    // cout << "here" << endl;
  }
  if (c.value_cache.contains(hash)) {
    return c.value_cache.at(hash);
  }

  // for (int i = 0; i < indent; ++i) {
  //   cout << "    ";
  // }
  // cout << valve << " -> " << static_cast<int16_t>(time_remaining) <<
  // std::endl;

  int32_t walk_value = 0;
  int32_t open_value = 0;
  int32_t open2_value = 0;

  for (const Neighbor& n : v.neighbors) {
    const bool can_open = (time_remaining > dt_open) && !open[v.id];
    const bool can_walk = time_remaining > (n.dt_walk + dt_open);
    const bool can_open_and_walk =
        (time_remaining > (n.dt_walk + 2 * dt_open)) && !open[v.id];

    if (can_walk) {
      // there is still time to open another valve and benefit, otherwise no
      // value
      // cout << static_cast<int>(time_remaining) << endl;
      walk_value = max(
          walk_value,
          remainingValue(c, n.id, open, time_remaining - n.dt_walk, indent + 1)
              .second);
      // cout << static_cast<int>(time_remaining) << endl;
    }

    if (can_open_and_walk) {
      open[valve] = true;
      // cout << static_cast<int>(time_remaining) << endl;
      int32_t tmp;
      tmp = remainingValue(c, n.id, open, time_remaining - n.dt_walk - dt_open,
                           indent + 1)
                .second;
      open[valve] = false;
      tmp += (time_remaining - dt_open) * v.flow_rate;
      open_value = max(open_value, tmp);
    } else if (can_open) {
      open_value = (time_remaining - dt_open) * v.flow_rate;
    }
  }

  if (open_value >= walk_value) {
    int64_t hash = static_cast<int64_t>(time_remaining) << 56;
    hash |= static_cast<int64_t>(valve) << 40;
    for (int32_t i = 0; i < open.size(); ++i) {
      if (open_old[i]) {
        hash |= (1ULL << i);
      }
    }
    c.value_cache[hash] = make_pair(true, open_value);
    return make_pair(true, open_value);
  } else {
    int64_t hash = static_cast<int64_t>(time_remaining) << 56;
    hash |= static_cast<int64_t>(valve) << 40;
    for (int32_t i = 0; i < open.size(); ++i) {
      if (open_old[i]) {
        hash |= (1ULL << i);
      }
    }
    c.value_cache[hash] = make_pair(false, walk_value);
    return make_pair(false, walk_value);
  }
  return make_pair(false, 0);
}

inline std::string part1(ifstream& in)
{
  unique_ptr<Cave> c = parse(in);
  int16_t current_valve = 0;
  int32_t total_pressure = 0;
  int32_t delta_pressure = 0;
  int8_t time_remaining = 30;
  vector<bool> open_valves(c->valves.size(), false);
  open_valves[0] = true;

  //for (auto valve : c->valves) {
  //  std::cout << format("ID: {}, Flow-Rate: {}", valve.id, valve.flow_rate)
  //            << endl;
  //  for (Neighbor neighbor : valve.neighbors) {
  //    std::cout << format("Neighbor: {}, Distance: {}", neighbor.id,
  //                        neighbor.dt_walk)
  //              << endl;
  //  }
  //}

  while (time_remaining > 0) {
    const Valve& v = c->valves.at(current_valve);
    int32_t value = 0;
    bool open = false;
    int16_t next_valve = -1;
    int8_t dt = 0;
    for (const Neighbor& neighbor : v.neighbors) {
      auto action_value = remainingValue(*c, neighbor.id, open_valves,
                                         time_remaining - neighbor.dt_walk, 0);
      if (action_value.second > value) {
        open = action_value.first;
        value = action_value.second;
        next_valve = neighbor.id;
        dt = neighbor.dt_walk;
      }
    }

    {
      if (next_valve == -1) {
        total_pressure += delta_pressure;
        time_remaining--;
      /*  cout << std::format("T{}. Did not move. Released {} pressure.",
                            time_remaining + 1, delta_pressure)
             << std::endl;*/
      } else {
        total_pressure += delta_pressure * dt;
        time_remaining -= dt;
        current_valve = next_valve;
        //cout << std::format("T{}. Moved to {}. Released {} pressure.",
        //                    time_remaining + 1, next_valve, delta_pressure * dt)
        //     << std::endl;
      }
    }

    {
      if (open) {
        total_pressure += delta_pressure;
        time_remaining--;
        //cout << std::format("T{}. Opened {}. Released {} pressure.",
        //                    time_remaining + 1, next_valve, delta_pressure)
        //     << std::endl;
      }
    }

    if (open) {
      open_valves[next_valve] = true;
      c->valves.at(next_valve).open = true;
      delta_pressure += c->valves.at(next_valve).flow_rate;
    }
  }

  return to_string(total_pressure);
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