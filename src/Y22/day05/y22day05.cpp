#include <algorithm>
#include <deque>
#include <iostream>
#include <sstream>
#include <vector>

#include "y22.h"

namespace {
using namespace std;

// Warehouse-Stack is a deque, front is top, back is bottom
using Pile = std::deque<char>;
using Warehouse = std::vector<Pile>;

void print(Warehouse w)
{
  std::int32_t height =
      std::max_element(w.begin(), w.end(), [](auto& a, auto& b) {
        return a.size() < b.size();
      })->size();

  while (!w.front().empty()) {
    std::for_each(w.begin(), w.end(), [height](auto& s) {
      if (s.size() == height) {
        std::cout << " [" << s.front() << "]";
        s.pop_front();
      } else {
        std::cout << "    ";
      }
    });
    std::cout << std::endl;
    height--;
  }
}

void parse(std::ifstream& in, Warehouse& w)
{
  std::string line;
  while (getline(in, line)) {
    if (w.empty()) {
      // each stack (except the last) reserves a size of 4
      w.resize((line.size() + 1) / 4);
    }

    if (line.empty()) {
      break;
    }

    for (int32_t i = 0; i < w.size(); ++i) {
      char c = line[i * 4 + 1];
      if (c >= 'A' && c <= 'Z') {
        w[i].push_back(c);
      }
    }
  }
}

inline string part1(ifstream& in)
{
  Warehouse w;
  parse(in, w);

  string line;
  int32_t amount, from, to;
  while (getline(in, line)) {
    std::stringstream ss(line);
    ss.ignore(5);
    ss >> amount;
    ss.ignore(6);
    ss >> from;
    ss.ignore(4);
    ss >> to;

    Pile& toPile = w[to - 1];
    Pile& fromPile = w[from - 1];

    std::copy(fromPile.begin(), fromPile.begin() + amount,
              std::front_inserter(toPile));
    fromPile.erase(fromPile.begin(), fromPile.begin() + amount);
  }

  string s = "";
  for (const Pile& pile : w) {
    s.push_back(pile.front());
  }
  return s;
}

inline string part2(ifstream& in)
{
  Warehouse w;
  parse(in, w);

  std::string line;
  std::int32_t amount, from, to;
  while (getline(in, line)) {
    std::stringstream ss(line);
    ss.ignore(5);
    ss >> amount;
    ss.ignore(6);
    ss >> from;
    ss.ignore(4);
    ss >> to;

    Pile& toPile = w[to - 1];
    Pile& fromPile = w[from - 1];

    std::copy(fromPile.rend() - amount, fromPile.rend(),
              std::front_inserter(toPile));
    fromPile.erase(fromPile.begin(), fromPile.begin() + amount);
  }

  string s = "";
  for (const Pile& pile : w) {
    s.push_back(pile.front());
  }
  return s;
}

}  // namespace

string y22day05(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}