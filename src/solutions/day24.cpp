
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Matrix2D.h"
#include "Tools.h"
#include "Vector2D.h"
#include "solutions.h"

namespace {

using namespace std;

struct Blizzard {
  Blizzard(Vector2D p, Vector2D dir)
  {
    position = p;
    direction = dir;
  }
  Vector2D position;
  Vector2D direction;
};

// forward declarations
void parse(ifstream& in, Matrix2D<bool>& map, vector<Blizzard>& blizzards);
int32_t solve(Matrix2D<bool>& map, vector<Blizzard>& blizzards,
              queue<Vector2D>& waypoints);
void moveElves(Matrix2D<bool>& map,
               unordered_set<Vector2D, Vector2DHash>& positions,
               unordered_set<Vector2D, Vector2DHash>& next_positions,
               queue<Vector2D>& waypoints);
void moveBlizzards(Matrix2D<bool>& map, vector<Blizzard>& blizzards);
void clearBlizzards(Matrix2D<bool>& map);
void print(Matrix2D<bool>& map);

void parse(ifstream& in, Matrix2D<bool>& map, vector<Blizzard>& blizzards)
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
  in.clear();
  in.seekg(0);

  // Get blizzards
  int32_t y = 0;
  while (getline(in, line)) {
    for (int32_t x = 0; x < line.size(); ++x) {
      if (kDirs.contains(line[x])) {
        blizzards.emplace_back(Vector2D(x, y), kDirs.at(line[x]));
      }
    }
    ++y;
  }

  // Create map
  map.resize(size_x, size_y);
  map.fill(false);
  for (int32_t x = 0; x < size_x; ++x) {
    if (x != 1) {
      // Entrance
      map.set(x, 0, true);
    }
    if (x != size_x - 2) {
      // Exit
      map.set(x, size_y - 1, true);
    }
  }
  for (int32_t y = 0; y < size_y; ++y) {
    map.set(0, y, true);
    map.set(size_x - 1, y, true);
  }
}

int32_t solve(Matrix2D<bool>& map, vector<Blizzard>& blizzards,
              queue<Vector2D>& waypoints)
{
  int32_t time = 0;
  unordered_set<Vector2D, Vector2DHash> positions;
  unordered_set<Vector2D, Vector2DHash> next_positions;

  positions.insert(waypoints.front());
  waypoints.pop();

  while (!waypoints.empty()) {
    time++;

    moveBlizzards(map, blizzards);
    moveElves(map, positions, next_positions, waypoints);
  }
  return time;
}

void moveElves(Matrix2D<bool>& map,
               unordered_set<Vector2D, Vector2DHash>& positions,
               unordered_set<Vector2D, Vector2DHash>& next_positions,
               queue<Vector2D>& waypoints)
{
  constexpr array kMoves = {Vector2D(0, 0), Vector2D(1, 0), Vector2D(-1, 0),
                            Vector2D(0, -1), Vector2D(0, 1)};
  for (const auto& p : positions) {
    for (const auto& move : kMoves) {
      const Vector2D next_position = p + move;
      if (next_position == waypoints.front()) {
        positions.clear();
        next_positions.clear();
        positions.insert(waypoints.front());
        waypoints.pop();
        return;
      } else if (next_position.y >= 0 && next_position.y < map.sizeY() &&
                 !map.at(next_position.x, next_position.y)) {
        next_positions.insert(next_position);
      }
    }
  }
  positions = next_positions;
  next_positions.clear();
}

void moveBlizzards(Matrix2D<bool>& map, vector<Blizzard>& blizzards)
{
  clearBlizzards(map);
  for (auto& blizzard : blizzards) {
    blizzard.position += blizzard.direction;
    if (blizzard.position.x < 1) {
      blizzard.position.x = map.sizeX() - 2;
    } else if (blizzard.position.x >= map.sizeX() - 1) {
      blizzard.position.x = 1;
    } else if (blizzard.position.y < 1) {
      blizzard.position.y = map.sizeY() - 2;
    } else if (blizzard.position.y >= map.sizeY() - 1) {
      blizzard.position.y = 1;
    }
    map.set(blizzard.position.x, blizzard.position.y, true);
  }
}

void clearBlizzards(Matrix2D<bool>& map)
{
  for (int32_t y = 1; y < map.sizeY() - 1; ++y) {
    for (int32_t x = 1; x < map.sizeX() - 1; ++x) {
      map.set(x, y, false);
    }
  }
}

void print(Matrix2D<bool>& map)
{
  for (int32_t y = 0; y < map.sizeY(); ++y) {
    for (int32_t x = 0; x < map.sizeX(); ++x) {
      cout << (map.at(x, y) ? '#' : '.');
    }
    cout << endl;
  }
  cout << endl;
}

inline Solution part1(std::ifstream& in)
{
  Matrix2D<bool> map;          // occupancy map;
  vector<Blizzard> blizzards;  // location and direction of blizzards

  parse(in, map, blizzards);

  queue<Vector2D> waypoints;
  waypoints.emplace(1, 0);
  waypoints.emplace(map.sizeX() - 2, map.sizeY() - 1);

  int32_t solution = solve(map, blizzards, waypoints);

  return to_string(solution);
}

inline Solution part2(std::ifstream& in)
{
  Matrix2D<bool> map;          // occupancy map;
  vector<Blizzard> blizzards;  // location and direction of blizzards

  parse(in, map, blizzards);

  queue<Vector2D> waypoints;
  waypoints.emplace(1, 0);
  waypoints.emplace(map.sizeX() - 2, map.sizeY() - 1);
  waypoints.emplace(1, 0);
  waypoints.emplace(map.sizeX() - 2, map.sizeY() - 1);

  int32_t solution = solve(map, blizzards, waypoints);

  return to_string(solution);
}

}  // namespace

Solution day24(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}