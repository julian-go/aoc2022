
#include <array>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "solutions.h"

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

inline Solution part1(std::ifstream& in)
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

// bool checkPocket(set<Vector3D>& droplets, int32_t& exposed_sides,
//                  const Vector3D& v, int16_t depth)
//{
//   constexpr int16_t kMaxDepth = 60;
//   if (depth > kMaxDepth) {
//     return false;
//   }
//
//   bool is_pocket = true;
//   droplets.insert(v);
//   exposed_sides += 6;
//   for (const auto& n : kNeighbors) {
//     if (!droplets.contains(v + n)) {
//       droplets.insert(v + n);
//       is_pocket &= checkPocket(droplets, exposed_sides, v + n, depth + 1);
//     } else {
//       exposed_sides -= 2;
//     }
//   }
//
//   return is_pocket;
// }

// bool fillVolume(vector<vector<vector<bool>>>& volume,
//                 const Vector3D& volume_size, int32_t& exposed_sides,
//                 Vector3D v)
//{
//   bool is_pocket = true;
//   if (v.x >= volume_size.x || v.x < 0 || v.y >= volume_size.y || v.y < 0 ||
//       v.z >= volume_size.z || v.z < 0) {
//     return false;
//   }
//
//   //volume[1][1][1] = true;
//   //cout << v.x << endl;
//   //volume[v.x][1][1] = true;
//   //volume[v.x][v.y][1] = true;
//   volume[v.x][v.y][v.z] = true;
//   cout << "filled at " << v.x << " " << v.y << " " << v.z << endl;
//   exposed_sides += 6;
//   for (const auto& n : kNeighbors) {
//     const auto vn = v + n;
//     if (vn.x >= volume_size.x || vn.x < 0 || vn.y >= volume_size.y ||
//         vn.y < 0 || vn.z >= volume_size.z || vn.z < 0) {
//       is_pocket = false;
//       continue;
//     }
//     if (!volume[vn.x][vn.y][vn.z]) {
//       is_pocket &= fillVolume(volume, volume_size, exposed_sides, vn);
//     } else {
//       exposed_sides -= 2;
//     }
//   }
//
//   return is_pocket;
// }

// inline Solution part2(std::ifstream& in)
//{
//   Vector3D v;
//   Vector3D volume_size;
//   int32_t exposed_sides = 0;
//   char sep;
//   set<Vector3D> droplets;
//   while (in >> v.x >> sep >> v.y >> sep >> v.z) {
//     if (droplets.contains(v)) {
//       continue;
//     }
//
//     // first insert new droplet
//     droplets.insert(v);
//     exposed_sides += 6;
//     for (const auto& n : kNeighbors) {
//       if (droplets.contains(v + n)) {
//         exposed_sides -= 2;
//       }
//     }
//
//     // Then check all neighbors for an airpocket
//
//     for (const auto& n : kNeighbors) {
//       if (!droplets.contains(v + n)) {
//         set<Vector3D> tmp_droplets = droplets;
//         int32_t tmp_sides = exposed_sides;
//         if (checkPocket(tmp_droplets, tmp_sides, v + n, 0)) {
//           droplets = tmp_droplets;
//           exposed_sides = tmp_sides;
//         }
//       }
//     }
//   }
//   return std::to_string(exposed_sides);
// }

inline Solution part2(std::ifstream& in)
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
  // for (const auto& d : droplets) {
  //   volume[d.x][d.y][d.z] = true;
  // }

  // for (int x = 0; x < volume_size.x; ++x) {
  //   for (int y = 0; y < volume_size.y; ++y) {
  //     for (int z = 0; z < volume_size.z; ++z) {
  //       if (!volume[x][y][z]) {
  //         auto filled_volume = volume;
  //         auto filled_exposed_sides = exposed_sides;
  //         if (fillVolume(filled_volume, volume_size, filled_exposed_sides,
  //                        Vector3D(x, y, z))) {
  //           cout << "POCKET " << x << " " << y << " " << z << " " << endl;
  //           cout << "Sides " << filled_exposed_sides << " " << endl;
  //           volume = filled_volume;
  //           exposed_sides = filled_exposed_sides;
  //         }
  //       }
  //     }
  //   }
  // }

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

Solution day18(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}