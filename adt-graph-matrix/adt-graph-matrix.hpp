// graph.hpp
#ifndef ADT_GRAPH_MATRIX_HPP
#define ADT_GRAPH_MATRIX_HPP

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

using VertexId = int;
using VertexValue = int;
using EdgeValue = int;

class ADTGraphMatrix {
private:
    std::vector<std::vector<EdgeValue>> adjMatrix;
    std::map<VertexId, int> vertexToIndexMap;
    std::vector<VertexId> indexToVertexMap;
    std::vector<VertexValue> vertexValuesStore;
    size_t currentNumVertices;

    int getInternalIndex(VertexId v_id) const;
    bool isValidInternalIndex(int idx) const;

public:
    static constexpr EdgeValue NO_EDGE = 0;
    static constexpr EdgeValue DEFAULT_EDGE = 1;

    ADTGraphMatrix();
    static ADTGraphMatrix make();

    size_t getNumVertices() const;
    bool vertexExists(VertexId x) const;

    bool adjacent(VertexId x, VertexId y) const;
    std::vector<VertexId> neighbours(VertexId x) const;
    void addVertex(VertexId x, VertexValue val = VertexValue{});
    void removeVertex(VertexId x);
    void addEdge(VertexId x, VertexId y, EdgeValue val = DEFAULT_EDGE);
    void removeEdge(VertexId x, VertexId y);
    std::optional<VertexValue> getVertexValue(VertexId x) const;
    void setVertexValue(VertexId x, VertexValue v);
    std::optional<EdgeValue> getEdgeValue(VertexId x, VertexId y) const;
    void setEdgeValue(VertexId x, VertexId y, EdgeValue v);

    void printMatrix() const;
    void exportToDotFile(const std::string& filename) const;
};

#endif // ADT_GRAPH_MATRIX_HPP
