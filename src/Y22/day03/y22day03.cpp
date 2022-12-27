#include <array>
#include <unordered_set>

#include "y22.h"

namespace {
using namespace std;

inline int32_t getPriority(char item)
{
  if (item >= 'a') {
    return item + 1 - 'a';
  } else {
    return item + 27 - 'A';
  }
}

inline string part1(ifstream& in)
{
  std::int32_t total_priority(0);

  std::string line;
  while (getline(in, line)) {
    std::size_t compartment_size = line.size() / 2.0;

    {
      for (auto it = line.begin(); it != line.begin() + compartment_size;
           ++it) {
        auto duplicate_item =
            std::find(line.begin() + compartment_size, line.end(), *it);
        if (duplicate_item != line.end()) {
          total_priority += getPriority(*it);
          break;
        }
      }
    }

    /*
     * Alternative STL solution
    {
      std::vector<char> intersection;
      std::sort(line.begin(), line.begin() + compartment_size);
      std::sort(line.begin() + compartment_size, line.end());
      std::set_intersection(line.begin(), line.begin() + compartment_size,
                            line.begin() + compartment_size, line.end(),
                            std::back_inserter(intersection));
      if (!intersection.empty()) {
        total_priority += getPriority(intersection.at(0));
      }
    }
    */
  }

  return std::to_string(total_priority);
}

inline string part2(ifstream& in)
{
  std::int32_t priority(0);
  std::int32_t total_priority(0);
  constexpr std::int32_t num_packs = 3;

  std::string line;
  while (in.is_open() && !in.eof()) {
    std::array<int, 52> priorities{0};
    bool done{false};

    for (size_t i = 0; (i < num_packs) && !done; ++i) {
      if (!getline(in, line)) {
        break;
      }
      // Convert to set to eliminate duplicates
      std::unordered_set<char> pack(line.begin(), line.end());
      for (const char& item : pack) {
        priority = getPriority(item);
        priorities[priority - 1]++;
        if (priorities[priority - 1] == num_packs) {
          // Once a priority has occured three times, we are done
          total_priority += priority;
          done = true;
          break;
        }
      }
    }
  }

  return std::to_string(total_priority);
}

}  // namespace

string y22day03(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}