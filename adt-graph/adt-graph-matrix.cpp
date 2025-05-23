#include "adt-graph-matrix.hpp"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
const EdgeValue NO_EDGE = 0;
const EdgeValue DEFAULT_EDGE = 1;

ADTGraphMatrix::ADTGraphMatrix(): currentNumVertices(0) {}

int ADTGraphMatrix::getInternalIndex(VertexId v_id) const {
  auto it = vertexToIndexMap.find(v_id);
  if (it == vertexToIndexMap.end()) {
    throw std::out_of_range("Vertex with ID " + std::to_string(v_id) +
      " not found in getInternalIndex.");
  }
  return it -> second;
}

bool ADTGraphMatrix::isValidInternalIndex(int idx) const {
  return idx >= 0 && static_cast<size_t>(idx) < currentNumVertices;
}

size_t ADTGraphMatrix::getNumVertices() const {
  return currentNumVertices;
}

bool ADTGraphMatrix::vertexExists(VertexId x) const {
  return vertexToIndexMap.count(x) > 0;
}

// 1. adjacent(G, x, y)
bool ADTGraphMatrix::adjacent(VertexId x, VertexId y) const {
  if (!vertexExists(x) || !vertexExists(y)) {
    return false;
  }

  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);
  return adjMatrix[idx_x][idx_y] != NO_EDGE;
}

// 2. neighbours(G, x)
std::vector<VertexId> ADTGraphMatrix::neighbours(VertexId x) const {
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
void ADTGraphMatrix::addVertex(VertexId x, VertexValue val) {
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
void ADTGraphMatrix::removeVertex(VertexId x) {
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
void ADTGraphMatrix::addEdge(VertexId x, VertexId y, EdgeValue val) {
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
void ADTGraphMatrix::removeEdge(VertexId x, VertexId y) {
  if (!vertexExists(x) || !vertexExists(y)) {
    return;
  }
  
  int idx_x = getInternalIndex(x);
  int idx_y = getInternalIndex(y);
  adjMatrix[idx_x][idx_y] = NO_EDGE;
  adjMatrix[idx_y][idx_x] = NO_EDGE;
}

// 7. getVertexValue(G, x)
std::optional<VertexValue> ADTGraphMatrix::getVertexValue(VertexId x) const {
  if (!vertexExists(x)) {
    return std::nullopt;
  }

  int idx_x = getInternalIndex(x);
  return vertexValuesStore[idx_x];
}

// 8. setVertexValue(G, x, v)
void ADTGraphMatrix::setVertexValue(VertexId x, VertexValue v) {
  if (!vertexExists(x)) {
    return;
  }

  int idx_x = getInternalIndex(x);
  vertexValuesStore[idx_x] = v;
}

// 9. getEdgeValue(G, x, y)
std::optional<EdgeValue> ADTGraphMatrix::getEdgeValue(VertexId x, VertexId y) const {
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
void ADTGraphMatrix::setEdgeValue(VertexId x, VertexId y, EdgeValue v) {
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

void ADTGraphMatrix::printMatrix() const {
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

void ADTGraphMatrix::exportToDotFile(const std::string& filename) const {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename
                  << " for writing." << std::endl;
        return;
    }

    outfile << "graph G {\n";
    outfile << "  // Node definitions\n";
    for (size_t i = 0; i < currentNumVertices; ++i) {
        VertexId v_id = indexToVertexMap[i];
        VertexValue v_val = vertexValuesStore[i];
        outfile << "  " << v_id;
        outfile << " [label=\"" << v_id << " (val:" << v_val << ")\"]";
        outfile << ";\n";
    }

    outfile << "\n  // Edge definitions\n";
    for (size_t i = 0; i < currentNumVertices; ++i) {
        for (size_t j = i + 1; j < currentNumVertices; ++j) {
            if (adjMatrix[i][j] != NO_EDGE) {
                VertexId v1_id = indexToVertexMap[i];
                VertexId v2_id = indexToVertexMap[j];
                EdgeValue e_val = adjMatrix[i][j];
                outfile << "  " << v1_id << " -- " << v2_id;
                outfile << " [label=\"" << e_val << "\"]";
                outfile << ";\n";
            }
        }
    }

    for (size_t i = 0; i < currentNumVertices; ++i) {
        if (adjMatrix[i][i] != NO_EDGE) {
            VertexId v_id = indexToVertexMap[i];
            EdgeValue e_val = adjMatrix[i][i];
            outfile << "  " << v_id << " -- " << v_id;
            outfile << " [label=\"" << e_val << "\"]";
            outfile << ";\n";
        }
    }

    outfile << "}\n";
    outfile.close();
}