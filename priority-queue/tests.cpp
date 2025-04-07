#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "priority-queue-binary.h"
#include "priority-queue.h"

void testPqSetBasic() {
  std::cout << "Testing priorityQueue basic operations... ";
  PriorityQueue pq;
  bool passed = true;

  try {
    assert(pq.isEmpty());
    assert(pq.getSize() == 0);

    pq.insert(50);
    pq.insert(20);
    pq.insert(80);
    pq.insert(10);
    pq.insert(20);

    assert(!pq.isEmpty());
    assert(pq.getSize() == 4);

    assert(pq.pop() == 10);
    assert(pq.getSize() == 3);
    assert(pq.pop() == 20);
    assert(pq.getSize() == 2);
    assert(pq.pop() == 50);
    assert(pq.getSize() == 1);
    assert(pq.pop() == 80);
    assert(pq.getSize() == 0);
    assert(pq.isEmpty());
  } catch (...) {
    passed = false;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testPqSetEmptyPop() {
  std::cout << "Testing priorityQueue empty pop exception... ";
  PriorityQueue pq;
  bool passed = false;

  try {
    pq.pop();
  } catch (const std::runtime_error &e) {
    passed = true;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testPqSetLarge() {
  std::cout << "Testing priorityQueue with large number of elements... ";
  PriorityQueue pq;
  bool passed = true;

  try {
    for (int i = 1000; i >= 0; i--) {
      pq.insert(i);
    }
    assert(pq.getSize() == 1001);

    for (int i = 0; i <= 1000; i++) {
      assert(pq.pop() == i);
    }
    assert(pq.isEmpty());
  } catch (...) {
    passed = false;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testPqBinaryBasic() {
  std::cout << "Testing PriorityQueueBinary basic operations... ";
  PriorityQueueBinary pq;
  bool passed = true;

  try {
    assert(pq.isEmpty());
    assert(pq.size() == 0);

    pq.insert(50);
    pq.insert(20);
    pq.insert(80);
    pq.insert(10);
    pq.insert(20);

    assert(!pq.isEmpty());
    assert(pq.size() == 5);

    assert(pq.pop() == 10);
    assert(pq.size() == 4);
    assert(pq.pop() == 20);
    assert(pq.size() == 3);
    assert(pq.pop() == 20);
    assert(pq.size() == 2);
    assert(pq.pop() == 50);
    assert(pq.size() == 1);
    assert(pq.pop() == 80);
    assert(pq.size() == 0);
    assert(pq.isEmpty());
  } catch (...) {
    passed = false;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testPqBinaryEmptyPop() {
  std::cout << "Testing PriorityQueueBinary empty pop exception... ";
  PriorityQueueBinary pq;
  bool passed = false;

  try {
    pq.pop();
  } catch (const std::runtime_error &e) {
    passed = true;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testPqBinaryLarge() {
  std::cout << "Testing PriorityQueueBinary with large number of elements... ";
  PriorityQueueBinary pq;
  bool passed = true;

  try {
    for (int i = 1000; i >= 0; i--) {
      pq.insert(i);
    }
    assert(pq.size() == 1001);

    for (int i = 0; i <= 1000; i++) {
      assert(pq.pop() == i);
    }
    assert(pq.isEmpty());
  } catch (...) {
    passed = false;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void testPqBinaryDuplicates() {
  std::cout << "Testing PriorityQueueBinary duplicate handling... ";
  PriorityQueueBinary pq;
  bool passed = true;

  try {
    pq.insert(5);
    pq.insert(5);
    pq.insert(5);
    assert(pq.size() == 3);
    assert(pq.pop() == 5);
    assert(pq.pop() == 5);
    assert(pq.pop() == 5);
  } catch (...) {
    passed = false;
  }
  std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

int main() {
  std::cout << "\n=== Testing Set-based Priority Queue ===\n";
  testPqSetBasic();
  testPqSetEmptyPop();
  testPqSetLarge();

  std::cout << "\n=== Testing Binary Heap Priority Queue ===\n";
  testPqBinaryBasic();
  testPqBinaryEmptyPop();
  testPqBinaryLarge();
  testPqBinaryDuplicates();

  std::cout << "\nAll tests completed.\n";
  return 0;
}
