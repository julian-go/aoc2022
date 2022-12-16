#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <string>

#include "solutions.h"

constexpr const char* table_div =
    "+{0:->12}+{0:->24}+{0:->6}+{0:->12}+{0:->12}+{0:->12}+{0:->12}+";
constexpr const char* header_row =
    "| {:^10} | {:^22} | {:^4} | {:^10} | {:^10} | {:^10} | {:^10} |";
constexpr const char* table_row =
    "| {:<10} | {:<22} | {:^4} | {:>10.0f} | {:>10.0f} | {:>10} | {:>10} |";
constexpr std::int32_t kNumRuns = 100;
constexpr std::int8_t kNumParts = 2;
constexpr bool kBenchmark = true;

using Solve = std::function<Solution(std::ifstream&, std::int8_t part)>;

struct Problem {
  Solve solve;
  std::array<Solution, kNumParts> expected;
};

struct Benchmark {
  double mean = 0;
  std::int64_t minimum = 0;
  std::int64_t maximum = 0;
  double standard_deviation = 0;
};

void solve(Problem p, std::int32_t day);
Benchmark benchmark(Problem p, std::ifstream& file, std::int8_t part);

int main()
{
  using std::cout;
  std::array problems = {Problem{day01, {"71300", "209691"}},
                         Problem{day02, {"12772", "11618"}},
                         Problem{day03, {"8298", "2708"}},
                         Problem{day04, {"584", "933"}},
                         Problem{day05, {"TWSGQHNHL", "JNRSCDWPP"}},
                         Problem{day06, {"1723", "3708"}},
                         Problem{day07, {"2061777", "4473403"}},
                         Problem{day08, {"1543", "595080"}},
                         Problem{day09, {"5874", "2467"}},
                         Problem{day10, {"13920", "10069953020766180745"}},
                         Problem{day11, {"67830", "15305381442"}},
                         Problem{day12, {"456", "454"}},
                         Problem{day13, {"6101", "21909"}},
                         Problem{day14, {"1001", "27976"}},
                         Problem{day15, {"4560025", "12480406634249"}},
                         Problem{day16, {"0", "0"}}};

  cout << std::format(table_div, "") << std::endl;
  cout << std::format(header_row, "Problem", "Solution", "Fail", "E.T. Mean",
                      "E.T. STD", "E.T. Min", "E.T. Max")
       << std::endl;
  cout << std::format(table_div, "", "", "", "", "", "", "") << std::endl;

  for (size_t i = 0; i < problems.size(); ++i) {
    solve(problems[i], i);
  }

  cout << std::format(table_div, "") << std::endl;
  cout << std::endl;
  if (kBenchmark) {
    cout << "Execution times are specified in microseconds.  Number of runs: "
         << kNumRuns << std::endl;
  } else {
    cout << "Benchmarks were not run. Execution times are invalid. "
         << std::endl;
  }

  return 0;
}

void solve(Problem p, std::int32_t day)
{
  for (std::int8_t part = 0; part < kNumParts; ++part) {
    std::ifstream file(std::format("input/day{:02}.txt", day + 1));

    Solution s;
    if (p.solve) {
      s = p.solve(file, part + 1);
    }

    Benchmark b;
    if (kBenchmark) {
      b = benchmark(p, file, part + 1);
    }

    std::string problem = std::format("day{:02}-{}", day + 1, part + 1);
    std::cout << std::format(table_row, problem, s,
                             s == p.expected[part] ? "" : "x", b.mean,
                             b.standard_deviation, b.minimum, b.maximum)
              << std::endl;
  }
}

Benchmark benchmark(Problem p, std::ifstream& file, std::int8_t part)
{
  using std::chrono::duration_cast;
  using std::chrono::microseconds;
  using std::chrono::steady_clock;

  steady_clock::time_point start;
  Benchmark b;
  b.minimum = std::numeric_limits<std::int64_t>::max();

  std::vector<std::int64_t> v;

  double variance = 0.0;
  double mean_old = 0.0;

  for (std::int32_t i = 0; i < kNumRuns; ++i) {
    file.clear();
    file.seekg(0, std::ios::beg);

    start = steady_clock::now();

    if (p.solve) {
      p.solve(file, part);
    }

    auto elapsed =
        duration_cast<microseconds>(steady_clock::now() - start).count();

    v.push_back(elapsed);

    b.minimum = std::min(b.minimum, elapsed);
    b.maximum = std::max(b.maximum, elapsed);
    // Incremental variance according to:
    // https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm
    // mean = mean + (value - mean) / n
    // variance = variance + ((val - mean) * (val - old_mean) - variance) / n
    int n = i + 1;
    mean_old = b.mean;
    b.mean += (static_cast<double>(elapsed) - b.mean) / n;
    variance =
        variance + ((elapsed - b.mean) * (elapsed - mean_old) - variance) / n;
  }

  b.standard_deviation = std::sqrt(variance);
  return b;
}