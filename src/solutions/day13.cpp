#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <variant>
#include <vector>

#include "solutions.h"

namespace {

struct ValuePacket;
struct ListPacket;
struct Packet;
std::ostream& operator<<(std::ostream& out, const Packet& packet);

struct Packet {
  virtual ~Packet() = default;
  virtual inline std::strong_ordering operator<=>(const Packet& rhs) const
  {
    return compare(rhs);
  }
  virtual bool operator==(const Packet& rhs) const
  {
    return !(*this < rhs) && !(*this > rhs);
  }
  virtual std::ostream& serialize(std::ostream& out) const = 0;
  virtual inline std::strong_ordering compare(const Packet& rhs) const = 0;
};

std::ostream& operator<<(std::ostream& out, const Packet& packet)
{
  return packet.serialize(out);
}

struct ListPacket : Packet {
  virtual ~ListPacket() = default;
  virtual std::ostream& serialize(std::ostream& out) const
  {
    out << "[";
    for (const auto& p : data) {
      out << *p;
      if (p != data.back()) {
        out << ",";
      }
    }
    out << "]";
    return out;
  }
  virtual inline std::strong_ordering compare(const Packet& rhs) const
  {
    if (const ListPacket* rhs_list = dynamic_cast<const ListPacket*>(&rhs)) {
      auto lhs_it = data.begin();
      auto rhs_it = rhs_list->data.begin();
      while (true) {
        if (lhs_it == data.end() && rhs_it == rhs_list->data.end()) {
          return std::strong_ordering::equal;
        } else if (lhs_it == data.end()) {
          return std::strong_ordering::less;
        } else if (rhs_it == rhs_list->data.end()) {
          return std::strong_ordering::greater;
        }

        if (**lhs_it < **rhs_it) {
          return std::strong_ordering::less;
        } else if (**lhs_it > **rhs_it) {
          return std::strong_ordering::greater;
        }

        lhs_it++;
        rhs_it++;
      }
    } else {
      if (data.size() == 0) {
        return std::strong_ordering::less;
      } else if (data.size() > 1 && *data.front() == rhs) {
        return std::strong_ordering::greater;
      } else {
        return *data.front() <=> rhs;
      }
    }
  }
  std::vector<std::unique_ptr<Packet>> data;
};

struct ValuePacket : Packet {
  virtual ~ValuePacket() = default;
  ValuePacket(int32_t val) : data(val) {}
  virtual std::ostream& serialize(std::ostream& out) const
  {
    out << data;
    return out;
  }
  virtual inline std::strong_ordering compare(const Packet& rhs) const
  {
    if (const ValuePacket* rhs_value = dynamic_cast<const ValuePacket*>(&rhs)) {
      return data <=> rhs_value->data;
    } else if (const ListPacket* rhs_list =
                   dynamic_cast<const ListPacket*>(&rhs)) {
      if (rhs_list->data.size() == 0) {
        return std::strong_ordering::greater;
      } else {
        const auto& fval = *(rhs_list->data.front());
        if (rhs_list->data.size() == 1) {
          return *this <=> fval;
        } else {
          if (*this <= fval) {
            return std::strong_ordering::less;
          } else {
            return std::strong_ordering::greater;
          }
        }
      }
    }
  }
  int32_t data;
};

std::unique_ptr<Packet> toPacket(std::string s)
{
  char tmp;
  int32_t value;

  std::stringstream ss(s);
  ss >> tmp;  // skip first opening brace
  std::stack<std::unique_ptr<ListPacket>> packets;
  std::unique_ptr<ListPacket> p = std::make_unique<ListPacket>();
  while (!ss.eof()) {
    if (isdigit(ss.peek())) {
      ss >> value;
      p->data.push_back(std::make_unique<ValuePacket>(value));
    } else {
      ss >> tmp;
      if (tmp == ',') {
        // do nothing
      } else if (tmp == '[') {
        packets.push(std::move(p));
        p = std::make_unique<ListPacket>();
      } else if (tmp == ']' && !packets.empty()) {
        std::unique_ptr<ListPacket> top = std::move(packets.top());
        packets.pop();
        top->data.push_back(std::move(p));
        p = std::move(top);
      }
    }
  }
  return p;
}

inline Solution part1(std::ifstream& in)
{
  std::string signal1, signal2, tmp;
  int32_t idx_sum = 0;
  int32_t idx = 1;
  while (std::getline(in, signal1) && std::getline(in, signal2)) {
    std::unique_ptr<Packet> p1 = toPacket(signal1);
    std::unique_ptr<Packet> p2 = toPacket(signal2);
    if (*p1 < *p2) {
      idx_sum += idx;
    }
    std::getline(in, tmp);  // skip empty line
    idx++;
  }

  return std::to_string(idx_sum);
}

inline Solution part2(std::ifstream& in)
{
  std::string signal1, signal2, tmp;
  std::vector<std::unique_ptr<Packet>> packets;
  while (std::getline(in, signal1) && std::getline(in, signal2)) {
    packets.push_back(toPacket(signal1));
    packets.push_back(toPacket(signal2));
    std::getline(in, tmp);  // skip empty line
  }

  std::unique_ptr<Packet> divider1 = toPacket("[[2]]");
  std::unique_ptr<Packet> divider2 = toPacket("[[6]]");

  packets.push_back(toPacket("[[2]]"));
  packets.push_back(toPacket("[[6]]"));

  std::sort(packets.begin(), packets.end(),
            [](const std::unique_ptr<Packet>& a,
               const std::unique_ptr<Packet>& b) { return (*a) < (*b); });
  auto it1 = std::find_if(
      packets.begin(), packets.end(),
      [&](const std::unique_ptr<Packet>& a) { return (*a) == *divider1; });

  int index1 = it1 - packets.begin();
  index1++;

  auto it2 = std::find_if(
      packets.begin(), packets.end(),
      [&](const std::unique_ptr<Packet>& a) { return (*a) == *divider2; });

  int index2 = it2 - packets.begin();
  index2++;

  return std::to_string(index1 * index2);
}

}  // namespace

Solution day13(std::ifstream& in, std::int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}