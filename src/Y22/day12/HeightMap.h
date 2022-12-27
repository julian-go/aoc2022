#ifndef HEIGHT_MAP_
#define HEIGHT_MAP_

#include <istream>
#include <vector>

#include "utils/Vector2D.h"

class HeightMap {
using Vec2D = Vector2D<int32_t>;

 public:
  HeightMap() = default;
  virtual ~HeightMap() = default;

  void parseStream(std::istream& in);

  void printMap() const;
  void printCostMap() const;

  int32_t costStartToEnd() const;    // day12-1
  int32_t costShortestHike() const;  // day12-2

 private:
  void computeCost();
  int32_t height(const Vec2D& c) const;
  int32_t height(const int32_t& x, const int32_t& y) const;
  int32_t cost(const Vec2D& c) const;
  int32_t cost(const int32_t& x, const int32_t& y) const;
  bool valid(const Vec2D& c) const;

  std::vector<std::vector<int32_t>> map_;
  std::vector<std::vector<int32_t>> cost_map_;
  Vec2D size_;
  Vec2D start_;
  Vec2D end_;
};

#endif  // !HEIGHT_MAP_
