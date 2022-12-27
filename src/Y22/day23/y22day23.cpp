#include <array>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/Matrix2D.h"
#include "utils/Vector2D.h"
#include "y22.h"

namespace {

using namespace std;
using Vec2D = Vector2D<int32_t>;

template <typename TType>
using Matrix = Matrix2D<TType>;

void parse(ifstream& in, Matrix2D<uint8_t>& elves, vector<Vec2D>& elf_positions,
           int32_t num_rounds)
{
  string line;
  int32_t size_x = 0;
  int32_t size_y = 0;
  while (getline(in, line)) {
    size_x = line.size();
    size_y++;
  }

  elves.resize(size_x + 2 * num_rounds, size_y + 2 * num_rounds);
  elves.fill(false);

  in.clear();
  in.seekg(0);

  size_t y = 0;
  while (getline(in, line)) {
    for (size_t x = 0; x < line.size(); ++x) {
      if (line[x] == '#') {
        elves.at(x + num_rounds, y + num_rounds) = true;
        elf_positions.emplace_back(x + num_rounds, y + num_rounds);
      }
    }
    y++;
  }
}

void print(const Matrix2D<uint8_t>& elves)
{
  cout << endl;
  for (int32_t y = 0; y < elves.sizeY(); ++y) {
    for (int32_t x = 0; x < elves.sizeX(); ++x) {
      if (elves.at(x, y)) {
        cout << '#';
      } else {
        cout << '.';
      }
    }
    cout << endl;
  }
  cout << endl;
}

inline string part1(std::ifstream& in)
{
  constexpr int32_t kRounds = 10;
  constexpr array kDirs = {Vec2D(0, -1), Vec2D(0, 1), Vec2D(-1, 0),
                           Vec2D(1, 0)};
  constexpr array<array<Vec2D, 3>, 4> kCheckDirs = {
      array{Vec2D(0, -1), Vec2D(1, -1), Vec2D(-1, -1)},
      array{Vec2D(0, 1), Vec2D(1, 1), Vec2D(-1, 1)},
      array{Vec2D(-1, 0), Vec2D(-1, 1), Vec2D(-1, -1)},
      array{Vec2D(1, 0), Vec2D(1, 1), Vec2D(1, -1)}};

  int32_t first_direction = 0;
  Matrix2D<uint8_t> elves;
  vector<Vec2D> elf_positions;
  unordered_map<Vec2D, vector<int32_t>, Vec2D::Hash> next_move;
  parse(in, elves, elf_positions, kRounds);
  // print(elves);

  for (int32_t i = 0; i < kRounds; ++i) {
    next_move.clear();

    // First half
    for (int32_t elf_i = 0; elf_i < elf_positions.size(); ++elf_i) {
      const Vec2D current_position = elf_positions[elf_i];
      bool all_clear = true;
      for (int32_t dir = 0; dir < 4; ++dir) {
        for (int32_t check_dir = 0; check_dir < 3; ++check_dir) {
          const Vec2D to_check = current_position + kCheckDirs[dir][check_dir];
          if (elves.at(to_check.x, to_check.y)) {
            all_clear = false;
          }
        }
      }

      if (all_clear) {
        continue;  // do nothing
      }

      for (int32_t dir = 0; dir < 4; ++dir) {
        bool clear = true;
        const Vec2D next_move_vector = kDirs[(dir + first_direction) % 4];
        for (int32_t check_dir = 0; check_dir < 3; ++check_dir) {
          const Vec2D to_check =
              current_position +
              kCheckDirs[(dir + first_direction) % 4][check_dir];
          if (elves.at(to_check.x, to_check.y)) {
            clear = false;
            break;
          }
        }
        if (clear) {
          if (next_move.contains(current_position + next_move_vector)) {
            next_move[current_position + next_move_vector].push_back(elf_i);
          } else {
            next_move[current_position + next_move_vector] = vector{elf_i};
          }
          break;
        }
      }
    }

    // Second half
    for (const auto& move : next_move) {
      if (move.second.size() == 1) {
        Vec2D& current_position = elf_positions[move.second.front()];
        elves.at(current_position.x, current_position.y) = false;
        current_position = move.first;
        elves.at(current_position.x, current_position.y) = true;
      }
    }

    first_direction++;
    first_direction %= 4;

    // print(elves);
  }

  int32_t min_x = numeric_limits<int32_t>::max(),
          max_x = numeric_limits<int32_t>::min(),
          min_y = numeric_limits<int32_t>::max(),
          max_y = numeric_limits<int32_t>::min();
  for (const auto& v : elf_positions) {
    min_x = min(min_x, v.x);
    max_x = max(max_x, v.x);
    min_y = min(min_y, v.y);
    max_y = max(max_y, v.y);
  }

  int32_t ground_tiles =
      (max_x + 1 - min_x) * (max_y + 1 - min_y) - elf_positions.size();

  // print(elves);

  return std::to_string(ground_tiles);
}

inline string part2(std::ifstream& in)
{
  constexpr array kDirs = {Vec2D(0, -1), Vec2D(0, 1), Vec2D(-1, 0),
                           Vec2D(1, 0)};
  constexpr array<array<Vec2D, 3>, 4> kCheckDirs = {
      array{Vec2D(0, -1), Vec2D(1, -1), Vec2D(-1, -1)},
      array{Vec2D(0, 1), Vec2D(1, 1), Vec2D(-1, 1)},
      array{Vec2D(-1, 0), Vec2D(-1, 1), Vec2D(-1, -1)},
      array{Vec2D(1, 0), Vec2D(1, 1), Vec2D(1, -1)}};

  int32_t first_direction = 0;
  Matrix2D<uint8_t> elves;
  vector<Vec2D> elf_positions;
  unordered_map<Vec2D, vector<int32_t>, Vec2D::Hash> next_move;
  parse(in, elves, elf_positions, 150);
  // print(elves);

  bool moved = true;
  int32_t rounds = 0;
  while (moved) {
    rounds++;
    moved = false;
    next_move.clear();

    // First half
    for (int32_t elf_i = 0; elf_i < elf_positions.size(); ++elf_i) {
      const Vec2D current_position = elf_positions[elf_i];
      bool all_clear = true;
      for (int32_t dir = 0; dir < 4; ++dir) {
        for (int32_t check_dir = 0; check_dir < 3; ++check_dir) {
          const Vec2D to_check = current_position + kCheckDirs[dir][check_dir];
          if (elves.at(to_check.x, to_check.y)) {
            all_clear = false;
          }
        }
      }

      if (all_clear) {
        continue;  // do nothing
      } else {
        moved = true;
      }

      for (int32_t dir = 0; dir < 4; ++dir) {
        bool clear = true;
        const Vec2D next_move_vector = kDirs[(dir + first_direction) % 4];
        for (int32_t check_dir = 0; check_dir < 3; ++check_dir) {
          const Vec2D to_check =
              current_position +
              kCheckDirs[(dir + first_direction) % 4][check_dir];
          if (elves.at(to_check.x, to_check.y)) {
            clear = false;
            break;
          }
        }
        if (clear) {
          if (next_move.contains(current_position + next_move_vector)) {
            next_move[current_position + next_move_vector].push_back(elf_i);
          } else {
            next_move[current_position + next_move_vector] = vector{elf_i};
          }
          break;
        }
      }
    }

    // Second half
    for (const auto& move : next_move) {
      if (move.second.size() == 1) {
        Vec2D& current_position = elf_positions[move.second.front()];
        elves.at(current_position.x, current_position.y) = false;
        current_position = move.first;
        elves.at(current_position.x, current_position.y) = true;
      }
    }

    first_direction++;
    first_direction %= 4;

    if (moved) {
    }

    // print(elves);
  }

  int32_t min_x = numeric_limits<int32_t>::max(),
          max_x = numeric_limits<int32_t>::min(),
          min_y = numeric_limits<int32_t>::max(),
          max_y = numeric_limits<int32_t>::min();
  for (const auto& v : elf_positions) {
    min_x = min(min_x, v.x);
    max_x = max(max_x, v.x);
    min_y = min(min_y, v.y);
    max_y = max(max_y, v.y);
  }

  int32_t ground_tiles =
      (max_x + 1 - min_x) * (max_y + 1 - min_y) - elf_positions.size();

  // print(elves);

  return std::to_string(rounds);
}

}  // namespace

string y22day23(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}