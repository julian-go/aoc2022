#include "FileSystem.h"
#include "y22.h"

namespace {
using namespace std;

inline string part1(ifstream& in)
{
  FileSystem f;
  f.parse(in);
  const int32_t sum = f.sumSmallestFiles();
  return std::to_string(sum);
}

inline string part2(ifstream& in)
{
  FileSystem f;
  f.parse(in);
  const int32_t smallest = f.findSmallestForDeletion();
  return std::to_string(smallest);
}

}  // namespace

string y22day07(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}