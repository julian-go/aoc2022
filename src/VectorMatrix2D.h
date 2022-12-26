#ifndef VECTOR_MATRIX_2D_H_
#define VECTOR_MATRIX_2D_H_

#include <type_traits>
#include <vector>

template <typename TType, bool TRowMajor = true>
class VectorMatrix2D {
 public:
  VectorMatrix2D() : size_x_(0), size_y_(0) {}
  VectorMatrix2D(size_t size_x, size_t size_y) { resize(size_x, size_y); }
  VectorMatrix2D(size_t size_x, size_t size_y, const TType& initial)
  {
    resize(size_x, size_y);
    fill(initial);
  }
  ~VectorMatrix2D() = default;

  template <typename T = TType,
            typename std::enable_if_t<!std::is_same_v<T, bool>>* = nullptr>
  T& at(const size_t& x, const size_t& y)
  {
    return data_[index(x, y)];
  }

  template <typename T = TType,
            typename std::enable_if_t<!std::is_same_v<T, bool>>* = nullptr>
  const TType& at(const size_t& x, const size_t& y) const
  {
    return data_[index(x, y)];
  }

  template <typename T = TType,
            typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
  T at(const size_t& x, const size_t& y) const
  {
    return data_[index(x, y)];
  }

  template <typename T = TType,
            typename std::enable_if_t<std::is_same_v<T, bool>>* = nullptr>
  void set(const size_t& x, const size_t& y, T value)
  {
    data_[index(x, y)] = value;
  }

  void resize(const size_t& size_x, const size_t& size_y)
  {
    size_x_ = size_x;
    size_y_ = size_y;
    data_.resize(size_x * size_y);
  }

  void fill(const TType& value)
  {
    for (size_t i = 0; i < data_.size(); ++i) {
      data_[i] = value;
    }
  }

  size_t sizeX() const { return size_x_; }
  size_t sizeY() const { return size_y_; }

 private:
  std::vector<TType> data_;
  size_t size_x_;
  size_t size_y_;

  size_t index(const size_t& x, const size_t& y) const
  {
    if (TRowMajor) {
      return y * size_x_ + x;
    } else {
      return x * size_y_ + y;
    }
  }
};

#endif  // !VECTOR_MATRIX_2D_H_
