#include <cassert>
#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "y22.h"

namespace {

using namespace std;

struct LinkedList {
  int64_t value;
  LinkedList* next;
  LinkedList* previous;

  LinkedList* push_back(int64_t v)
  {
    assert(next == nullptr);
    LinkedList* element = new LinkedList();
    element->value = v;
    this->next = element;
    element->previous = this;
    return element;
  }

  void remove()
  {
    previous->next = next;
    next->previous = previous;
    previous = nullptr;
    next = nullptr;
  }

  void insert_after(LinkedList* element)
  {
    element->next = next;
    element->previous = this;
    next->previous = element;
    next = element;
  }

  void insert_before(LinkedList* element)
  {
    element->next = this;
    element->previous = previous;
    previous->next = element;
    previous = element;
  }

  int64_t find_index_by_value(int64_t v)
  {
    int64_t index = 0;
    LinkedList* element = this;
    while (element->value != v) {
      element = element->next;
      index++;
    }
    return index;
  }

  LinkedList* find_element_by_value(int64_t v)
  {
    LinkedList* element = this;
    while (element->value != v) {
      element = element->next;
    }
    return element;
  }

  LinkedList* operator[](int64_t index)
  {
    LinkedList* element = this;
    while (index > 0) {
      element = element->next;
      --index;
    }
    while (index < 0) {
      element = element->previous;
      ++index;
    }
    return element;
  }
};

int64_t wrap(int64_t i, int64_t i_max) { return ((i % i_max) + i_max) % i_max; }

void mix(const vector<int64_t>& original_order, LinkedList** head)
{
  for (int64_t i = 0; i < original_order.size(); ++i) {
    int64_t jump = original_order[i];
    LinkedList* current = (*head)->find_element_by_value(i);

    if (jump > 0) {
      jump = wrap(jump, original_order.size() - 1);
      LinkedList* insert_after = current->previous;
      if (current == *head) {
        *head = current->next;
      }
      current->remove();
      insert_after = (*insert_after)[jump];
      insert_after->insert_after(current);
    } else if (jump < 0) {
      jump = wrap(jump, original_order.size() - 1);
      LinkedList* insert_before = current->next;
      if (current == *head) {
        *head = current->next;
      }
      current->remove();
      insert_before = (*insert_before)[jump];
      insert_before->insert_before(current);
    }
  }
}

int64_t parse(ifstream& in, vector<int64_t>& original_order, LinkedList** head,
           int64_t key)
{
  int64_t i;
  int64_t index = 0;
  (*head)->value = index;

  in >> i;
  LinkedList* tail = *head;
  original_order.push_back(i * key);
  while (in >> i) {
    index++;
    tail = tail->push_back(index);
    original_order.push_back(i * key);
  }
  tail->next = *head;
  (*head)->previous = tail;

  return index + 1;
}

inline string solve(std::ifstream& in, int64_t key, int32_t num_mixes)
{
  vector<int64_t> original_order;
  LinkedList* head = new LinkedList();
  int64_t length = parse(in, original_order, &head, key);

  for (int32_t i = 0; i < num_mixes; ++i) {
    mix(original_order, &head);
  }

  LinkedList* tmp = head;
  int64_t zero_idx = 0;
  while (original_order[tmp->value] != 0) {
    tmp = tmp->next;
    zero_idx++;
  }

  int64_t value = original_order[(*head)[zero_idx + 1000 % length]->value] +
                  original_order[(*head)[zero_idx + 2000 % length]->value] +
                  original_order[(*head)[zero_idx + 3000 % length]->value];
  
  LinkedList* current = head;
  for (size_t i = 0; i < length; ++i) {
    LinkedList* next = current->next;
    delete current;
    current = next;
  }

  return to_string(value);
}

}  // namespace

string y22day20(ifstream& in, int8_t part)
{
  if (part == 1) {
    return solve(in, 1, 1);
  } else {
    return solve(in, 811'589'153, 10);
  }
}