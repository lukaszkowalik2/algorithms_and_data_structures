#ifndef ADT_GRAPH_LIST_HPP
#define ADT_GRAPH_LIST_HPP

#include <list>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

using VertexId = int;
using VertexValue = int;
using EdgeValue = int;

class ADTGraphList {
private:
    std::map<VertexId, std::list<std::pair<VertexId, EdgeValue>>> adjList;
    std::map<VertexId, VertexValue> vertexValuesStore;

public:
    static constexpr EdgeValue NO_EDGE_SENTINEL = 0;
    static constexpr EdgeValue DEFAULT_EDGE_VALUE = 1;

    ADTGraphList();

    size_t getNumVertices() const;
    bool vertexExists(VertexId x) const;

    bool adjacent(VertexId x, VertexId y) const;
    std::vector<VertexId> neighbours(VertexId x) const;
    void addVertex(VertexId x, VertexValue val = VertexValue{});
    void removeVertex(VertexId x);
    void addEdge(VertexId x, VertexId y, EdgeValue val = DEFAULT_EDGE_VALUE);
    void removeEdge(VertexId x, VertexId y);
    std::optional<VertexValue> getVertexValue(VertexId x) const;
    void setVertexValue(VertexId x, VertexValue v);
    std::optional<EdgeValue> getEdgeValue(VertexId x, VertexId y) const;
    void setEdgeValue(VertexId x, VertexId y, EdgeValue v);

    void printGraph() const;
    void exportToDotFile(const std::string& filename) const;
};

#endif // ADT_GRAPH_LIST_HPP
