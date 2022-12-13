#include <sstream>
#include <string>

#include "solutions.h"

namespace {

struct Range {
  bool contains(const Range& other)
  {
    return other.min >= min && other.max <= max;
  }

  bool overlaps(const Range& other)
  {
    return other.max >= min && other.min <= max;
  }

  std::int32_t min;
  std::int32_t max;
};

inline Solution part1(std::ifstream& in)
{
  std::int32_t duplicates(0);
  std::string line;
  char delim;
  std::array<Range, 2> ranges;

  while (getline(in, line)) {
    std::stringstream ss(line);
    ss >> ranges[0].min >> delim >> ranges[0].max;
    ss >> delim;
    ss >> ranges[1].min >> delim >> ranges[1].max;
    if (ranges[0].contains(ranges[1]) || ranges[1].contains(ranges[0])) {
      duplicates++;
    }
  }

  Solution s = std::to_string(duplicates);
  return s;
}

inline Solution part2(std::ifstream& in)
{
  std::int32_t duplicates(0);
  std::string line;
  char delim;
  std::array<Range, 2> ranges;

  while (in.is_open() && getline(in, line)) {
    std::stringstream ss(line);
    ss >> ranges[0].min >> delim >> ranges[0].max;
    ss >> delim;
    ss >> ranges[1].min >> delim >> ranges[1].max;
    if (ranges[0].overlaps(ranges[1])) {
      duplicates++;
    }
  }

  Solution s = std::to_string(duplicates);
  return s;
}

}  // namespace

Solution day04(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}