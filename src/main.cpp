#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <string>

#include "solutions.h"

constexpr std::int32_t kNumRuns = 1000;
constexpr bool kBenchmark = false;
constexpr bool kTestInputOnly = false;

struct Benchmark {
  double mean = 0;
  std::int64_t minimum = 0;
  std::int64_t maximum = 0;
  double standard_deviation = 0;
};

struct Run {
  std::string input_path;
  Problem problem;
  Benchmark benchmark;
  std::string solution;
};

void solve(Run& r, int32_t part);
Benchmark benchmark(Problem p, std::ifstream& file, std::int8_t part);
void printDivider();
void printHeader(Year y);
void printRun(std::string label, Run r, bool pass);
void printFooter();

int main()
{
  const Problems& problems = getProblems();

  for (const Year& year : problems) {
    if (!year.run) continue;
    printHeader(year);
    Run run;
    for (size_t i = 0; i < year.problems.size(); ++i) {
      for (std::int8_t part = 0; part < kNumParts; ++part) {
        run.input_path = std::format("src/{}/day{:02}/example_input.txt",
                                     year.folder, i + 1);
        run.problem = year.problems[i];
        solve(run, part);
        bool pass = run.solution == run.problem.sample_expected[part] ||
                    !run.problem.run;
        std::string label = std::format("day{:02}-{}-ex", i + 1, part + 1);
        printRun(label, run, pass);
      }

      if (!kTestInputOnly) {
        for (std::int8_t part = 0; part < kNumParts; ++part) {
          run.input_path =
              std::format("src/{}/day{:02}/input.txt", year.folder, i + 1);
          run.problem = year.problems[i];
          solve(run, part);
          bool pass =
              run.solution == run.problem.expected[part] || !run.problem.run;
          std::string label = std::format("day{:02}-{}", i + 1, part + 1);
          printRun(label, run, pass);
        }
      }

      printDivider();
    }

    std::cout << std::endl;
  }

  printFooter();
  return 0;
}

void solve(Run& r, int32_t part)
{
  std::ifstream file(r.input_path);

  std::string s;
  if (r.problem.run) {
    r.solution = r.problem.solve(file, part + 1);
  } else {
    r.solution = "not run";
  }

  if (kBenchmark && r.problem.run) {
    r.benchmark = benchmark(r.problem, file, part + 1);
  } else {
    r.benchmark = Benchmark();
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

void printHeader(Year y)
{
  constexpr const char* year_header = "| {:^94} |";
  constexpr const char* header_row =
      "| {:^10} | {:^22} | {:^4} | {:^10} | {:^10} | {:^10} | {:^10} |";
  printDivider();
  std::cout << std::format(year_header, y.str) << std::endl;
  printDivider();
  std::cout << std::format(header_row, "Problem", "Solution", "Fail",
                           "E.T. Mean", "E.T. STD", "E.T. Min", "E.T. Max")
            << std::endl;
  printDivider();
}

void printDivider()
{
  constexpr const char* table_div =
      "+{0:->12}+{0:->24}+{0:->6}+{0:->12}+{0:->12}+{0:->12}+{0:->12}+";
  std::cout << std::format(table_div, "") << std::endl;
}

void printRun(std::string label, Run r, bool pass)
{
  constexpr const char* table_row =
      "| {:<10} | {:<22} | {:^4} | {:>10.0f} | {:>10.0f} | {:>10} | {:>10} |";
  std::cout << std::format(table_row, label, r.solution, pass ? "" : "x",
                           r.benchmark.mean, r.benchmark.standard_deviation,
                           r.benchmark.minimum, r.benchmark.maximum)
            << std::endl;
}

void printFooter()
{
  if (kBenchmark) {
    std::cout
        << "Execution times are specified in microseconds.  Number of runs: "
        << kNumRuns << std::endl;
  } else {
    std::cout << "Benchmarks were not run. Execution times are invalid. "
              << std::endl;
  }
}