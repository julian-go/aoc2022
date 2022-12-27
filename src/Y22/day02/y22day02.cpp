#include "y22.h"

namespace {
using namespace std;

inline string part1(ifstream& in)
{
  std::int32_t score_total(0);
  char enemy, player;

  while (in >> enemy && in >> player) {
    player -= 'X';
    enemy -= 'A';
    bool win = (player - enemy + 3) % 3 == 1;
    bool draw = (player == enemy);
    score_total += player + 1;
    score_total += win ? 6 : (draw ? 3 : 0);
  }

  return std::to_string(score_total);
}

inline string part2(ifstream& in)
{
  std::int32_t score_total(0);
  char enemy, player;

  while (in >> enemy && in >> player) {
    enemy -= 'A';
    bool win = player == 'Z';
    bool draw = player == 'Y';
    score_total += (enemy + (win ? 1 : (draw ? 0 : -1)) + 3) % 3 + 1;
    score_total += win ? 6 : (draw ? 3 : 0);
  }

  return std::to_string(score_total);
}

}  // namespace

string y22day02(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}