
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Tools.h"
#include "Vector2D.h"
#include "VectorMatrix2D.h"
#include "solutions.h"

namespace {

using namespace std;

enum class Direction { kNorth, kSouth, kEast, kWest };

class Storm {
  struct Blizzard {
    Blizzard(Vector2D p, Vector2D dir)
    {
      position = p;
      direction = dir;
    }
    Vector2D position;
    Vector2D direction;
  };

 public:
  void solve()
  {
    const array kMoves = {Vector2D(0, 0), Vector2D(1, 0), Vector2D(-1, 0),
                          Vector2D(0, -1), Vector2D(0, 1)};
    unordered_set<Vector2D, Vector2DHash> possible_positions;
    possible_positions.insert(start_position_offset_);
    int32_t time = 0;
    while (true) {
      time++;
      moveBlizzards();
      unordered_set<Vector2D, Vector2DHash> new_positions;
      for (const auto& p : possible_positions) {
        for (const auto& move : kMoves) {
          const Vector2D next_position = p + move;
          if (next_position == goal_position_) {
            cout << "done at: " << time << endl;
          }
          if (next_position.y >= 0 && next_position.y < map_.sizeY() &&
              !map_.at(next_position.x, next_position.y)) {
            new_positions.insert(next_position);
          }
        }
      }
      possible_positions = new_positions;
    }
  }

  void solve2()
  {
    const array kMoves = {Vector2D(0, 0), Vector2D(1, 0), Vector2D(-1, 0),
                          Vector2D(0, -1), Vector2D(0, 1)};
    unordered_set<Vector2D, Vector2DHash> possible_positions;
    possible_positions.insert(start_position_offset_);
    int32_t time = 0;
    bool done = false;
    while (!done) {
      time++;
      moveBlizzards();
      unordered_set<Vector2D, Vector2DHash> new_positions;
      for (const auto& p : possible_positions) {
        for (const auto& move : kMoves) {
          const Vector2D next_position = p + move;
          if (next_position == goal_position_) {
            done = true;
            cout << "time: " << time << endl;
            break;
          }
          if (next_position.y >= 0 && next_position.y < map_.sizeY() &&
              !map_.at(next_position.x, next_position.y)) {
            new_positions.insert(next_position);
          }
        }
        if (done) {
          break;
        }
      }
      possible_positions = new_positions;
    }

    possible_positions.clear();
    possible_positions.insert(goal_position_);
    done = false;

    while (!done) {
      time++;
      moveBlizzards();
      unordered_set<Vector2D, Vector2DHash> new_positions;
      for (const auto& p : possible_positions) {
        for (const auto& move : kMoves) {
          const Vector2D next_position = p + move;
          if (next_position == start_position_offset_) {
            done = true;
            cout << "time: " << time << endl;
            break;
          }
          if (next_position.y >= 0 && next_position.y < map_.sizeY() &&
              !map_.at(next_position.x, next_position.y)) {
            new_positions.insert(next_position);
          }
        }
        if (done) {
          break;
        }
      }
      possible_positions = new_positions;
    }

    possible_positions.clear();
    possible_positions.insert(start_position_offset_);
    done = false;

    while (!done) {
      time++;
      moveBlizzards();
      unordered_set<Vector2D, Vector2DHash> new_positions;
      for (const auto& p : possible_positions) {
        for (const auto& move : kMoves) {
          const Vector2D next_position = p + move;
          if (next_position == goal_position_) {
            done = true;
            cout << "time: " << time << endl;
            break;
          }
          if (next_position.y >= 0 && next_position.y < map_.sizeY() &&
              !map_.at(next_position.x, next_position.y)) {
            new_positions.insert(next_position);
          }
        }
        if (done) {
          break;
        }
      }
      possible_positions = new_positions;
    }
  }

  void parse(ifstream& in)
  {
    const unordered_map<char, Vector2D> kDirs = {{'>', Vector2D(1, 0)},
                                                 {'<', Vector2D(-1, 0)},
                                                 {'^', Vector2D(0, -1)},
                                                 {'v', Vector2D(0, 1)}};

    // Get size
    string line;
    int32_t size_x = 0, size_y = 0;
    while (getline(in, line)) {
      size_x = line.size();
      size_y++;
    }
    goal_position_ = Vector2D(size_x - 1, size_y - 1) + goal_position_offset_;
    in.clear();
    in.seekg(0);

    // Get blizzards
    int32_t y = 0;
    while (getline(in, line)) {
      for (int32_t x = 0; x < line.size(); ++x) {
        if (kDirs.contains(line[x])) {
          blizzards_.emplace_back(Vector2D(x, y), kDirs.at(line[x]));
        }
      }
      ++y;
    }

    // Create map
    map_.resize(size_x, size_y);
    map_.fill(false);
    for (int32_t x = 0; x < size_x; ++x) {
      if (x != 1) {
        // Entrance
        map_.set(x, 0, true);
      }
      if (x != size_x - 2) {
        // Exit
        map_.set(x, size_y - 1, true);
      }
    }
    for (int32_t y = 0; y < size_y; ++y) {
      map_.set(0, y, true);
      map_.set(size_x - 1, y, true);
    }
  }

  void clearBlizzards()
  {
    for (int32_t y = 1; y < map_.sizeY() - 1; ++y) {
      for (int32_t x = 1; x < map_.sizeX() - 1; ++x) {
        map_.set(x, y, false);
      }
    }
  }

  void moveBlizzards()
  {
    clearBlizzards();
    for (auto& blizzard : blizzards_) {
      blizzard.position += blizzard.direction;
      if (blizzard.position.x < 1) {
        blizzard.position.x = map_.sizeX() - 2;
      } else if (blizzard.position.x >= map_.sizeX() - 1) {
        blizzard.position.x = 1;
      } else if (blizzard.position.y < 1) {
        blizzard.position.y = map_.sizeY() - 2;
      } else if (blizzard.position.y >= map_.sizeY() - 1) {
        blizzard.position.y = 1;
      }
      map_.set(blizzard.position.x, blizzard.position.y, true);
    }
  }

  void print()
  {
    for (int32_t y = 0; y < map_.sizeY(); ++y) {
      for (int32_t x = 0; x < map_.sizeX(); ++x) {
        if (Vector2D(x, y) == start_position_offset_) {
          cout << 'S';
        } else if (Vector2D(x, y) == goal_position_) {
          cout << 'E';
        } else {
          cout << (map_.at(x, y) ? '#' : '.');
        }
      }
      cout << endl;
    }
    cout << endl;
  }

 private:
  static constexpr Vector2D start_position_offset_ = Vector2D(1, 0);
  static constexpr Vector2D goal_position_offset_ = Vector2D(-1, 0);
  Vector2D goal_position_;

  vector<Blizzard> blizzards_;
  VectorMatrix2D<bool> map_;
};

inline Solution part1(std::ifstream& in)
{
  Storm s;
  s.parse(in);
  s.print();
  s.solve2();
  // s.moveBlizzards();
  // s.print();
  // s.moveBlizzards();
  // s.print();
  // s.moveBlizzards();
  // s.print();
  // s.moveBlizzards();
  return "";
}

inline Solution part2(std::ifstream& in) { return std::to_string(0); }

}  // namespace

Solution day24(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}