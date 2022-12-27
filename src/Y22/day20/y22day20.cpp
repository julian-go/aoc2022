#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "y22.h"

namespace {

using namespace std;

// struct Element {
//   int32_t i;
//   int32_t id;
//   list<Element>::iterator next;
// };

// struct Element {
//   int32_t value;
//   int32_t index;
// };

inline int32_t wrap_positive(int32_t value, const int32_t maximum)
{
  if (value <= 0) {
    while (value <= 0) {
      value += maximum - 1;
    }
  }
  if (value >= maximum) {
    while (value >= maximum) {
      value -= (maximum - 1);
    }
  }

  return value;
}

// inline Solution part1(std::ifstream& in)
//{
//   /*int32_t i;
//   int32_t id = 0;
//   list<Element> l;
//   while (in >> i) {
//     cout << i << endl;
//     Element e;
//     e.id = id;
//     id++;
//     e.i = i;
//     if (!l.empty()) {
//       e.next = next(l.end(), -1);
//     }
//     l.push_back(e);
//   }
//   l.back().next = l.begin();
//
//   auto cur = l.begin();
//   Element end = l.back();
//   while (cur->id != end.id) {
//     const int32_t index = std::distance(l.begin(), cur);
//     int32_t offset = index + cur->i;
//     offset = wrap_positive(offset, l.size());
//     l.splice(next(l.begin(), offset), l, cur);
//     cur = cur->next;
//   }
//
//   for (auto x : l) {
//     cout << x.i << endl;
//   }*/
//
//   int32_t i;
//   int32_t id = 0;
//   vector<Element> vec;
//   while (in >> i) {
//     cout << i << endl;
//     Element e;
//     e.id = id;
//     e.value = i;
//     e.index = id;
//     id++;
//     vec.push_back(e);
//   }
//
//   bool done = true;
//   while ()
//
//   return std::to_string(0);
// }

inline string part1(std::ifstream& in)
{
  int32_t i;
  vector<shared_ptr<int32_t>> original_order;
  while (in >> i) {
    shared_ptr<int32_t> p = make_shared<int32_t>(i);
    original_order.push_back(p);
  }
  vector<shared_ptr<int32_t>> new_order = original_order;

  // for (auto i : new_order) {
  //   cout << *i << ", ";
  // }
  // cout << endl;

  for (auto p : original_order) {
    // cout << "Moving " << *p << endl;
    const int32_t distance = *p;
    if (distance == 0) {
      continue;
    }
    auto it = find(new_order.begin(), new_order.end(), p);
    const int32_t index = it - new_order.begin();
    const int32_t new_index = wrap_positive(index + distance, new_order.size());
    if (new_index > index) {
      // move everythin between here and new index one back
      auto tmp = *it;
      for (int i = new_index; i >= index; --i) {
        auto tmp2 = new_order[i];
        new_order[i] = tmp;
        tmp = tmp2;
      }
    } else if (new_index < index) {
      auto tmp = *it;
      for (int i = new_index; i <= index; ++i) {
        auto tmp2 = new_order[i];
        new_order[i] = tmp;
        tmp = tmp2;
      }
    }

    // for (auto i : new_order) {
    //   cout << *i << ", ";
    // }
    // cout << endl;
  }

  auto zero_it = find_if(new_order.begin(), new_order.end(),
                         [](auto& p) { return *p == 0; });
  int32_t zero_pos = zero_it - new_order.begin();
  // cout << *new_order[(zero_pos + 1000) % new_order.size()] << endl;
  // cout << *new_order[(zero_pos + 2000) % new_order.size()] << endl;
  // cout << *new_order[(zero_pos + 3000) % new_order.size()] << endl;

  return to_string(*new_order[(zero_pos + 1000) % new_order.size()] +
                   *new_order[(zero_pos + 2000) % new_order.size()] +
                   *new_order[(zero_pos + 3000) % new_order.size()]);
}

inline string part2(std::ifstream& in)
{
  int64_t i;
  int64_t key = 811589153;
  vector<shared_ptr<int64_t>> original_order;
  while (in >> i) {
    shared_ptr<int64_t> p = make_shared<int64_t>(i * key);
    original_order.push_back(p);
  }
  vector<shared_ptr<int64_t>> new_order = original_order;

  // for (auto i : new_order) {
  //   cout << *i << ", ";
  // }
  // cout << endl;

  for (int i = 0; i < 10; ++i) {
    for (auto p : original_order) {
      // cout << "Moving " << *p << endl;
      const int64_t distance = *p;
      if (distance == 0) {
        continue;
      }
      auto it = find(new_order.begin(), new_order.end(), p);
      const int64_t index = it - new_order.begin();
      const int64_t new_index =
          wrap_positive(index + distance, new_order.size());
      if (new_index > index) {
        // move everythin between here and new index one back
        auto tmp = *it;
        for (int i = new_index; i >= index; --i) {
          auto tmp2 = new_order[i];
          new_order[i] = tmp;
          tmp = tmp2;
        }
      } else if (new_index < index) {
        auto tmp = *it;
        for (int i = new_index; i <= index; ++i) {
          auto tmp2 = new_order[i];
          new_order[i] = tmp;
          tmp = tmp2;
        }
      }

      // for (auto i : new_order) {
      //   cout << *i << ", ";
      // }
      // cout << endl;
    }
  }

  // for (auto p : original_order) {
  //   *p /= key;
  // }

  auto zero_it = find_if(new_order.begin(), new_order.end(),
                         [](auto& p) { return *p == 0; });
  int32_t zero_pos = zero_it - new_order.begin();
  // cout << *new_order[(zero_pos + 1000) % new_order.size()] << endl;
  // cout << *new_order[(zero_pos + 2000) % new_order.size()] << endl;
  // cout << *new_order[(zero_pos + 3000) % new_order.size()] << endl;

  return "";
}

}  // namespace

string y22day20(ifstream& in, int8_t part)
{
  if (part == 1) {
    return part1(in);
  } else {
    return part2(in);
  }
}