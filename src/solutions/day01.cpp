#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

#include "solutions.h"

namespace {

inline Solution part1(std::ifstream& in)
{
  std::string line;
  std::int32_t calories(0);
  std::array<std::int32_t, 3> max_calories{0, 0, 0};

  while (getline(in, line)) {
    if (line.empty()) {
      auto min_calories =
          std::min_element(max_calories.begin(), max_calories.end());
      *min_calories = std::max(*min_calories, calories);
      calories = 0;
    } else {
      calories += std::stoi(line);
    }
  }

  Solution s = std::to_string(
      *std::max_element(max_calories.begin(), max_calories.end()));

  return s;
}

inline Solution part2(std::ifstream& in)
{
  std::string line;
  std::int32_t calories(0);
  std::array<std::int32_t, 3> max_calories{0, 0, 0};

  while (getline(in, line)) {
    if (line.empty()) {
      auto min_calories =
          std::min_element(max_calories.begin(), max_calories.end());
      *min_calories = std::max(*min_calories, calories);
      calories = 0;
    } else {
      calories += std::stoi(line);
    }
  }

  Solution s = std::to_string(
      std::accumulate(max_calories.begin(), max_calories.end(), 0));

  return s;
}

}  // namespace

Solution day01(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}