#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Vector2D.h"
#include "solutions.h"

namespace {

using Vec2D = Vector2D<int32_t>;

class RopeSegment {
 public:
  RopeSegment() = default;
  ~RopeSegment() { delete next_; };

  void move(const Vec2D& v)
  {
    position_ += v;
    next_->follow(*this);
  }

  void follow(const RopeSegment& rs)
  {
    bool should_move = false;
    const Vec2D distance = rs.getPosition() - position_;
    Vec2D move;
    if (std::abs(distance.x) > 1) {
      move.x += distance.x / std::abs(distance.x);
      if (std::abs(distance.y) > 0) {
        move.y += distance.y / std::abs(distance.y);
      }
      should_move = true;
    } else if (std::abs(distance.y) > 1) {
      move.y += distance.y / std::abs(distance.y);
      if (std::abs(distance.x) > 0) {
        move.x += distance.x / std::abs(distance.x);
      }
      should_move = true;
    }

    if (should_move) {
      position_ += move;
      if (next_ != nullptr) {
        next_->follow(*this);
      }
    }
  }

  void setNext(RopeSegment* next) { next_ = next; }
  const Vec2D& getPosition() const { return position_; }

 private:
  RopeSegment* next_ = nullptr;
  Vec2D position_;
};

template <int32_t kNumSegments>
inline Solution solve(std::ifstream& in)
{
  RopeSegment head;
  RopeSegment* tail = &head;
  for (int32_t i = 1; i < kNumSegments; ++i) {
    RopeSegment* new_tail = new RopeSegment();
    tail->setNext(new_tail);
    tail = new_tail;
  }

  char c;
  int32_t steps;
  std::unordered_set<Vec2D, Vec2D::Hash> visited;
  while (in >> c >> steps) {
    while (steps > 0) {
      switch (c) {
        case 'U':
          head.move(Vector2D(0, 1));
          break;
        case 'D':
          head.move(Vector2D(0, -1));
          break;
        case 'L':
          head.move(Vector2D(1, 0));
          break;
        case 'R':
          head.move(Vector2D(-1, 0));
          break;
      }
      visited.insert(tail->getPosition());
      steps--;
    }
  }

  return std::to_string(visited.size());
}

}  // namespace

Solution day09(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return solve<2>(in);
  } else {
    return solve<10>(in);
  }
}