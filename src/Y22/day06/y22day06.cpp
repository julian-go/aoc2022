#include "y22.h"

namespace {
using namespace std;

template <size_t kSyncLength>
inline string findSyncPosition(std::ifstream& in)
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

string y22day06(ifstream& in, int8_t part)
{
  if (part == 1) {
    return findSyncPosition<4>(in);
  } else {
    return findSyncPosition<14>(in);
  }
}