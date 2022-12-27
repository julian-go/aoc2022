#include <array>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "y22.h"

namespace {

using namespace std;

struct Vector3D {
  constexpr Vector3D(int16_t x = 0, int16_t y = 0, int16_t z = 0)
      : x(x), y(y), z(z)
  {
  }
  bool operator<(const Vector3D& r) const
  {
    return x < r.x || (x == r.x && y < r.y) ||
           (x == r.x && y == r.y && z < r.z);
  }
  Vector3D operator+(const Vector3D& r) const
  {
    return Vector3D(x + r.x, y + r.y, z + r.z);
  }
  int16_t x;
  int16_t y;
  int16_t z;
};

static constexpr array<Vector3D, 6> kNeighbors = {
    Vector3D({1, 0, 0}),  Vector3D({-1, 0, 0}), Vector3D({0, 1, 0}),
    Vector3D({0, -1, 0}), Vector3D({0, 0, 1}),  Vector3D({0, 0, -1})};

struct Vector3DHash {
  size_t operator()(const Vector3D& v) const
  {
    return (static_cast<size_t>(v.x) << 32) | (static_cast<size_t>(v.y) << 16) |
           static_cast<size_t>(v.z);
  }
};

inline std::string part1(std::ifstream& in)
{
  Vector3D v;
  int32_t exposed_sides = 0;
  char sep;
  set<Vector3D> droplets;
  while (in >> v.x >> sep >> v.y >> sep >> v.z) {
    droplets.insert(v);
    exposed_sides += 6;
    for (const auto& n : kNeighbors) {
      if (droplets.contains(v + n)) {
        exposed_sides -= 2;
      }
    }
  }
  return std::to_string(exposed_sides);
}

inline std::string part2(std::ifstream& in)
{
  Vector3D v;
  Vector3D volume_size;
  int32_t exposed_sides = 0;
  char sep;
  set<Vector3D> droplets;
  vector<vector<vector<bool>>> volume;
  {
    while (in >> v.x >> sep >> v.y >> sep >> v.z) {
      volume_size.x = volume_size.x > v.x ? volume_size.x : v.x;
      volume_size.y = volume_size.y > v.y ? volume_size.y : v.y;
      volume_size.z = volume_size.z > v.z ? volume_size.z : v.z;

      // first insert new droplet
      droplets.insert(v);
      exposed_sides += 6;
      for (const auto& n : kNeighbors) {
        if (droplets.contains(v + n)) {
          exposed_sides -= 2;
        }
      }
    }

    vector<bool> tmp_z(volume_size.z + 2, false);
    vector<vector<bool>> tmp_y(volume_size.y + 2, tmp_z);
    volume.resize(volume_size.x + 2, tmp_y);
  }

  stack<Vector3D> to_check;
  to_check.push(Vector3D(0, 0, 0));
  auto visited = volume;
  exposed_sides = 0;
  set<Vector3D> checked;
  while (!to_check.empty()) {
    Vector3D v = to_check.top();
    to_check.pop();
    for (const auto& n : kNeighbors) {
      const auto vn = v + n;
      if (droplets.contains(vn)) {
        exposed_sides += 1;
      } else {
        if (vn.x >= 0 && vn.x < visited.size() && vn.y >= 0 &&
            vn.y < visited.front().size() && vn.z >= 0 &&
            vn.z < visited.front().front().size()) {
          if (!visited[vn.x][vn.y][vn.z]) {
            to_check.push(vn);
            visited[vn.x][vn.y][vn.z] = true;
          }
        }
      }
    }
    checked.insert(v);
  }

  return std::to_string(exposed_sides);
}

}  // namespace

string y22day18(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}