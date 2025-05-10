#include "graph.hpp"

#include <iostream>
#include <cassert>
#include <algorithm>

void printTestResult(const std::string & testName, bool passed) {
  std::cout << testName << ": " << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testAddVertex() {
  Graph g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);

  bool passed = g.getNumVertices() == 2 &&
    g.vertexExists(1) &&
    g.vertexExists(2) &&
    g.getVertexValue(1).value() == 10 &&
    g.getVertexValue(2).value() == 20;

  printTestResult("Add Vertex Test", passed);
}

void testAddEdge() {
  Graph g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addEdge(1, 2, 5);

  bool passed = g.adjacent(1, 2) &&
    g.adjacent(2, 1) &&
    g.getEdgeValue(1, 2).value() == 5 &&
    g.getEdgeValue(2, 1).value() == 5;

  printTestResult("Add Edge Test", passed);
}

void testRemoveEdge() {
  Graph g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addEdge(1, 2, 5);
  g.removeEdge(1, 2);

  bool passed = !g.adjacent(1, 2) &&
    !g.adjacent(2, 1) &&
    !g.getEdgeValue(1, 2).has_value() &&
    !g.getEdgeValue(2, 1).has_value();

  printTestResult("Remove Edge Test", passed);
}

void testRemoveVertex() {
  Graph g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addEdge(1, 2, 5);
  g.removeVertex(1);

  bool passed = g.getNumVertices() == 1 &&
    !g.vertexExists(1) &&
    g.vertexExists(2) &&
    !g.adjacent(1, 2) &&
    !g.adjacent(2, 1);

  printTestResult("Remove Vertex Test", passed);
}

void testNeighbours() {
  Graph g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addVertex(3, 30);
  g.addEdge(1, 2, 1);
  g.addEdge(1, 3, 1);

  auto neighbours = g.neighbours(1);
  bool passed = neighbours.size() == 2 &&
    std::find(neighbours.begin(), neighbours.end(), 2) != neighbours.end() &&
    std::find(neighbours.begin(), neighbours.end(), 3) != neighbours.end();

  printTestResult("Neighbours Test", passed);
}

void testSetVertexValue() {
  Graph g;
  g.addVertex(1, 10);
  g.setVertexValue(1, 20);

  bool passed = g.getVertexValue(1).value() == 20;
  printTestResult("Set Vertex Value Test", passed);
}

void testSetEdgeValue() {
  Graph g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addEdge(1, 2, 1);
  g.setEdgeValue(1, 2, 10);

  bool passed = g.getEdgeValue(1, 2).value() == 10 &&
    g.getEdgeValue(2, 1).value() == 10;

  printTestResult("Set Edge Value Test", passed);
}

int main() {
  std::cout << "Running Graph Tests...\n" << std::endl;

  testAddVertex();
  testAddEdge();
  testRemoveEdge();
  testRemoveVertex();
  testNeighbours();
  testSetVertexValue();
  testSetEdgeValue();

  std::cout << "\nAll tests completed!" << std::endl;
  return 0;
}