#ifndef VECTOR_2D_
#define VECTOR_2D_

#include <cstdint>

struct Vector2D {
  constexpr Vector2D() : x(0), y(0){};
  constexpr Vector2D(std::int32_t x, std::int32_t y) : x(x), y(y){};
  constexpr virtual ~Vector2D() = default;

  Vector2D operator*(const int32_t& r) const { return Vector2D(x * r, y * r); }

  Vector2D operator+(const Vector2D& r) const
  {
    return Vector2D(x + r.x, y + r.y);
  }

  Vector2D operator-(const Vector2D& r) const
  {
    return Vector2D(x - r.x, y - r.y);
  }

  inline bool operator==(const Vector2D& r) const
  {
    return x == r.x && y == r.y;
  }

  inline bool operator<(const Vector2D& r) const
  {
    if (x < r.x) {
      return true;
    } else if (x == r.x) {
      return y < r.y;
    } else {
      return false;
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const Vector2D& v)
  {
    out << "(" << v.x << "," << v.y << ")";
    return out;
  }

  std::int32_t x;
  std::int32_t y;
};

struct Vector2DHash {
  size_t operator()(const Vector2D& v) const
  {
    return (v.x << 16) | (v.y & 0xFFFF);
  }
};

#endif  // !VECTOR_2D_
