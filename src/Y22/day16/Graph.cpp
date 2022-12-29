#include "Graph.h"

#include <algorithm>

void Graph::reduce()
{
  const size_t N = nodes_.size();
  for (size_t k = 0; k < N; ++k) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        if (edges_.at(i, j) > edges_.at(i, k) + edges_.at(k, j)) {
          edges_.at(i, j) = edges_.at(i, k) + edges_.at(k, j);
        }
      }
    }
  }
}

void Graph::removeNodes(const std::vector<std::string>& rm_nodes)
{
  auto contains = [](auto vec, auto value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
  };

  // erase from node list
  std::vector<int32_t> remove_indices;
  for (auto& node : nodes_) {
    if (contains(rm_nodes, node.first)) {
      remove_indices.push_back(nodes_[node.first]);
    }
  }
  std::sort(remove_indices.begin(), remove_indices.end());
  std::map<std::string, int32_t> new_nodes_;
  for (auto& node : nodes_) {
    if (contains(rm_nodes, node.first)) {
      continue;
    } else {
      int32_t i = 0;
      while (i < remove_indices.size() && remove_indices[i] < node.second) {
        ++i;
      }
      new_nodes_[node.first] = node.second - i;
    }
  }
  // erase from values
  std::vector<int32_t> new_values(node_values_.size() - rm_nodes.size(), 0);
  for (auto node : new_nodes_) {
    if (!contains(rm_nodes, node.first)) {
      new_values[node.second] = node_values_[nodes_[node.first]];
    }
  }
  nodes_ = new_nodes_;
  node_values_ = new_values;

  // erase from edges
  Matrix2D<std::int32_t> new_edges_;
  new_edges_.resize(edges_.sizeX() - rm_nodes.size(),
                    edges_.sizeY() - rm_nodes.size());
  for (size_t y = 0, y_old = 0; y < new_edges_.sizeY(); ++y, ++y_old) {
    while (contains(remove_indices, y_old)) {
      ++y_old;
    }
    for (size_t x = 0, x_old = 0; x < new_edges_.sizeX(); ++x, ++x_old) {
      while (contains(remove_indices, x_old)) {
        ++x_old;
      }
      new_edges_.at(x, y) = edges_.at(x_old, y_old);
    }
  }
  edges_ = new_edges_;
}

std::ostream& operator<<(std::ostream& out, const Graph& v)
{
  // out << "Graph with " << v.nodes_.size() << " Nodes:";
  // for (const auto& node : v.nodes_) {
  //   out << "\n\t" << *node;
  // }
  return out;
}
