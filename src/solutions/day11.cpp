#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "solutions.h"

namespace {
using std::int32_t;
using std::int8_t;

// Monkey for part 1
struct Monkey {
  std::vector<int32_t> items;
  std::function<void(int32_t&)> inspect;
  int32_t divison_test;
  int32_t true_friend;
  int32_t false_friend;
  int64_t monkey_business = 0;
};

std::vector<Monkey> createMonkeys(std::ifstream& in)
{
  std::vector<Monkey> monkeys;
  std::string tmp;

  while (!in.eof()) {
    monkeys.emplace_back();
    std::getline(in, tmp);  // skip first line

    in.ignore(256, ':');  // Skip until list of items
    int32_t item;
    do {
      in >> item >> tmp;
      monkeys.back().items.emplace_back(item);
    } while (tmp == ",");

    in.ignore(256, '=');  // Skip until operation
    std::string arg1, operation, arg2;
    in >> arg1 >> operation >> arg2;
    if (operation == "+" && arg2 == "old") {
      monkeys.back().inspect = [](int32_t& old) { old += old; };
    } else if (operation == "*" && arg2 == "old") {
      monkeys.back().inspect = [](int32_t& old) { old *= old; };
    } else {
      int32_t operand = std::stoi(arg2);
      if (operation == "+") {
        monkeys.back().inspect = [operand](int32_t& old) { old += operand; };
      } else if (operation == "*") {
        monkeys.back().inspect = [operand](int32_t& old) { old *= operand; };
      }
    }

    in.ignore(256, ':');  // Skip until test
    in >> tmp >> tmp >> monkeys.back().divison_test;

    in.ignore(256, ':');  // Skip until true friend
    in >> tmp >> tmp >> tmp >> monkeys.back().true_friend;

    in.ignore(256, ':');  // Skip until false friend
    in >> tmp >> tmp >> tmp >> monkeys.back().false_friend;

    in.ignore(256, '\n');  // skip last line
    std::getline(in, tmp);
  }

  return monkeys;
}

// Monkey for part 2
using Item = std::unordered_map<int32_t, int32_t>;

struct ScaryMonkey {
  std::vector<int32_t> initial_items;
  std::vector<Item> items;
  std::function<int32_t(int32_t, int32_t)> inspect;
  int32_t operand = 0;
  int32_t divison_test;
  int32_t true_friend;
  int32_t false_friend;
  int64_t monkey_business = 0;
};

std::vector<ScaryMonkey> createScaryMonkeys(std::ifstream& in)
{
  std::vector<ScaryMonkey> monkeys;
  std::string tmp;

  while (!in.eof()) {
    monkeys.emplace_back();
    std::getline(in, tmp);  // skip first line

    in.ignore(256, ':');  // Skip until list of items
    int32_t item;
    do {
      in >> item >> tmp;
      monkeys.back().initial_items.emplace_back(item);
    } while (tmp == ",");

    in.ignore(256, '=');  // Skip until operation
    std::string arg1, operation, arg2;
    in >> arg1 >> operation >> arg2;
    if (operation == "+" && arg2 == "old") {
      monkeys.back().inspect = [](int32_t l, int32_t r) { return l + l; };
    } else if (operation == "*" && arg2 == "old") {
      monkeys.back().inspect = [](int32_t l, int32_t r) { return l * l; };
    } else {
      monkeys.back().operand = std::stoi(arg2);
      if (operation == "+") {
        monkeys.back().inspect = [](int32_t l, int32_t r) { return l + r; };
      } else if (operation == "*") {
        monkeys.back().inspect = [](int32_t l, int32_t r) { return l * r; };
      }
    }

    in.ignore(256, ':');  // Skip until test
    in >> tmp >> tmp >> monkeys.back().divison_test;

    in.ignore(256, ':');  // Skip until true friend
    in >> tmp >> tmp >> tmp >> monkeys.back().true_friend;

    in.ignore(256, ':');  // Skip until false friend
    in >> tmp >> tmp >> tmp >> monkeys.back().false_friend;

    in.ignore(256, '\n');  // skip last line
    std::getline(in, tmp);
  }

  // Collect divisors
  std::vector<int32_t> divisors;
  for (ScaryMonkey& monkey : monkeys) {
    divisors.push_back(monkey.divison_test);
  }

  for (ScaryMonkey& monkey : monkeys) {
    for (int32_t& item : monkey.initial_items) {
      monkey.items.emplace_back(Item());
      for (int32_t& divisor : divisors) {
        monkey.items.back()[divisor] = item;
      }
    }
  }

  return monkeys;
}

inline Solution part1(std::ifstream& in)
{
  constexpr int8_t kNumRounds = 5;
  // constexpr int8_t kWorryDecay = 3;
  constexpr int8_t kWorryDecay = 1;

  std::vector<Monkey> monkeys = createMonkeys(in);
  for (int i = 0; i < kNumRounds; ++i) {
    int id = 0;
    for (Monkey& monkey : monkeys) {
      auto item = monkey.items.begin();
      while (item != monkey.items.end()) {
        monkey.inspect(*item);
        monkey.monkey_business++;
        *item /= kWorryDecay;
        if (*item % monkey.divison_test == 0) {
          monkeys[monkey.true_friend].items.push_back(*item);
          std::cout << "From " << id << " to " << monkey.true_friend
                    << std::endl;
        } else {
          monkeys[monkey.false_friend].items.push_back(*item);
          std::cout << "From " << id << " to " << monkey.false_friend
                    << std::endl;
        }
        item = monkey.items.erase(item);
      }
      id++;
      std::cout << std::endl;
    }
     int count = 0;
     for (Monkey& monkey : monkeys) {
       std::cout << count << ": ";
       count++;
       for (auto item : monkey.items) {
        std::cout << item << " ";
       }
       std::cout << std::endl;
     }
     std::cout << std::endl;
    std::cout << "----------------" << std::endl;
  }

  int64_t monkey_business_1 = 0;
  int64_t monkey_business_2 = 0;
  for (Monkey& monkey : monkeys) {
    if (monkey.monkey_business >= monkey_business_1) {
      monkey_business_2 = monkey_business_1;
      monkey_business_1 = monkey.monkey_business;
    } else {
      monkey_business_2 = std::max(monkey.monkey_business, monkey_business_2);
    }
  }

  Solution s = std::to_string(monkey_business_1 * monkey_business_2);
  return s;
}

inline Solution part2(std::ifstream& in)
{
  constexpr int32_t kNumRounds = 10'000;

  std::vector<ScaryMonkey> monkeys = createScaryMonkeys(in);
  int counter = 0;
  for (int i = 0; i < kNumRounds; ++i) {
    int id = 0;
    counter++;
    for (ScaryMonkey& monkey : monkeys) {
      auto item = monkey.items.begin();
      while (item != monkey.items.end()) {
        for (auto it = item->begin(); it != item->end(); ++it) {
          //if (id == 2) {
          //  std::cout << std::format("Value: {}, Operand: {}, Div: {}\n",
          //                           it->second, monkey.operand, it->first);
          //}
          it->second = monkey.inspect(it->second % it->first,
                                      monkey.operand % it->first) %
                       it->first;
        }
        monkey.monkey_business++;
        //counter++;
        if ((*item)[monkey.divison_test] == 0) {
          monkeys[monkey.true_friend].items.push_back(*item);
          //std::cout << "From " << id << " to " << monkey.true_friend
          //          << std::endl;
        } else {
          monkeys[monkey.false_friend].items.push_back(*item);
          //std::cout << "From " << id << " to " << monkey.false_friend
          //          << std::endl;
        }
        item = monkey.items.erase(item);
      }
      id++;
      //std::cout << std::endl;
    }
    //std::cout << "----------------" << std::endl;
    // int count = 0;
    // for (ScaryMonkey& monkey : monkeys) {
    //   std::cout << count << ": ";
    //   count++;
    //   for (auto item : monkey.items) {
    //      std::cout << "#";
    //   }
    //   std::cout << std::endl;
    // }
    // std::cout << std::endl;
  }
  std::cout << "Counter: " << counter << std::endl;

  int64_t monkey_business_1 = 0;
  int64_t monkey_business_2 = 0;
  for (ScaryMonkey& monkey : monkeys) {
    if (monkey.monkey_business >= monkey_business_1) {
      monkey_business_2 = monkey_business_1;
      monkey_business_1 = monkey.monkey_business;
    } else {
      monkey_business_2 = std::max(monkey.monkey_business, monkey_business_2);
    }
  }

  Solution s = std::to_string(monkey_business_1 * monkey_business_2);
  return s;
}

}  // namespace

Solution day11(std::ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}