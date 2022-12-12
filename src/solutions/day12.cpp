#include "solutions.h"

#include "Device.h"

namespace {

inline Solution part1(std::ifstream& in)
{
  Device d;
  d.height_map.parseStream(in);
  return std::to_string(d.height_map.costStartToEnd());
}

inline Solution part2(std::ifstream& in)
{
  Device d;
  d.height_map.parseStream(in);
  return std::to_string(d.height_map.costShortestHike());
}

}  // namespace

Solution day12(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}