#include "graph.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>

const EdgeValue NO_EDGE = 0;
const EdgeValue DEFAULT_EDGE = 1;

Graph::Graph(): currentNumVertices(0) {}

int Graph::getInternalIndex(VertexId v_id) const {
  auto it = vertexToIndexMap.find(v_id);
  if (it == vertexToIndexMap.end()) {
    throw std::out_of_range("Vertex with ID " + std::to_string(v_id) +
      " not found in getInternalIndex.");
  }
  return it -> second;
}

bool Graph::isValidInternalIndex(int idx) const {
  return idx >= 0 && static_cast<size_t>(idx) < currentNumVertices;
}

size_t Graph::getNumVertices() const {
  return currentNumVertices;
}

bool Graph::vertexExists(VertexId x) const {
  return vertexToIndexMap.count(x) > 0;
}

// 1. adjacent(G, x, y)
bool Graph::adjacent(VertexId x, VertexId y) const {
  if (!vertexExists(x) || !vertexExists(y)) {
    return false;
  }

  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);
  return adjMatrix[idx_x][idx_y] != NO_EDGE;
}

// 2. neighbours(G, x)
std::vector<VertexId> Graph::neighbours(VertexId x) const {
  std::vector<VertexId> result;
  if (!vertexExists(x)) {
    return result;
  }

  int idx_x = getInternalIndex(x);
  for (size_t i = 0; i < currentNumVertices; ++i) {
    if (adjMatrix[idx_x][i] != NO_EDGE) {
      result.push_back(indexToVertexMap[i]);
    }
  }
  return result;
}

// 3. addVertex(G, x, val)
void Graph::addVertex(VertexId x, VertexValue val) {
  if (vertexExists(x)) {
    return;
  }

  vertexToIndexMap[x] = static_cast<int>(currentNumVertices);
  indexToVertexMap.push_back(x);
  vertexValuesStore.push_back(val);

  for (size_t i = 0; i < currentNumVertices; ++i) {
    adjMatrix[i].push_back(NO_EDGE);
  }
  currentNumVertices++;
  adjMatrix.push_back(std::vector<EdgeValue>(currentNumVertices, NO_EDGE));
}

// 4. removeVertex(G, x)
void Graph::removeVertex(VertexId x) {
  if (!vertexExists(x)) {
    return;
  }

  int idx_to_remove = getInternalIndex(x);

  adjMatrix.erase(adjMatrix.begin() + idx_to_remove);

  for (auto &row : adjMatrix) {
    if (static_cast<size_t>(idx_to_remove) < row.size()) {
      row.erase(row.begin() + idx_to_remove);
    }
  }

  vertexValuesStore.erase(vertexValuesStore.begin() + idx_to_remove);

  indexToVertexMap.erase(indexToVertexMap.begin() + idx_to_remove);

  vertexToIndexMap.erase(x);

  currentNumVertices--;

  for (size_t i = 0; i < currentNumVertices; ++i) {
    vertexToIndexMap[indexToVertexMap[i]] = static_cast<int>(i);
  }
}

// 5. addEdge(G, x, y, val)
void Graph::addEdge(VertexId x, VertexId y, EdgeValue val) {
  if (!vertexExists(x) || !vertexExists(y)) {
    throw std::out_of_range(
      "One or both vertices not found for addEdge.");
  }
  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);

  adjMatrix[idx_x][idx_y] = val;
  adjMatrix[idx_y][idx_x] = val;
}

// 6. removeEdge(G, x, y)
void Graph::removeEdge(VertexId x, VertexId y) {
  if (!vertexExists(x) || !vertexExists(y)) {
    return;
  }
  
  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);
  adjMatrix[idx_x][idx_y] = NO_EDGE;
  adjMatrix[idx_y][idx_x] = NO_EDGE;
}

// 7. getVertexValue(G, x)
std::optional<VertexValue> Graph::getVertexValue(VertexId x) const {
  if (!vertexExists(x)) {
    return std::nullopt;
  }

  int idx_x = getInternalIndex(x);
  return vertexValuesStore[idx_x];
}

// 8. setVertexValue(G, x, v)
void Graph::setVertexValue(VertexId x, VertexValue v) {
  if (!vertexExists(x)) {
    return;
  }

  int idx_x = getInternalIndex(x);
  vertexValuesStore[idx_x] = v;
}

// 9. getEdgeValue(G, x, y)
std::optional<EdgeValue> Graph::getEdgeValue(VertexId x, VertexId y) const {
  if (!vertexExists(x) || !vertexExists(y)) {
    return std::nullopt;
  }

  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);
  EdgeValue val = adjMatrix[idx_x][idx_y];
  if (val == NO_EDGE) {
    return std::nullopt;
  }
  return val;
}

// 10. setEdgeValue(G, x, y, v)
void Graph::setEdgeValue(VertexId x, VertexId y, EdgeValue v) {
  if (v == NO_EDGE) {
    removeEdge(x, y);
    return;
  }
  if (!vertexExists(x) || !vertexExists(y)) {
    return;
  }

  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);
  adjMatrix[idx_x][idx_y] = v;
  adjMatrix[idx_y][idx_x] = v;
}

void Graph::printMatrix() const {
  std::cout << "Adjacency Matrix (" << currentNumVertices << "x" <<
    currentNumVertices << "):\n";
  std::cout << "   |";
  for (size_t i = 0; i < currentNumVertices; ++i) {
    if (i < indexToVertexMap.size()) {
      std::cout << std::setw(3) << indexToVertexMap[i];
    } else {
      std::cout << std::setw(3) << "?";
    }
  }
  std::cout << "\n---|";
  for (size_t i = 0; i < currentNumVertices; ++i) std::cout << "---";
  std::cout << "\n";

  for (size_t i = 0; i < currentNumVertices; ++i) {
    if (i < indexToVertexMap.size()) {
      std::cout << std::setw(3) << indexToVertexMap[i] << "|";
    } else {
      std::cout << std::setw(3) << "?|";
    }
    for (size_t j = 0; j < currentNumVertices; ++j) {
      if (i < adjMatrix.size() && j < adjMatrix[i].size()) {
        std::cout << std::setw(3) << adjMatrix[i][j];
      } else {
        std::cout << std::setw(3) << "X";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "Vertex Values:\n";
  for (size_t i = 0; i < currentNumVertices; ++i) {
    if (i < indexToVertexMap.size() && i < vertexValuesStore.size()) {
      std::cout << "ID " << indexToVertexMap[i] << " (idx " << i << "): " << vertexValuesStore[i] << std::endl;
    }
  }
  std::cout << "Vertex to Index Map:\n";
  for (const auto &pair : vertexToIndexMap) {
    std::cout << "ID " << pair.first << " -> Index " << pair.second << std::endl;
  }
}