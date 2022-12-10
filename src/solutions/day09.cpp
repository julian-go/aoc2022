#include "solutions.h"

namespace {

inline Solution part1(std::ifstream& in)
{
  Solution s = std::to_string(0);
  return s;
}

inline Solution part2(std::ifstream& in)
{
  
  Solution s = std::to_string(0);
  return s;
}

}  // namespace

Solution day09(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}