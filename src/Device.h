#ifndef DEVICE_H_
#define DEVICE_H

#include <array>
#include <istream>
#include <string>

#include "HeightMap.h"

class Device {
 public:
  Device();
  virtual ~Device();

  // day 10
  void processCommandStream(std::istream& in);
  std::int32_t getTotalSignalStrength() const;  // day10-1
  std::string render(bool show) const;          // day10-2

  // day 12
  HeightMap height_map;

 private:
  // day 10
  static constexpr std::int32_t screen_size_x = 40;
  static constexpr std::int32_t screen_size_y = 6;

  std::int32_t cycle = 0;
  std::int32_t register_x = 1;
  std::int32_t total_strength = 0;
  std::array<std::string, screen_size_y> screen;

  void cycleTick();
};

#endif  // !DEVICE_H_
