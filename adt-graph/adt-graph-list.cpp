#include "adt-graph-list.hpp"

#include <iostream>

#include <fstream>

ADTGraphList::ADTGraphList() {}

size_t ADTGraphList::getNumVertices() const {
  return vertexValuesStore.size();
}

bool ADTGraphList::vertexExists(VertexId x) const {
  return vertexValuesStore.count(x) > 0;
}

// 1. adjacent(G, x, y)
bool ADTGraphList::adjacent(VertexId x, VertexId y) const {
  if (!vertexExists(x) || !vertexExists(y)) {
    return false;
  }
  auto it_x = adjList.find(x);
  if (it_x != adjList.end()) {
    for (const auto & edge: it_x -> second) {
      if (edge.first == y) {
        return true;
      }
    }
  }
  return false;
}

// 2. neighbours(G, x)
std::vector < VertexId > ADTGraphList::neighbours(VertexId x) const {
  std::vector < VertexId > result;
  if (!vertexExists(x)) {
    return result;
  }
  auto it_x = adjList.find(x);
  if (it_x != adjList.end()) {
    for (const auto & edge: it_x -> second) {
      result.push_back(edge.first);
    }
  }
  return result;
}

// 3. addVertex(G, x, val)
void ADTGraphList::addVertex(VertexId x, VertexValue val) {
  if (vertexExists(x)) {
    return;
  }
  vertexValuesStore[x] = val;
  adjList[x] = std::list<std::pair<VertexId, EdgeValue>>();
}

// 4. removeVertex(G, x)
void ADTGraphList::removeVertex(VertexId x) {
  if (!vertexExists(x)) {
    return;
  }
  vertexValuesStore.erase(x);
  adjList.erase(x);

  for (auto & pair_vertex_list: adjList) {
    pair_vertex_list.second.remove_if(
      [x](const std::pair < VertexId, EdgeValue > & edge) {
        return edge.first == x;
      });
  }
}

// 5. addEdge(G, x, y, val)
void ADTGraphList::addEdge(VertexId x, VertexId y, EdgeValue val) {
  if (!vertexExists(x) || !vertexExists(y)) {
    return;
  }
  if (val == NO_EDGE && val != 0) {
    removeEdge(x, y);
    return;
  }

  removeEdge(x, y);

  adjList[x].push_back({
    y,
    val
  });
  if (x != y) {
    adjList[y].push_back({
      x,
      val
    });
  }
}

// 6. removeEdge(G, x, y)
void ADTGraphList::removeEdge(VertexId x, VertexId y) {
  if (!vertexExists(x) || !vertexExists(y)) {
    return;
  }
  if (adjList.count(x)) {
    adjList.at(x).remove_if(
      [y](const std::pair < VertexId, EdgeValue > & edge) {
        return edge.first == y;
      });
  }
  if (x != y && adjList.count(y)) {
    adjList.at(y).remove_if(
      [x](const std::pair < VertexId, EdgeValue > & edge) {
        return edge.first == x;
      });
  }
}

// 7. getVertexValue(G, x)
std::optional<VertexValue> ADTGraphList::getVertexValue(VertexId x) const {
  if (!vertexExists(x)) {
    return std::nullopt;
  }
  return vertexValuesStore.at(x);
}

// 8. setVertexValue(G, x, v)
void ADTGraphList::setVertexValue(VertexId x, VertexValue v) {
  if (!vertexExists(x)) {
    addVertex(x, v);
    return;
  }
  vertexValuesStore[x] = v;
}

// 9. getEdgeValue(G, x, y)
std::optional<EdgeValue> ADTGraphList::getEdgeValue(VertexId x, VertexId y) const {
  if (!vertexExists(x) || !vertexExists(y)) {
    return std::nullopt;
  }
  auto it_x = adjList.find(x);
  if (it_x != adjList.end()) {
    for (const auto & edge: it_x -> second) {
      if (edge.first == y) {
        return edge.second;
      }
    }
  }
  return std::nullopt;
}

// 10. setEdgeValue(G, x, y, v)
void ADTGraphList::setEdgeValue(VertexId x, VertexId y, EdgeValue v) {
  if (!vertexExists(x) || !vertexExists(y)) {
    return;
  }
  if (v == NO_EDGE && v != 0) {
    removeEdge(x, y);
    return;
  }

  bool found_x_to_y = false;
  if (adjList.count(x)) {
    for (auto & edge: adjList.at(x)) {
      if (edge.first == y) {
        edge.second = v;
        found_x_to_y = true;
        break;
      }
    }
  }
  if (!found_x_to_y) {
    adjList[x].push_back({
      y,
      v
    });
  }

  if (x == y) return;

  bool found_y_to_x = false;
  if (adjList.count(y)) {
    for (auto & edge: adjList.at(y)) {
      if (edge.first == x) {
        edge.second = v;
        found_y_to_x = true;
        break;
      }
    }
  }
  if (!found_y_to_x) {
    adjList[y].push_back({
      x,
      v
    });
  }
}

void ADTGraphList::printGraph() const {
  std::cout << "Graph (Adjacency List Representation):" << std::endl;
  std::cout << "Number of vertices: " << getNumVertices() << std::endl;
  for (const auto & vertex_pair: vertexValuesStore) {
    VertexId u = vertex_pair.first;
    VertexValue val = vertex_pair.second;
    std::cout << "Vertex " << u << " (Value: " << val << "): ";
    if (adjList.count(u)) {
      for (const auto & edge: adjList.at(u)) {
        std::cout << "-> (" << edge.first << ", val:" << edge.second << ") ";
      }
    }
    std::cout << std::endl;
  }
}

void ADTGraphList::exportToDotFile(const std::string & filename) const {
  std::ofstream outfile(filename);
  if (!outfile.is_open()) {
    std::cerr << "Error: Could not open file " << filename <<
      " for writing." << std::endl;
    return;
  }

  outfile << "graph G {\n";
  outfile << "  node [shape=circle];\n";

  for (const auto & v_pair: vertexValuesStore) {
    outfile << "  " << v_pair.first << " [label=\"" << v_pair.first <<
      "\\n(val:" << v_pair.second << ")\"];\n";
  }

  std::map < std::pair < VertexId, VertexId > , bool > added_edges;

  for (const auto & pair_vertex_list: adjList) {
    VertexId u = pair_vertex_list.first;
    for (const auto & edge: pair_vertex_list.second) {
      VertexId v = edge.first;
      EdgeValue weight = edge.second;

      std::pair < VertexId, VertexId > edge_key = (u < v) ? std::make_pair(u, v) : std::make_pair(v, u);

      if (added_edges.find(edge_key) == added_edges.end()) {
        outfile << "  " << u << " -- " << v;
        outfile << " [label=\"" << weight << "\"]";
        outfile << ";\n";
        added_edges[edge_key] = true;
      }
    }
  }
  outfile << "}\n";
  outfile.close();
}