#include <iostream>

#include "y19.h"

namespace {
using namespace std;

int32_t computeFuelNeed(int32_t mass)
{
  if (mass > 0) {
    int32_t fuel = mass / 3 - 2;
    fuel = max(fuel, 0);
    fuel += computeFuelNeed(fuel);
  } else {
    return 0;
  }
}

inline string part1(ifstream& in)
{
  int32_t total = 0;
  int32_t current;
  while (in >> current) {
    total += current / 3 - 2;
  }
  return to_string(total);
}

inline string part2(ifstream& in)
{
  int32_t total = 0;
  int32_t current;
  while (in >> current) {
    total += computeFuelNeed(current);
  }
  return to_string(total);
}

}  // namespace

string y19day01(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}