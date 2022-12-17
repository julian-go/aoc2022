#include <array>
#include <iostream>
#include <utility>
#include <vector>

#include "Vector2D.h"
#include "solutions.h"

namespace {

using namespace std;

template <size_t size_x, size_t size_y>
class RingBuffer {
 public:
  RingBuffer()
  {
    std::array<bool, size_y> tmp;
    tmp.fill(false);
    buffer_.fill(tmp);
  };

  bool at(size_t x, size_t y) const { return buffer_[x][y % size_y]; }

  void set(size_t x, size_t y, bool value) { buffer_[x][y % size_y] = value; }

  void clearAbove(size_t y_clear, int8_t count)
  {
    for (size_t x = 0; x < size_x; ++x) {
      for (size_t y = y_clear; y < y_clear + count; ++y) {
        buffer_[x][y % size_y] = false;
      }
    }
  }

  size_t x_max() { return size_x; }
  size_t y_max() { return size_y; }

 private:
  array<array<bool, size_y>, size_x> buffer_;
};

class Tetris {
  using Block = array<array<bool, 4>, 4>;

 public:
  Tetris() = default;

  void process(istream& in)
  {
    string commands;
    getline(in, commands);
    spawnBlock();
    int64_t last_floor = 0;
    int64_t last_count = 0;
    int64_t floor_bonus = 0;

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
        if (counter == 1'000'000'000'000) {
          cout << "<" << floor_ + floor_bonus << ">" << endl;
          done = true;
          break;
        }
      }
      ++i;
      i %= commands.size();
      if (i == 0) {
        done = true;
        for (size_t x = 0; x < top.x_max(); ++x) {
          for (size_t y = 0; y < top.y_max(); ++y) {
            done &= top.at(x, y) == game_state_.at(x, floor_ - y);
            top.set(x, y, game_state_.at(x, floor_ - y));
          }
        }
      }

      if (i == 0 && !done) {
        last_floor = floor_;
        last_count = counter;
      }
      if (done) {
        done = false;
        /*   cout << "loop:" << endl;
           cout << "   Floor   " << floor_ - last_floor << endl;
           cout << "   Counter " << counter - last_count << endl;*/

        // lets feed forward a little
        int64_t dFloor = floor_ - last_floor;
        int64_t dCounter = counter - last_count;

        int64_t runs_to_go = (1'000'000'000'000 - counter) / dCounter;

        counter += runs_to_go * dCounter;
        floor_bonus = runs_to_go * dFloor;

        last_floor = floor_;
        last_count = counter;
      }
      // printGame();
    }
  }

  void spawnBlock()
  {
    active_block_++;
    active_block_ %= kBlocks.size();

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

  bool tryMoveBlock(int64_t new_y, int8_t new_x)
  {
    const Block& block = kBlocks[active_block_];
    for (int64_t y = 0; y < 4; ++y) {
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
        if (block[x][y] && game_state_.at(new_x + x, new_y + y)) {
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
    for (int64_t y = 0; y < 4; ++y) {
      for (int8_t x = 0; x < 4; ++x) {
        if (block[x][y]) {
          game_state_.set(active_block_x_ + x, active_block_y_ + y, true);
        }
      }
    }

    for (int64_t y = 0; y < 4; ++y) {
      bool full = true;
      for (int8_t x = 0; x < kWidth; ++x) {
        full &= game_state_.at(x, active_block_y_ + y);
      }
    }
  }

  void updateFloor()
  {
    for (int64_t y = floor_; y < floor_ + 10; ++y) {
      bool found_block = false;
      for (int32_t x = 0; x < kWidth; ++x) {
        if (game_state_.at(x, y)) {
          found_block = true;
          break;
        }
      }
      if (!found_block) {
        floor_ = y;
        break;
      }
    }
    game_state_.clearAbove(floor_ + 4, kSpawnY + 4);
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
    constexpr int64_t context_up = 10;
    constexpr int64_t context_down = 20;
    for (int64_t y = floor_ + context_up; y >= floor_ - context_down && y >= 0;
         --y) {
      for (int32_t x = 0; x < kWidth; ++x) {
        string s = (game_state_.at(x, y) ? "#" : ".");
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
  static constexpr int32_t kBufferSize = 30'000;
  static constexpr int8_t kWidth = 7;
  static constexpr int8_t kSpawnX = 2;
  static constexpr int8_t kSpawnY = 3;

  // std::array<std::array<bool, kBufferSize>, kWidth> game_state_;
  RingBuffer<kWidth, kBufferSize> game_state_;
  RingBuffer<kWidth, 10'000> top;

  static constexpr array<Block, 5> kBlocks = {
      Block({array<bool, 4>({1, 0, 0, 0}), array<bool, 4>({1, 0, 0, 0}),
             array<bool, 4>({1, 0, 0, 0}), array<bool, 4>({1, 0, 0, 0})}),
      Block({array<bool, 4>({0, 1, 0, 0}), array<bool, 4>({1, 1, 1, 0}),
             array<bool, 4>({0, 1, 0, 0}), array<bool, 4>({0, 0, 0, 0})}),
      Block({array<bool, 4>({1, 0, 0, 0}), array<bool, 4>({1, 0, 0, 0}),
             array<bool, 4>({1, 1, 1, 0}), array<bool, 4>({0, 0, 0, 0})}),
      Block({array<bool, 4>({1, 1, 1, 1}), array<bool, 4>({0, 0, 0, 0}),
             array<bool, 4>({0, 0, 0, 0}), array<bool, 4>({0, 0, 0, 0})}),
      Block({array<bool, 4>({1, 1, 0, 0}), array<bool, 4>({1, 1, 0, 0}),
             array<bool, 4>({0, 0, 0, 0}), array<bool, 4>({0, 0, 0, 0})})};

  int8_t active_block_ = -1;
  int8_t active_block_x_ = 0;
  int64_t active_block_y_ = 0;
  int64_t floor_ = 0;
  int64_t counter = 0;
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