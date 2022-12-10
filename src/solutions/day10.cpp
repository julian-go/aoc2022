#include "solutions.h"

#include "Device.h"

namespace {

inline Solution part1(std::ifstream& in)
{
  Device d;
  d.processCommandStream(in);
  Solution s = std::to_string(d.getTotalSignalStrength());
  return s;
}

inline Solution part2(std::ifstream& in)
{
  Device d;
  d.processCommandStream(in);
  return d.render(false);
}

}  // namespace

Solution day10(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}