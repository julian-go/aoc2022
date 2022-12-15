#include <iostream>
#include <vector>

#include "solutions.h"

namespace {

inline int32_t getScore(const std::vector<std::vector<int8_t>>& trees,
                        const int32_t row, const int32_t col)
{
  const int32_t height = trees[row][col];
  const int32_t max_row = trees.size();
  const int32_t max_col = trees.front().size();
  int32_t score = 1;

  if (row == 0 || row == max_row - 1 || col == 0 || col == max_col - 1) {
    return 0;
  }

  // Check up
  for (int32_t i = row - 1;; --i) {
    if (trees[i][col] >= height || i == 0) {
      score *= (row - i);
      break;
    }
  }
  // Check down
  for (int32_t i = row + 1;; ++i) {
    if (trees[i][col] >= height || i == max_row - 1) {
      score *= (i - row);
      break;
    }
  }
  // Check left
  for (int32_t i = col - 1;; --i) {
    if (trees[row][i] >= height || i == 0) {
      score *= (col - i);
      break;
    }
  }
  // Check right
  for (int32_t i = col + 1;; ++i) {
    if (trees[row][i] >= height || i == max_col - 1) {
      score *= (i - col);
      break;
    }
  }
  return score;
}

inline Solution part1(std::ifstream& in)
{
  std::string line;
  int32_t num_visible = 0;
  std::vector<std::vector<int8_t>> trees;
  std::vector<std::vector<bool>> visible;

  while (getline(in, line)) {
    trees.emplace_back(line.size());
    for (size_t i = 0; i < line.size(); ++i) {
      trees.back()[i] = line.at(i) - '0';
    }
  }

  visible.resize(trees.size(), std::vector<bool>(trees.front().size(), false));
  int8_t max = -1;
  const auto updateVisible = [&](int32_t row, int32_t col) {
    visible[row][col] = visible[row][col] || trees[row][col] > max;
    if (trees[row][col] > max) {
      max = trees[row][col];
    }
  };

  for (int32_t row = 0; row < visible.size(); ++row) {
    max = -1;
    for (int32_t col = 0; col < visible.front().size(); ++col) {
      updateVisible(row, col);
    }
    max = -1;
    for (int32_t col = visible.front().size() - 1; col >= 0; --col) {
      updateVisible(row, col);
    }
  }

  for (int32_t col = 0; col < visible.front().size(); ++col) {
    max = -1;
    for (int32_t row = 0; row < visible.size(); ++row) {
      updateVisible(row, col);
    }
    max = -1;
    for (int32_t row = visible.size() - 1; row >= 0; --row) {
      updateVisible(row, col);
      if (visible[row][col]) {
        num_visible++;
      }
    }
  }

  return std::to_string(num_visible);
}

inline Solution part2(std::ifstream& in)
{
  std::string line;
  int32_t max_score = 0;
  std::vector<std::vector<int8_t>> trees;

  while (getline(in, line)) {
    trees.emplace_back(line.size());
    for (size_t i = 0; i < line.size(); ++i) {
      trees.back()[i] = line.at(i) - '0';
    }
  }

  int32_t score;
  for (int32_t row = 0; row < trees.size(); ++row) {
    for (int32_t col = 0; col < trees.front().size(); ++col) {
      score = getScore(trees, row, col);
      max_score = std::max(max_score, score);
    }
  }

  return std::to_string(max_score);
}

}  // namespace

Solution day08(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}