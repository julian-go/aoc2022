#include <iostream>
#include <string>

#include "solutions.h"

namespace {

template<size_t kSyncLength>
inline Solution findSyncPosition(std::ifstream& in)
{
  size_t sync_start = 0;
  std::string message;

  in >> message;

  for (size_t i = 1; i < message.size(); ++i) {
    for (size_t j = sync_start; j < i; ++j) {
      if (message[j] == message[i]) {
        sync_start = j + 1;
      }
    }
    if (i - sync_start == kSyncLength - 1) {
      return std::to_string(i + 1);
    }
  }

  return "ERROR";
}

}  // namespace

Solution day06(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return findSyncPosition<4>(in);
  } else {
    return findSyncPosition<14>(in);
  }
}