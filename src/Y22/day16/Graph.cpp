#include "Graph.h"

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

void Graph::removeNodes(const std::vector<std::string>& nodes)
{
  // erase from values
  std::vector<int32_t> new_values;
  for (auto node : nodes_) {
    if (std::find(nodes.begin(), nodes.end(), node.first) == nodes.end()) {
        new_values.push_back(node_values_[node.second]);
    }
  }
  node_values_ = new_values;

  // erase from node list
  std::vector<int32_t> removed_indices;
  std::map<std::string, int32_t> new_nodes_;
  int32_t new_index = 0;
  for (auto& node : nodes_) {
    if (std::find(nodes.begin(), nodes.end(), node.first) == nodes.end()) {
      new_nodes_[node.first] = new_index;
      new_index++;
    } else {
      removed_indices.push_back(nodes_[node.first]);
    }
  }
  nodes_ = new_nodes_;

  // erase from edges
  Matrix2D<std::int32_t> new_edges_;
  new_edges_.resize(edges_.sizeX() - nodes.size(),
                    edges_.sizeY() - nodes.size());
  for (size_t y = 0, y_old = 0; y < new_edges_.sizeY(); ++y, ++y_old) {
    while (std::find(removed_indices.begin(), removed_indices.end(), y_old) !=
           removed_indices.end()) {
      ++y_old;
    }
    for (size_t x = 0, x_old = 0; x < new_edges_.sizeX(); ++x, ++x_old) {
      while (std::find(removed_indices.begin(), removed_indices.end(), x_old) !=
             removed_indices.end()) {
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
