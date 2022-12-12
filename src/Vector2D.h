#ifndef VECTOR_2D_
#define VECTOR_2D_

#include <cstdint>

struct Vector2D {
 public:
  constexpr Vector2D() : x(0), y(0){};
  constexpr Vector2D(std::int32_t x, std::int32_t y) : x(x), y(y){};
  constexpr virtual ~Vector2D() = default;

  Vector2D operator+(const Vector2D& r) const { return Vector2D(x + r.x, y + r.y); }

  std::int32_t x;
  std::int32_t y;
};

#endif  // !VECTOR_2D_
