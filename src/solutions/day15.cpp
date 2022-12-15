#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_set>
#include <vector>

#include "Vector2D.h"
#include "solutions.h"

namespace {

static constexpr int32_t kMaxCoord = 4'000'000;

struct Sensor {
  Vector2D position;
  int32_t reach;
};

struct Range {
  Range(int32_t min = 0, int32_t max = 0) : min(min), max(max) {}

  int32_t min;
  int32_t max;

  bool touches(const Range& r) const
  {
    return r.max + 1 >= min && r.min - 1 <= max;
  }

  friend std::ostream& operator<<(std::ostream& out, const Range& v)
  {
    out << "[" << v.min << "," << v.max << "]";
    return out;
  }
};

template <bool clamp>
std::optional<Range> coveredRange(const Sensor& s, int32_t row)
{
  const int32_t y_distance = std::abs(row - s.position.y);
  const int32_t x_reach = s.reach - y_distance;
  // [min, max] is inclusive
  if (x_reach >= 0) {
    Range r(s.position.x - x_reach, s.position.x + x_reach);
    if (clamp) {
      r.min = std::clamp(r.min, 0, kMaxCoord);
      r.max = std::clamp(r.max, 0, kMaxCoord);
    }
    return r;
  } else {
    return std::nullopt;
  }
}

// Collapes the vector of ranges to the minimal set, i.e adjacend ranges are
// combined
void collapse(std::vector<Range>& v)
{
  bool done = false;
  while (!done) {
    done = true;
    for (auto l = v.begin(); l != v.end(); ++l) {
      for (auto r = v.begin(); r != v.end(); ++r) {
        if (r == l) {
          break;
        }

        if (l->touches(*r)) {
          l->min = std::min(l->min, r->min);
          l->max = std::max(l->max, r->max);
          v.erase(r);
          done = false;
          break;
        }
      }
      if (!done) {
        break;
      }
    }
  }
}

inline void parse(std::ifstream& in, std::vector<Sensor>& sensors,
                  std::unordered_set<Vector2D, Vector2DHash>& beacons)
{
  std::string line;
  while (std::getline(in, line)) {
    std::stringstream ss(line);
    Vector2D beacon;
    sensors.emplace_back();
    ss.ignore(12);
    ss >> sensors.back().position.x;
    ss.ignore(4);
    ss >> sensors.back().position.y;
    ss.ignore(25);
    ss >> beacon.x;
    ss.ignore(4);
    ss >> beacon.y;
    beacons.insert(beacon);

    Vector2D distance = sensors.back().position - beacon;
    sensors.back().reach = distance.mlength();
  }
}

inline Solution part1(std::ifstream& in)
{
  constexpr int32_t kRow = 2'000'000;

  int32_t occupied = 0;
  std::vector<Sensor> sensors;
  std::unordered_set<Vector2D, Vector2DHash> beacons;
  std::vector<Range> ranges;
  parse(in, sensors, beacons);

  for (const auto& sensor : sensors) {
    if (auto range = coveredRange<false>(sensor, kRow)) {
      ranges.push_back(*range);
    }
  }

  collapse(ranges);

  for (const auto& range : ranges) {
    occupied += (range.max - range.min + 1);
  }

  for (const auto& beacon : beacons) {
    if (beacon.y == kRow) {
      // Locations where a beacon is placed do not count
      occupied--;
    }
  }

  return std::to_string(occupied);
}

inline Solution part2(std::ifstream& in)
{
  int64_t frequency = -1;
  std::vector<Sensor> sensors;
  std::unordered_set<Vector2D, Vector2DHash> beacons;
  std::vector<Range> ranges;
  parse(in, sensors, beacons);

  for (int32_t i = 0; i <= kMaxCoord && frequency == -1; ++i) {
    for (const auto& sensor : sensors) {
      if (auto range = coveredRange<true>(sensor, i)) {
        ranges.push_back(*range);
      }
    }

    collapse(ranges);

    if (ranges.size() == 1) {
      if (ranges[0].min > 0) {
        frequency = i;
      } else if (ranges[0].max < kMaxCoord) {
        frequency =
            static_cast<int64_t>(kMaxCoord) * static_cast<int64_t>(kMaxCoord);
        frequency += i;
      }
    } else if (ranges.size() == 2) {
      frequency =
          static_cast<int64_t>(std::min(ranges[0].max, ranges[1].max)) + 1;
      frequency *= kMaxCoord;
      frequency += i;
    } else {
      std::cerr << "Something went wrong..." << std::endl;
    }
    ranges.clear();
  }

  return std::to_string(frequency);
}

}  // namespace

Solution day15(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}