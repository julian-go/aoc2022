#include <array>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Vector2D.h"
#include "solutions.h"

namespace {

using Vec2D = Vector2D<int32_t>;

struct Cave {
  static constexpr int32_t kMaxHeight = 500;
  static constexpr int32_t kMaxWidth = 2 * kMaxHeight - 1;
  static constexpr Vec2D spawn = Vec2D(kMaxHeight, 0);
  std::array<std::array<bool, kMaxHeight>, kMaxWidth> map;
  int32_t grains_of_sand = 0;

  std::function<bool(const Vec2D&)> outOfScope;
  std::function<bool(const Vec2D&)> onFloor;

  // Only used for printing
  Vec2D min = spawn;
  Vec2D max = spawn;
};

std::unique_ptr<Cave> createCave(std::istream& in)
{
  std::unique_ptr<Cave> c = std::make_unique<Cave>();
  char sep;
  std::string line;

  for (auto& column : c->map) {
    column.fill(false);
  }

  while (getline(in, line)) {
    std::vector<Vec2D> segment;
    std::stringstream ss(line);

    while (ss) {
      segment.emplace_back();
      ss >> segment.back().x >> sep >> segment.back().y;
      ss.ignore(4);
      c->min.x = std::min(c->min.x, segment.back().x);
      c->min.y = std::min(c->min.y, segment.back().y);
      c->max.x = std::max(c->max.x, segment.back().x);
      c->max.y = std::max(c->max.y, segment.back().y);
    }

    for (auto it = segment.begin(); it != segment.end(); ++it) {
      c->map[(*it).x][(*it).y] = true;
      if (it + 1 != segment.end()) {
        Vec2D direction = *(it + 1) - *it;
        Vec2D unit_dir;
        int32_t steps = 0;
        if (direction.x > 0) {
          unit_dir = Vec2D(1, 0);
          steps = direction.x;
        } else if (direction.x < 0) {
          unit_dir = Vec2D(-1, 0);
          steps = -direction.x;
        } else if (direction.y > 0) {
          unit_dir = Vec2D(0, 1);
          steps = direction.y;
        } else if (direction.y < 0) {
          unit_dir = Vec2D(0, -1);
          steps = -direction.y;
        }

        for (int32_t i = 1; i <= steps; ++i) {
          Vec2D current = (*it) + unit_dir * i;
          c->map[current.x][current.y] = true;
        }
      }
    }
  }
  return c;
}

void flood(Cave& c)
{
  constexpr std::array directions = {Vec2D(0, 1), Vec2D(-1, 1),
                                     Vec2D(1, 1)};
  bool flooding = true;
  while (flooding) {
    Vec2D grain = c.spawn;
    bool at_rest = false;

    while (!at_rest && flooding) {
      at_rest = true;
      for (const auto& dir : directions) {
        const Vec2D moved_grain = grain + dir;
        const bool dir_empty = !c.map[moved_grain.x][moved_grain.y];
        const bool out_of_scope = c.outOfScope(moved_grain);
        const bool on_floor = c.onFloor(moved_grain);

        if (out_of_scope) {
          flooding = false;
          at_rest = false;
          break;
        }

        if (dir_empty && !on_floor) {
          grain = grain + dir;
          at_rest = false;
          break;
        }
      }

      if (at_rest) {
        c.map[grain.x][grain.y] = true;
        c.grains_of_sand++;
        if (grain == c.spawn) {
          flooding = false;
          break;
        }
      }
    }
  }
}

void printOccupancy(const Cave& c)
{
  for (int32_t y = c.min.y; y <= c.max.y; ++y) {
    for (int32_t x = c.min.x; x <= c.max.x; ++x) {
      if (c.map[x][y]) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

inline Solution part1(std::ifstream& in)
{
  std::unique_ptr<Cave> cave = createCave(in);
  cave->onFloor = [&cave](const Vec2D& v) { return false; };
  cave->outOfScope = [&cave](const Vec2D& v) {
    return v.x > cave->max.x || v.y > cave->max.y;
  };
  flood(*cave);

  return std::to_string(cave->grains_of_sand);
}

inline Solution part2(std::ifstream& in)
{
  std::unique_ptr<Cave> cave = createCave(in);
  cave->onFloor = [&cave](const Vec2D& v) { return v.y > cave->max.y + 1; };
  cave->outOfScope = [&cave](const Vec2D& v) { return false; };
  flood(*cave);

  return std::to_string(cave->grains_of_sand);
}

}  // namespace

Solution day14(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}