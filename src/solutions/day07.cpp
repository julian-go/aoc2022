#include "solutions.h"

#include "Device.h"

namespace {

inline Solution part1(std::ifstream& in)
{
  Device d;
  d.file_system.parse(in);
  const int32_t sum = d.file_system.sumSmallestFiles();
  return std::to_string(sum);
}

inline Solution part2(std::ifstream& in)
{
  
  Device d;
  d.file_system.parse(in);
  const int32_t smallest = d.file_system.findSmallestForDeletion();
  return std::to_string(smallest);
}

}  // namespace

Solution day07(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}