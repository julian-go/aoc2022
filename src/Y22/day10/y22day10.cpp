#include "Device.h"
#include "y22.h"

namespace {
using namespace std;

inline string part1(ifstream& in)
{
  Device d;
  d.processCommandStream(in);
  return std::to_string(d.getTotalSignalStrength());
}

inline string part2(ifstream& in)
{
  Device d;
  d.processCommandStream(in);
  return d.render(false);
}

}  // namespace

string y22day10(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}