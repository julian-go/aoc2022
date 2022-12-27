#include <array>
#include <fstream>
#include <functional>
#include <string>

constexpr std::int8_t kNumParts = 2;
using Solve = std::function<std::string(std::ifstream&, std::int8_t part)>;

struct Problem {
  bool run;
  Solve solve;
  std::array<std::string, kNumParts> sample_expected;
  std::array<std::string, kNumParts> expected;
};

struct Year {
  bool run;
  std::string str;
  std::string folder;
  std::array<Problem, 25> problems;
};

using Problems = std::array<Year, 1>;

const Problems& getProblems();