#include "y22.h"

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/Vector2D.h"

namespace {

using namespace std;
using Vec2D = Vector2D<int32_t>;

struct Move {
  enum Direction { East = 0x1, North = 0x2, West = 0x4, South = 0x8 };
  Direction dir;
  int32_t steps;
};

class Map2D {
 public:
  Map2D() = default;

  void setOpen(const int32_t x, const int32_t y, const bool open)
  {
    open_[index(x, y)] = open;
  }

  bool open(const int32_t x, const int32_t y) const
  {
    return open_[index(x, y)];
  }

  void setBlocked(const int32_t x, const int32_t y, const bool blocked)
  {
    blocked_[index(x, y)] = blocked;
  }

  bool blocked(const int32_t x, const int32_t y) const
  {
    return blocked_[index(x, y)];
  }

  int32_t index(const int32_t x, const int32_t y) const
  {
    return y * max_x + x;
  }

  int32_t getStartX() const
  {
    for (int32_t x = 0; x < max_x; ++x) {
      if (open(x, 0)) {
        return x;
      }
    }
  }

  Vec2D transform(Vec2D pos, Move m)
  {
    const unordered_map<Move::Direction, Vec2D> kDir = {
        {Move::Direction::East, Vec2D(1, 0)},
        {Move::Direction::North, Vec2D(0, -1)},
        {Move::Direction::West, Vec2D(-1, 0)},
        {Move::Direction::South, Vec2D(0, 1)}};
    const unordered_map<Move::Direction, function<Vec2D(Vec2D)>> kWrap = {
        {Move::Direction::East, [&](auto v) { return wrapEastMove(v); }},
        {Move::Direction::North, [&](auto v) { return wrapNorthMove(v); }},
        {Move::Direction::West, [&](auto v) { return wrapWestMove(v); }},
        {Move::Direction::South, [&](auto v) { return wrapSouthMove(v); }}};

    Vec2D dir = kDir.at(m.dir);
    auto wrap = kWrap.at(m.dir);
    for (int32_t i = 0; i < m.steps; ++i) {
      Vec2D new_pos = pos + dir;
      new_pos = wrap(new_pos);
      if (blocked(new_pos.x, new_pos.y)) {
        break;
      }
      pos = new_pos;
    }

    return pos;
  }  // namespace

  Vec2D wrapEastMove(Vec2D v)
  {
    if (v.x >= max_x || !open(v.x, v.y)) {
      for (int32_t x = v.x - 1; x >= 0; --x) {
        if (!open(x, v.y)) {
          return Vec2D(x + 1, v.y);
        }
        if (x == 0) {
          return Vec2D(x, v.y);
        }
      }
    }
    return v;
  }

  Vec2D wrapWestMove(Vec2D v)
  {
    if (v.x < 0 || !open(v.x, v.y)) {
      for (int32_t x = v.x + 1; x < max_x; ++x) {
        if (!open(x, v.y)) {
          return Vec2D(x - 1, v.y);
        }
        if (x == max_x - 1) {
          return Vec2D(x, v.y);
        }
      }
    }
    return v;
  }

  Vec2D wrapSouthMove(Vec2D v)
  {
    if (v.y >= max_y || !open(v.x, v.y)) {
      for (int32_t y = v.y - 1; y >= 0; --y) {
        if (!open(v.x, y)) {
          return Vec2D(v.x, y + 1);
        }
        if (y == 0) {
          return Vec2D(v.x, y);
        }
      }
    }
    return v;
  }

  Vec2D wrapNorthMove(Vec2D v)
  {
    if (v.y < 0 || !open(v.x, v.y)) {
      for (int32_t y = v.y + 1; y < max_y; ++y) {
        if (!open(v.x, y)) {
          return Vec2D(v.x, y - 1);
        }
        if (y == max_y - 1) {
          return Vec2D(v.x, y);
        }
      }
    }
    return v;
  }

  void print(int32_t px, int32_t py) const
  {
    for (int32_t y = 0; y < max_y; ++y) {
      for (int32_t x = 0; x < max_x; ++x) {
        if (blocked(x, y)) {
          cout << '#';
        } else if (x == px && y == py) {
          cout << 'O';
        } else if (open(x, y)) {
          cout << '.';
        } else {
          cout << ' ';
        }
      }
      cout << endl;
    }
    cout << endl;
  }

  vector<bool> open_;
  vector<bool> blocked_;

  int32_t max_x;
  int32_t max_y;
};

void parse(std::ifstream& in, Map2D& map, vector<Move>& moves)
{
  string line;
  map.max_x = 0;
  map.max_y = 0;

  // First find the maximum dimensions
  while (getline(in, line)) {
    if (line.empty()) break;
    map.max_y++;
    map.max_x = max(map.max_x, static_cast<int32_t>(line.size()));
  }

  map.open_ = vector<bool>(map.max_x * map.max_y, false);
  map.blocked_ = vector<bool>(map.max_x * map.max_y, false);

  in.clear();
  in.seekg(0);

  // Parse Map
  int32_t y = 0;
  while (getline(in, line)) {
    if (line.empty()) break;
    for (int32_t x = 0; x < line.size(); ++x) {
      if (line[x] == '.') {
        map.setOpen(x, y, true);
      } else if (line[x] == '#') {
        map.setOpen(x, y, true);
        map.setBlocked(x, y, true);
      }
    }
    y++;
  }

  // Parse Inputs
  getline(in, line);
  stringstream ss(line);
  Move::Direction facing = Move::Direction::East;
  char turn;
  int32_t amount;
  while (!ss.eof()) {
    if (ss >> amount) {
      Move m;
      m.steps = amount;
      m.dir = facing;
      moves.push_back(m);
    }
    if (ss >> turn) {
      if (turn == 'L') {
        if (facing == Move::Direction::South) {
          facing = Move::Direction::East;
        } else {
          facing = static_cast<Move::Direction>(facing << 1);
        }
      } else if (turn == 'R') {
        if (facing == Move::Direction::East) {
          facing = Move::Direction::South;
        } else {
          facing = static_cast<Move::Direction>(facing >> 1);
        }
      }
    }
  }
}

inline string part1(std::ifstream& in)
{
  Map2D map;
  vector<Move> moves;
  parse(in, map, moves);

  Vec2D position;
  position.x = map.getStartX();
  position.y = 0;
  //map.print(position.x, position.y);

  for (Move& move : moves) {
    //cout << "Move " << move.steps << " to the ";
    //switch (move.dir) {
    //  case Move::Direction::East:
    //    cout << "East" << endl;
    //    break;
    //  case Move::Direction::North:
    //    cout << "North" << endl;
    //    break;
    //  case Move::Direction::West:
    //    cout << "West" << endl;
    //    break;
    //  case Move::Direction::South:
    //    cout << "South" << endl;
    //    break;
    //}

    position = map.transform(position, move);
    //cout << "Now at " << position << endl;
    // map.print(position.x, position.y);
  }

  const unordered_map<Move::Direction, int32_t> kValue = {
      {Move::Direction::East, 0},
      {Move::Direction::North, 3},
      {Move::Direction::West, 2},
      {Move::Direction::South, 1}};
  int32_t solution = 1000 * (position.y + 1) + 4 * (position.x + 1) +
                     kValue.at(moves.back().dir);

  return std::to_string(solution);
}

inline string part2(std::ifstream& in) { return std::to_string(0); }

}  // namespace

string y22day22(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}