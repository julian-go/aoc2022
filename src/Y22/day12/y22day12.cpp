#include "y22.h"

#include "HeightMap.h"

namespace {

inline std::string part1(std::ifstream& in)
{
  HeightMap h;
  h.parseStream(in);
  return std::to_string(h.costStartToEnd());
}

inline std::string part2(std::ifstream& in)
{
  HeightMap h;
  h.parseStream(in);
  return std::to_string(h.costShortestHike());
}

}  // namespace


std::string y22day12(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}