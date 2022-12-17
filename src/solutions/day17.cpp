#include <array>
#include <iostream>
#include <utility>
#include <vector>

#include "Vector2D.h"
#include "solutions.h"

namespace {

using namespace std;

class Tetris {
  using Block = std::array<std::array<bool, 4>, 4>;

 public:
  Tetris()
  {
    game_state_ = {};
    for (auto& col : game_state_) {
      col = {};
      col.fill(false);
    }

    spawnBlock();
  }

  void process(istream& in)
  {
    string commands;
    getline(in, commands);

    bool done = false;
    int i = 0;
    while (!done) {
      if (commands[i] == '>') {
        rightBlock();
      } else {
        leftBlock();
      }
      if (!downBlock()) {
        lockBlock();
        counter++;
        updateFloor();
        spawnBlock();
        if (counter == 2022) {
          cout << floor_;
          done = true;
        }
      }
      ++i;
      i %= commands.size();
      //printGame();
    }
  }

  void spawnBlock()
  {
    active_block_++;
    active_block_ %= 5;

    active_block_y_ = floor_ + kSpawnY;
    active_block_x_ = kSpawnX;
  }

  bool downBlock()
  {
    return tryMoveBlock(active_block_y_ - 1, active_block_x_);
  }

  bool leftBlock()
  {
    return tryMoveBlock(active_block_y_, active_block_x_ - 1);
  }

  bool rightBlock()
  {
    return tryMoveBlock(active_block_y_, active_block_x_ + 1);
  }

  bool tryMoveBlock(int32_t new_y, int8_t new_x)
  {
    const Block& block = kBlocks[active_block_];
    for (int32_t y = 0; y < 4; ++y) {
      for (int8_t x = 0; x < 4; ++x) {
        if (new_y < 0) {
          // floor
          return false;
        }
        if (block[x][y] && (new_x + x < 0 || new_x + x >= kWidth)) {
          // wall
          return false;
        }
        if (new_x + x < 0 || new_x + x >= kWidth) {
          continue;
        }
        if (block[x][y] && game_state_[new_x + x][new_y + y]) {
          return false;
        }
      }
    }

    active_block_y_ = new_y;
    active_block_x_ = new_x;
    return true;
  }

  void lockBlock()
  {
    const Block& block = kBlocks[active_block_];
    for (int32_t y = 0; y < 4; ++y) {
      for (int8_t x = 0; x < 4; ++x) {
        if (block[x][y]) {
          game_state_[active_block_x_ + x][active_block_y_ + y] = true;
        }
      }
    }
  }

  void updateFloor()
  {
    for (int32_t y = floor_; y < game_state_.front().size(); ++y) {
      bool found_block = false;
      for (int32_t x = 0; x < kWidth; ++x) {
        if (game_state_[x][y]) {
          found_block = true;
          break;
        }
      }
      if (!found_block) {
        floor_ = y;
        break;
      }
    }
  }

  void printBlocks()
  {
    for (const Block& b : kBlocks) {
      for (size_t y = 0; y < b.front().size(); ++y) {
        for (size_t x = 0; x < b.size(); ++x) {
          cout << (b[x][y] ? "#" : ".");
        }
        cout << endl;
      }
      cout << endl;
    }
  }

  void printGame()
  {
    constexpr int32_t context_up = 10;
    constexpr int32_t context_down = 4000;
    for (int32_t y = floor_ + context_up; y >= max(floor_ - context_down, 0);
         --y) {
      for (int32_t x = 0; x < kWidth; ++x) {
        string s = (game_state_[x][y] ? "#" : ".");
        if (x >= active_block_x_ && x < active_block_x_ + 4 &&
            y >= active_block_y_ && y < active_block_y_ + 4) {
          const Block& b = kBlocks[active_block_];
          if (b[x - active_block_x_][y - active_block_y_]) {
            s = "@";
          }
        }
        cout << s;
      }
      cout << endl;
    }
    cout << endl;
  }

 private:
  static constexpr int8_t kWidth = 7;
  static constexpr int8_t kSpawnX = 2;
  static constexpr int8_t kSpawnY = 3;

  std::array<std::array<bool, 15'000>, kWidth> game_state_;
  static constexpr std::array<Block, 5> kBlocks = {
      Block({std::array<bool, 4>({1, 0, 0, 0}),
             std::array<bool, 4>({1, 0, 0, 0}),
             std::array<bool, 4>({1, 0, 0, 0}),
             std::array<bool, 4>({1, 0, 0, 0})}),
      Block({std::array<bool, 4>({0, 1, 0, 0}),
             std::array<bool, 4>({1, 1, 1, 0}),
             std::array<bool, 4>({0, 1, 0, 0}),
             std::array<bool, 4>({0, 0, 0, 0})}),
      Block({std::array<bool, 4>({1, 0, 0, 0}),
             std::array<bool, 4>({1, 0, 0, 0}),
             std::array<bool, 4>({1, 1, 1, 0}),
             std::array<bool, 4>({0, 0, 0, 0})}),
      Block({std::array<bool, 4>({1, 1, 1, 1}),
             std::array<bool, 4>({0, 0, 0, 0}),
             std::array<bool, 4>({0, 0, 0, 0}),
             std::array<bool, 4>({0, 0, 0, 0})}),
      Block({std::array<bool, 4>({1, 1, 0, 0}),
             std::array<bool, 4>({1, 1, 0, 0}),
             std::array<bool, 4>({0, 0, 0, 0}),
             std::array<bool, 4>({0, 0, 0, 0})})};

  int8_t active_block_ = -1;
  int8_t active_block_x_ = 0;
  int32_t active_block_y_ = 0;
  int32_t floor_ = 0;
  int32_t counter = 0;
};

inline Solution part1(std::ifstream& in)
{
  Tetris t;
  // t.printBlocks();
  // t.printGame();
  t.process(in);
  return std::to_string(0);
}

inline Solution part2(std::ifstream& in) { return std::to_string(0); }

}  // namespace

Solution day17(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}