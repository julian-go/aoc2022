#include <array>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "y22.h"

namespace {

using namespace std;

enum class Bot { kOre = 0, kClay = 1, kObsidian = 2, kGeode = 3 };

struct Recipe {
  int8_t ore = 0;
  int8_t clay = 0;
  int8_t obsidian = 0;
};

struct Blueprint {
  Recipe ore_bot;
  Recipe clay_bot;
  Recipe obsidian_bot;
  Recipe geode_bot;
};

struct StockPile {
  explicit constexpr StockPile(int32_t ore = 0) : ore(ore) {}

  int8_t ore;
  int8_t clay = 0;
  int8_t obsidian = 0;
  int8_t geodes = 0;

  int8_t ore_bots = 1;
  int8_t clay_bots = 0;
  int8_t obsidian_bots = 0;
  int8_t geode_bots = 0;
};

StockPile build(const Blueprint& b, StockPile s, Bot bot,
                int32_t time_remaining)
{
  for (int32_t i = 0; i < 25 - time_remaining; ++i) {
    if (time_remaining != -1) cout << "    ";
  }
  if (time_remaining != -1) cout << time_remaining;
  Recipe r;
  switch (bot) {
    case Bot::kOre:
      if (time_remaining != -1) cout << ": Build: ORE ";
      r = b.ore_bot;
      s.ore_bots += 1;
      break;
    case Bot::kClay:
      if (time_remaining != -1) cout << ": Build: CLY ";
      r = b.clay_bot;
      s.clay_bots += 1;
      break;
    case Bot::kObsidian:
      if (time_remaining != -1) cout << ": Build: OBS ";
      r = b.obsidian_bot;
      s.obsidian_bots += 1;
      break;
    case Bot::kGeode:
      if (time_remaining != -1) cout << ": Build: GEO ";
      r = b.geode_bot;
      s.geode_bots += 1;
      break;
  }

  s.ore -= r.ore;
  s.clay -= r.clay;
  s.obsidian -= r.obsidian;

  int32_t ore = s.ore, clay = s.clay, obsidian = s.obsidian, geodes = s.geodes,
          obots = s.ore_bots, cbots = s.clay_bots, obbots = s.obsidian_bots,
          gbots = s.geode_bots;
  if (time_remaining != -1)
    cout << "MAT: " << ore << "|" << clay << "|" << obsidian << "|" << geodes
         << " BOTS: " << obots << "|" << cbots << "|" << obbots << "|" << gbots
         << endl;

  return s;
}

inline bool canBuild(const Blueprint& b, const StockPile& s, Bot bot)
{
  Recipe r;
  switch (bot) {
    case Bot::kOre:
      r = b.ore_bot;
      break;
    case Bot::kClay:
      r = b.clay_bot;
      break;
    case Bot::kObsidian:
      r = b.obsidian_bot;
      break;
    case Bot::kGeode:
      r = b.geode_bot;
      break;
  }
  return s.ore >= r.ore && s.clay >= r.clay && s.obsidian >= r.obsidian;
}

inline bool shouldBuild(const Blueprint& b, const StockPile& s,
                        int8_t time_remaining, Bot bot)
{
  if (bot == Bot::kOre) {
    const int8_t max_consumer =
        max(b.ore_bot.ore,
            max(b.clay_bot.ore, max(b.obsidian_bot.ore, b.geode_bot.ore)));
    return (s.ore_bots < b.ore_bot.ore || s.ore_bots < b.clay_bot.ore ||
            s.ore_bots < b.obsidian_bot.ore || s.ore_bots < b.geode_bot.ore) &&
           (s.ore / max_consumer < time_remaining);
  }

  if (bot == Bot::kClay) {
    return (s.clay_bots < b.obsidian_bot.clay) &&
           (s.clay / b.obsidian_bot.clay < time_remaining);
  }

  if (bot == Bot::kObsidian) {
    return (s.obsidian_bots < b.geode_bot.obsidian) &&
           (s.obsidian / b.geode_bot.obsidian < time_remaining);
  }

  if (bot == Bot::kGeode) {
    return true;
  }
}

StockPile mine(StockPile s)
{
  s.ore += s.ore_bots;
  s.clay += s.clay_bots;
  s.obsidian += s.obsidian_bots;
  s.geodes += s.geode_bots;
  return s;
}

using ValueCache = unordered_map<int64_t, int32_t>;

inline int64_t hash(const StockPile& s, int8_t time)
{
  constexpr int8_t mask = 0x7F;  // dont care about sign bit
  int64_t h = 0;
  int32_t offset = 0;
  h |= (static_cast<int64_t>(s.ore & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.clay & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.obsidian & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.geodes & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.ore_bots & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.clay_bots & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.obsidian_bots & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(s.geode_bots & mask) << offset);

  offset += 7;
  h |= (static_cast<int64_t>(time & mask) << offset);

  return h;
}

vector<Blueprint> parse(std::ifstream& in)
{
  vector<Blueprint> blueprints;
  string line;
  string tmp;
  int32_t tmp_int;

  while (getline(in, line)) {
    stringstream ss(line);
    Blueprint blueprint;

    // Blueprint X: Each ore robot costs
    ss >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp;
    ss >> tmp_int;
    blueprint.ore_bot.ore = tmp_int;
    // ore. Each clay robot costs
    ss >> tmp >> tmp >> tmp >> tmp >> tmp;
    ss >> tmp_int;
    blueprint.clay_bot.ore = tmp_int;
    // ore. Each obsidian robot costs
    ss >> tmp >> tmp >> tmp >> tmp >> tmp;
    ss >> tmp_int;
    blueprint.obsidian_bot.ore = tmp_int;
    // ore and
    ss >> tmp >> tmp;
    ss >> tmp_int;
    blueprint.obsidian_bot.clay = tmp_int;
    // clay. Each geode robot costs
    ss >> tmp >> tmp >> tmp >> tmp >> tmp;
    ss >> tmp_int;
    blueprint.geode_bot.ore = tmp_int;
    // ore and
    ss >> tmp >> tmp;
    ss >> tmp_int;
    blueprint.geode_bot.obsidian = tmp_int;
    // obsidian.

    blueprints.push_back(blueprint);
  }

  return blueprints;
}

int32_t computeValue(const Blueprint& b, ValueCache& cache, int32_t& best,
                     StockPile s, int8_t time_remaining,
                     array<bool, 4> must_not_build)
{
  int64_t h = hash(s, time_remaining);
  // if (cache.contains(h)) {
  //   return cache[h];
  // }

  if (time_remaining == 0) {
    // cache[h] = s.geodes;
    return s.geodes;
  }

  if (s.geodes + s.geode_bots * time_remaining +
          (time_remaining * (time_remaining + 1) / 2) <
      best) {
    return 0;
  }

  int32_t value = 0;
  StockPile new_pile = mine(s);

  array<bool, 4> did_not_build = {false, false, false, false};
  for (const Bot bot : {Bot::kGeode, Bot::kObsidian, Bot::kClay, Bot::kOre}) {
    if (canBuild(b, s, bot)) {
      did_not_build[static_cast<int>(bot)] =
          !shouldBuild(b, s, time_remaining, bot) ||
          must_not_build[static_cast<int>(bot)];
      if (shouldBuild(b, s, time_remaining, bot) &&
          !must_not_build[static_cast<int>(bot)]) {
        const StockPile build_pile = build(b, new_pile, bot, -1);
        const int32_t tmp =
            computeValue(b, cache, best, build_pile, time_remaining - 1,
                         {false, false, false, false});
        value = max(value, tmp);
        best = max(best, value);
      }
    }
  }

  // The value of doing nothing
  value = max(value, computeValue(b, cache, best, new_pile, time_remaining - 1,
                                  did_not_build));
  best = max(best, value);

  return value;
}

inline string part1(std::ifstream& in)
{
  /*  constexpr int8_t kSkipTime = 0;
    constexpr int8_t kTotalTime = 24 - kSkipTime;
    constexpr StockPile kStartingPile = StockPile(kSkipTime);

    vector<Blueprint> blueprints = parse(in);
    int32_t total_quality = 0;
    for (size_t i = 0; i < blueprints.size(); ++i) {
      int32_t best = 0;
     ValueCache cache;
     array<bool, 4> did_not_build = {false, false, false, false};
      int32_t value = computeValue(blueprints[i], cache, best, kStartingPile,
                                  kTotalTime, did_not_build);
      cout << "done " << value << endl;
      total_quality += value * (i + 1);
    }
    return std::to_string(total_quality);*/
  return "";
}

inline string part2(std::ifstream& in)
{
  constexpr int8_t kSkipTime = 0;
  constexpr int8_t kTotalTime = 32 - kSkipTime;
  constexpr StockPile kStartingPile = StockPile(kSkipTime);

  vector<Blueprint> blueprints = parse(in);
  int32_t total_quality = 0;
  for (size_t i = 0; i < 3; ++i) {
    int32_t best = 0;
    ValueCache cache;
    array<bool, 4> did_not_build = {false, false, false, false};
    int32_t value = computeValue(blueprints[i], cache, best, kStartingPile,
                                 kTotalTime, did_not_build);
    cout << "done " << value << endl;
    total_quality += value * (i + 1);
  }
  return std::to_string(total_quality);
  return "";
}

}  // namespace

string y22day19(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}