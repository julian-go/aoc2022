#include <iostream>
#include <vector>

#include "y19.h"

namespace {
using namespace std;

inline string part1(ifstream& in)
{
  std::vector<int32_t> program;
  int32_t value;
  char sep;
  while (in >> value) {
    program.push_back(value);
    in >> sep;
  }

  if (program.size() > 20) {
    program[1] = 12;
    program[2] = 2;
  }

  int32_t i = 0;
  while (i < program.size() && program[i] != 99) {
    if (program[i] == 1) {
      program[program[i + 3]] =
          program[program[i + 1]] + program[program[i + 2]];
    } else if (program[i] == 2) {
      program[program[i + 3]] =
          program[program[i + 1]] * program[program[i + 2]];
    } else {
      cout << "error" << endl;
    }
    i += 4;
  }

  return to_string(program[0]);
}   

inline string part2(ifstream& in)
{
  constexpr int32_t kGoal = 19'690'720;
  std::vector<int32_t> program;
  int32_t value;
  char sep;
  while (in >> value) {
    program.push_back(value);
    in >> sep;
  }

  if (program.size() < 20) {
    return "N/A";
  }

  int32_t noun, verb;
  std::vector<int32_t> p;
  for ( noun = 0; noun < 100; ++noun) {
    for (verb = 0; verb < 100; ++verb) {
      p = program;
      p[1] = noun;
      p[2] = verb;

      int32_t i = 0;
      while (i < p.size() && p[i] != 99) {
        if (p[i] == 1) {
          p[p[i + 3]] = p[p[i + 1]] + p[p[i + 2]];
        } else if (p[i] == 2) {
          p[p[i + 3]] = p[p[i + 1]] * p[p[i + 2]];
        } else {
          break;
        }
        i += 4;
      }

      if (p[0] == kGoal) {
        return to_string(100 * noun + verb);
      }
    }
  }
  
  return "error";
}

}  // namespace

string y19day02(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}