#include "Device.h"

#include <iostream>

Device::Device() { screen.fill(std::string(screen_size_x, '.')); }

Device::~Device() = default;

void Device::processCommandStream(std::istream& in)
{
  std::string operation;
  std::int32_t argument;

  cycle = 0;
  register_x = 1;
  total_strength = 0;

  while (in >> operation) {
    if (operation == "noop") {
      // do nothing
      cycleTick();
    } else {
      in >> argument;
      cycleTick();
      cycleTick();
      register_x += argument;
    }
  }
}

std::int32_t Device::getTotalSignalStrength() const { return total_strength; }

std::string Device::render(bool show) const
{
  std::string solution = "";
  for (const auto& line : screen) {
    if (show) {
      std::cout << line << std::endl;
    }
    solution += line;
  }
  size_t solution_hash = std::hash<std::string>{}(solution);
  return std::to_string(solution_hash);
}

void Device::cycleTick()
{
  std::int32_t screen_x = cycle % screen_size_x;
  std::int32_t screen_y = cycle / screen_size_x;

  if (register_x + 1 >= screen_x && register_x - 1 <= screen_x) {
    screen[screen_y][screen_x] = '#';
  }
  cycle++;
  if ((cycle - 20) % 40 == 0) {
    total_strength += cycle * register_x;
  }
}
