#include "adt-graph-list.hpp"

#include <iostream>
#include <cassert>
#include <algorithm>
#include <cstdlib>

void printTestResult(const std::string & testName, bool passed) {
  std::cout << testName << ": " << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testAddVertex() {
  ADTGraphList g;
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
  ADTGraphList g;
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
  ADTGraphList g;
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
  ADTGraphList g;
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
  ADTGraphList g;
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
  ADTGraphList g;
  g.addVertex(1, 10);
  g.setVertexValue(1, 20);

  bool passed = g.getVertexValue(1).value() == 20;
  printTestResult("Set Vertex Value Test", passed);
}

void testSetEdgeValue() {
  ADTGraphList g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addEdge(1, 2, 1);
  g.setEdgeValue(1, 2, 10);

  bool passed = g.getEdgeValue(1, 2).value() == 10 &&
    g.getEdgeValue(2, 1).value() == 10;

  printTestResult("Set Edge Value Test", passed);
}

void testSelfLoop() {
  ADTGraphList g;
  g.addVertex(1, 10);
  g.addEdge(1, 1, 5);

  bool passed = g.adjacent(1, 1) &&
    g.getEdgeValue(1, 1).value() == 5;

  printTestResult("Self Loop Test", passed);
}

void testEmptyGraph() {
  ADTGraphList g;
  bool passed = g.getNumVertices() == 0;
  printTestResult("Empty Graph Test", passed);
}

void testMultipleVertexRemoval() {
  ADTGraphList g;
  g.addVertex(1, 10);
  g.addVertex(2, 20);
  g.addVertex(3, 30);
  g.addEdge(1, 2, 5);
  g.addEdge(2, 3, 6);
  g.removeVertex(2);

  bool passed = g.getNumVertices() == 2 &&
    g.vertexExists(1) &&
    g.vertexExists(3) &&
    !g.adjacent(1, 3);

  printTestResult("Multiple Vertex Removal Test", passed);
}

void generateExampleGraphs() {
  std::system("mkdir -p build charts");

  ADTGraphList promptK3;
  promptK3.addVertex(10, 101);
  promptK3.addVertex(20, 102);
  promptK3.addVertex(30, 103);
  promptK3.addEdge(10, 20, 5);
  promptK3.addEdge(10, 30, 6);
  promptK3.addEdge(20, 30, 7);
  promptK3.exportToDotFile("build/prompt_k3_graph.dot");
  
  std::system("dot -Tpng build/prompt_k3_graph.dot -o charts/list_graph_prompt_k3.png");

  ADTGraphList cycleC4;
  cycleC4.addVertex(1, 11);
  cycleC4.addVertex(2, 12);
  cycleC4.addVertex(3, 13);
  cycleC4.addVertex(4, 14);
  cycleC4.addEdge(1, 2, 101);
  cycleC4.addEdge(2, 3, 102);
  cycleC4.addEdge(3, 4, 103);
  cycleC4.addEdge(4, 1, 104);
  cycleC4.exportToDotFile("build/cycle_c4_graph.dot");
  
  std::system("dot -Tpng build/cycle_c4_graph.dot -o charts/list_graph_cycle_c4.png");

  ADTGraphList disconnected;
  disconnected.addVertex(101, 201);
  disconnected.addVertex(102, 202);
  disconnected.addEdge(101, 102, 21);
  disconnected.addVertex(201, 203);
  disconnected.addVertex(202, 204);
  disconnected.addEdge(201, 202, 22);
  disconnected.addVertex(301, 205);
  disconnected.exportToDotFile("build/disconnected_graph.dot");
  
  std::system("dot -Tpng build/disconnected_graph.dot -o charts/list_graph_disconnected.png");

  ADTGraphList isolatedVertices;
  isolatedVertices.addVertex(50, 500);
  isolatedVertices.addVertex(60, 600);
  isolatedVertices.addVertex(70, 700);
  isolatedVertices.exportToDotFile("build/isolated_vertices_graph.dot");
  
  std::system("dot -Tpng build/isolated_vertices_graph.dot -o charts/list_graph_isolated_vertices.png");

  ADTGraphList star;
  star.addVertex(1, 10);
  star.addVertex(2, 20);
  star.addVertex(3, 30);
  star.addVertex(4, 40);
  star.addEdge(1, 2, 11);
  star.addEdge(1, 3, 12);
  star.addEdge(1, 4, 13);
  star.addEdge(1, 1, 19);
  star.exportToDotFile("build/star_graph_loop.dot");
  
  std::system("dot -Tpng build/star_graph_loop.dot -o charts/list_graph_star_loop.png");

  ADTGraphList pathP4;
  pathP4.addVertex(71, 701);
  pathP4.addVertex(72, 702);
  pathP4.addVertex(73, 703);
  pathP4.addVertex(74, 704);
  pathP4.addEdge(71, 72, 31);
  pathP4.addEdge(72, 73, 32);
  pathP4.addEdge(73, 74, 33);
  pathP4.exportToDotFile("build/path_p4_graph.dot");
  
  std::system("dot -Tpng build/path_p4_graph.dot -o charts/list_graph_path_p4.png");

  ADTGraphList completeK4;
  completeK4.addVertex(81, 801);
  completeK4.addVertex(82, 802);
  completeK4.addVertex(83, 803);
  completeK4.addVertex(84, 804);
  completeK4.addEdge(81, 82, 1);
  completeK4.addEdge(81, 83, 2);
  completeK4.addEdge(81, 84, 3);
  completeK4.addEdge(82, 83, 4);
  completeK4.addEdge(82, 84, 5);
  completeK4.addEdge(83, 84, 6);
  completeK4.exportToDotFile("build/complete_k4_graph.dot");
  
  std::system("dot -Tpng build/complete_k4_graph.dot -o charts/list_graph_complete_k4.png");
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
  testSelfLoop();
  testEmptyGraph();
  testMultipleVertexRemoval();

  std::cout << "\nGenerating example graph visualizations..." << std::endl;
  generateExampleGraphs();

  std::cout << "\nAll tests completed!" << std::endl;
  return 0;
}