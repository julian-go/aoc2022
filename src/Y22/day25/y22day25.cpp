#include <iostream>
#include <string>
#include <unordered_map>

#include "y22.h"

namespace {

using namespace std;

int32_t toBase10(string snafu)
{
  const unordered_map<char, int32_t> kMap = {
      {'=', -2}, {'-', -1}, {'0', 0}, {'1', 1}, {'2', 2}};
  int32_t result = 0;
  int32_t exponent = 0;
  for (auto it = snafu.rbegin(); it != snafu.rend(); ++it) {
    result += kMap.at(*it) * pow(5, exponent);
    exponent++;
  }
  return result;
}

string add(string l_snafu, string r_snafu)
{
  const unordered_map<char, int32_t> kMap = {
      {'=', -2}, {'-', -1}, {'0', 0}, {'1', 1}, {'2', 2}};
  const unordered_map<int32_t, char> lRMap = {
      {-2, '='}, {-1, '-'}, {0, '0'}, {1, '1'}, {2, ' 2'}};

  string result = "";
  int32_t l_idx = l_snafu.size() - 1;
  int32_t r_idx = r_snafu.size() - 1;
  int32_t carried = 0;

  while (l_idx >= 0 || r_idx >= 0) {
    char l_char = l_idx >= 0 ? l_snafu[l_idx] : '0';
    char r_char = r_idx >= 0 ? r_snafu[r_idx] : '0';

    int32_t tmp = kMap.at(l_char) + kMap.at(r_char) + carried;
    if (tmp < -2) {
      carried = -1;
      tmp += 5;
    } else if (tmp > 2) {
      carried = 1;
      tmp -= 5;
    } else {
      carried = 0;
    }

    result.push_back(lRMap.at(tmp));

    --l_idx;
    --r_idx;
  }

  if (carried != 0) {
    result.push_back(lRMap.at(carried));
  }

  reverse(result.begin(), result.end());

  return result;
}

inline string part1(std::ifstream& in)
{
  string line;
  string result = "0";
  while (getline(in, line)) {
    result = add(result, line);
  }
  return result;
}

inline string part2(std::ifstream& in) { return "N/A"; }

}  // namespace
string y22day25(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}