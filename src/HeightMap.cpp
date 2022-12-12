#include "HeightMap.h"

#include <array>
#include <format>
#include <iostream>
#include <stack>
#include <string>

void HeightMap::parseStream(std::istream& in)
{
  std::string line;
  while (std::getline(in, line)) {
    size_.x = line.size();
    map_.emplace_back(size_.x, 0);
    for (int32_t i = 0; i < size_.x; ++i) {
      if (line[i] == 'S') {
        line[i] = 'a';
        start_.x = i;
        start_.y = size_.y;
      } else if (line[i] == 'E') {
        line[i] = 'z';
        end_.x = i;
        end_.y = size_.y;
      }
      map_.back()[i] = line[i] - 'a';
    }
    size_.y++;
  }

  computeCost();
}

void HeightMap::printMap() const
{
  for (const auto& row : map_) {
    for (const auto& height : row) {
      std::cout << static_cast<char>(height + 'a');
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << std::format("Start: ({},{}), End: ({},{})", start_.x, start_.y,
                           end_.x, end_.y)
            << std::endl;
}

void HeightMap::printCostMap() const
{
  for (const auto& row : cost_map_) {
    for (const auto& cost : row) {
      if (cost == -1) {
        std::cout << "___ ";
      } else {
        std::cout << std::format("{:03} ", cost%100);
      }
    }
    std::cout << std::endl;
  }
}

int32_t HeightMap::costStartToEnd() const
{
  return cost_map_[start_.y][start_.x];
}

int32_t HeightMap::costShortestHike() const
{
  int32_t shortest = std::numeric_limits<int32_t>::max();
  for (int32_t x = 0; x < size_.x; ++x) {
    for (int32_t y = 0; y < size_.y; ++y) {
      if (height(x, y) == 0 && cost(x, y) != -1) {
        shortest = std::min(shortest, cost(x, y));
      }
    }
  }
  return shortest;
}

void HeightMap::computeCost()
{
  constexpr std::array<Vector2D, 4> relative_neighbors = {
      Vector2D(-1, 0),
      Vector2D(1, 0),
      Vector2D(0, -1),
      Vector2D(0, 1),
  };

  cost_map_ = std::vector<std::vector<int32_t>>(
      size_.y, std::vector<int32_t>(size_.x, -1));

  cost_map_[end_.y][end_.x] = 0;
  std::stack<Vector2D> to_check;
  to_check.push(end_);

  while (!to_check.empty()) {
    const Vector2D current = to_check.top();
    const int32_t current_cost = cost(current);
    to_check.pop();

    for (const auto& rn : relative_neighbors) {
      const Vector2D neighbor = current + rn;
      // Can we go here?
      if (valid(neighbor) && height(current) <= height(neighbor) + 1) {
        // Are we cheaper?
        if (cost(neighbor) == -1 || cost(neighbor) > current_cost + 1) {
          cost_map_[neighbor.y][neighbor.x] = current_cost + 1;
          to_check.push(neighbor);
        }
      }
    }
  }
}

int32_t HeightMap::height(const Vector2D& c) const { return map_[c.y][c.x]; }

int32_t HeightMap::height(const int32_t& x, const int32_t& y) const
{
  return map_[y][x];
}

int32_t HeightMap::cost(const Vector2D& c) const { return cost_map_[c.y][c.x]; }

int32_t HeightMap::cost(const int32_t& x, const int32_t& y) const
{
  return cost_map_[y][x];
}

bool HeightMap::valid(const Vector2D& c) const
{
  return c.x >= 0 && c.x < size_.x && c.y >= 0 && c.y < size_.y;
}