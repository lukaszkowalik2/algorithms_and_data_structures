#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "set-hashed.h"
#include "utilities.h"

bool compareElementVectors(std::vector<std::string> v1, std::vector<std::string> v2) {
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return v1 == v2;
}

int main() {

  // Basic operations tests
  setHashed s1(5);
  s1.insert("Test");
  s1.insert("Data");
  s1.insert("AAAA");
  s1.insert("BBBB");
  testResult(s1.size() == 4, "Initial size after inserts");
  testResult(s1.contains("Test"), "Contains existing element");
  testResult(!s1.contains("ZZZZ"), "Does not contain non-existent element");

  // Duplicate insert test
  s1.insert("Test");
  testResult(s1.size() == 4, "Size unchanged after duplicate insert");

  // Remove tests
  s1.remove("Data");
  testResult(s1.size() == 3, "Size after remove");
  testResult(!s1.contains("Data"), "Element removed successfully");
  s1.remove("XXXX");
  testResult(s1.size() == 3, "Size unchanged after removing non-existent element");

  // Set operations tests
  setHashed s2(5);
  s2.insert("AAAA");
  s2.insert("CCCC");
  s2.insert("DDDD");

  // Union test
  setHashed sUnion = setHashed::setUnion(s1, s2);
  std::vector<std::string> expectedUnion = {"AAAA", "BBBB", "CCCC", "DDDD", "Test"};
  testResult(compareElementVectors(sUnion.getElements(), expectedUnion), "Union operation");
  testResult(sUnion.size() == 5, "Union size");

  // Intersection test
  setHashed sIntersection = setHashed::intersection(s1, s2);
  std::vector<std::string> expectedIntersection = {"AAAA"};
  testResult(compareElementVectors(sIntersection.getElements(), expectedIntersection), "Intersection operation");
  testResult(sIntersection.size() == 1, "Intersection size");

  // Difference tests
  setHashed sDiff1 = setHashed::difference(s1, s2);
  std::vector<std::string> expectedDiff1 = {"BBBB", "Test"};
  testResult(compareElementVectors(sDiff1.getElements(), expectedDiff1), "Difference s1-s2");
  testResult(sDiff1.size() == 2, "Difference s1-s2 size");

  setHashed sDiff2 = setHashed::difference(s2, s1);
  std::vector<std::string> expectedDiff2 = {"CCCC", "DDDD"};
  testResult(compareElementVectors(sDiff2.getElements(), expectedDiff2), "Difference s2-s1");
  testResult(sDiff2.size() == 2, "Difference s2-s1 size");

  // Equality tests
  setHashed s3(5);
  s3.insert("Test");
  s3.insert("AAAA");
  s3.insert("BBBB");
  testResult(setHashed::equals(s1, s3), "Equal sets with same table size");
  testResult(!setHashed::equals(s1, s2), "Unequal sets");

  // Different table sizes but same elements
  setHashed s4(10);
  s4.insert("Test");
  s4.insert("AAAA");
  s4.insert("BBBB");
  testResult(setHashed::equals(s1, s4), "Equal sets with different table sizes");

  // Empty set tests
  setHashed empty1(5), empty2(10);
  testResult(setHashed::equals(empty1, empty2), "Empty sets equality");
  testResult(empty1.size() == 0, "Empty set size");

  // Operations with empty set
  setHashed s5(5);
  s5.insert("AAAA");
  s5.insert("BBBB");
  setHashed unionEmpty = setHashed::setUnion(s5, empty1);
  testResult(compareElementVectors(unionEmpty.getElements(), s5.getElements()), "Union with empty set");

  setHashed intersectEmpty = setHashed::intersection(s5, empty1);
  testResult(intersectEmpty.size() == 0, "Intersection with empty set");

  setHashed diffEmpty = setHashed::difference(s5, empty1);
  testResult(compareElementVectors(diffEmpty.getElements(), s5.getElements()), "Difference with empty set");

  return 0;
}
