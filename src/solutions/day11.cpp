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
using std::int64_t;
using std::int8_t;

struct Monkey {
  std::vector<int32_t> held_items;
  std::function<void(int32_t&, int32_t)> inspect;
  int32_t pass_friend = 0;
  int32_t fail_friend = 0;
  int64_t counter = 0;
};

struct Item {
  std::vector<int32_t> remainders;
  std::vector<int32_t> divisors;
};

template <bool scary>
void createMonkeys(std::ifstream& in, std::vector<Monkey>* monkeys,
                   std::vector<Item>* items)
{
  std::string tmp;
  std::vector<int32_t> divisors;
  std::vector<int32_t> item_values;
  int32_t current_item = 0;

  while (!in.eof()) {
    Monkey m;
    std::getline(in, tmp);  // Skip first line

    in.ignore(256, ':');  // Skip until list of items
    int32_t item;
    do {
      in >> item >> tmp;
      item_values.push_back(item);
      m.held_items.push_back(current_item);
      current_item++;
    } while (tmp == ",");

    in.ignore(256, '=');  // Skip until operation
    std::string arg1, operation, arg2;
    in >> arg1 >> operation >> arg2;
    if (scary) {
      // Scary operations track the remainder only
      if (operation == "*" && arg2 == "old") {
        m.inspect = [](int32_t& old, int32_t divisor) {
          old = ((old % divisor) * (old % divisor)) % divisor;
        };
      } else {
        int32_t operand = std::stoi(arg2);
        if (operation == "+") {
          m.inspect = [operand](int32_t& old, int32_t divisor) {
            old = ((old % divisor) + (operand % divisor)) % divisor;
          };
        } else if (operation == "*") {
          m.inspect = [operand](int32_t& old, int32_t divisor) {
            old = ((old % divisor) * (operand % divisor)) % divisor;
          };
        } else {
          std::cerr << "Error during parsing";
        }
      }
    } else {
      // Normal operations track the absolute value
      if (operation == "*" && arg2 == "old") {
        m.inspect = [](int32_t& old, int32_t divisor) { old *= old; };
      } else {
        int32_t operand = std::stoi(arg2);
        if (operation == "+") {
          m.inspect = [operand](int32_t& old, int32_t divisor) {
            old += operand;
          };
        } else if (operation == "*") {
          m.inspect = [operand](int32_t& old, int32_t divisor) {
            old *= operand;
          };
        } else {
          std::cerr << "Error during parsing";
        }
      }
    }

    in.ignore(256, ':');  // Skip until test
    int32_t divisor;
    in >> tmp >> tmp >> divisor;
    divisors.push_back(divisor);

    in.ignore(256, ':');  // Skip until pass friend
    in >> tmp >> tmp >> tmp >> m.pass_friend;

    in.ignore(256, ':');  // Skip until fail friend
    in >> tmp >> tmp >> tmp >> m.fail_friend;

    in.ignore(256, '\n');  // skip last line
    std::getline(in, tmp);

    monkeys->push_back(m);
  }

  for (auto& value : item_values) {
    Item item;
    for (auto& divisor : divisors) {
      item.remainders.push_back(value);
      item.divisors.push_back(divisor);
    }
    items->push_back(item);
  }
}

inline Solution computeMonkeyBusiness(const std::vector<Monkey>& monkeys)
{
  int64_t counter_1 = 0;
  int64_t counter_2 = 0;
  for (const Monkey& monkey : monkeys) {
    if (monkey.counter >= counter_1) {
      counter_2 = counter_1;
      counter_1 = monkey.counter;
    } else {
      counter_2 = std::max(monkey.counter, counter_2);
    }
  }

  return std::to_string(counter_1 * counter_2);
}

template <int32_t kNumRounds, int32_t kWorryDecay>
inline Solution solve(std::ifstream& in)
{
  // constexpr int32_t kNumRounds = 10'000;

  std::vector<Monkey> monkeys;
  std::vector<Item> items;
  createMonkeys<kWorryDecay == 1>(in, &monkeys, &items);
  for (int32_t i = 0; i < kNumRounds; ++i) {
    for (size_t monkey_i = 0; monkey_i < monkeys.size(); ++monkey_i) {
      Monkey& m = monkeys[monkey_i];

      // Loop over every held item (idx), look it up in the vector and apply the
      // division to every remainder
      auto item_i = m.held_items.begin();
      while (item_i != m.held_items.end()) {
        Item& item = items[*item_i];
        for (int32_t remain_i = 0; remain_i < item.remainders.size();
             ++remain_i) {
          int32_t& r = item.remainders[remain_i];
          int32_t& d = item.divisors[remain_i];
          m.inspect(r, d);
          r /= kWorryDecay;
        }
        m.counter++;

        // If own divisor is zero, pass to pass friend, else fail friend
        if (item.remainders[monkey_i] % item.divisors[monkey_i] == 0) {
          monkeys[m.pass_friend].held_items.push_back(*item_i);
        } else {
          monkeys[m.fail_friend].held_items.push_back(*item_i);
        }
        item_i = m.held_items.erase(item_i);
      }
    }
  }

  return computeMonkeyBusiness(monkeys);
}

}  // namespace

Solution day11(std::ifstream& in, int8_t part)
{
  if (part == 1) {
    return solve<20, 3>(in);
  } else {
    return solve<10'000, 1>(in);
  }
}