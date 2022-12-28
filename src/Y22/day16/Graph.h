#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include "utils/Matrix2D.h"

//class Node {
// public:
//  Node(std::string name) : name_(name){};
//  virtual ~Node() = default;
//
//  virtual bool operator<(const Node& rhs) const = 0;
//
//  virtual std::map<std::shared_ptr<Node>, int32_t>& neighbors() = 0;
//  virtual void print(std::ostream& out) const = 0;
//
//  const std::string name_;
//
//  friend std::ostream& operator<<(std::ostream& out, const Node& v)
//  {
//    v.print(out);
//    return out;
//  }
//};

class Graph {
 public:
  Graph() = default;
  virtual ~Graph() = default;

  // Reduce the graph using Floyd-Warshall
  void reduce();

  // Cull nodes with value of zero
  void removeNodes(const std::vector<std::string>& nodes);

  // Printing for debugging
  friend std::ostream& operator<<(std::ostream& out, const Graph& v);

  std::map<std::string, int32_t> nodes_;
  std::vector<std::int32_t> node_values_;
  Matrix2D<std::int32_t> edges_;

 //private:
 // std::vector<std::shared_ptr<Node>> nodes_;
};

#endif  // !_GRAPH_H_