#include "priority-queue.h"
#include <limits>
#include <stdexcept>
#include <vector>

PriorityQueue::PriorityQueue(size_t initialCapacity) : storage(initialCapacity) {}

void PriorityQueue::insert(int element) {
  storage.insert(element);
}

int PriorityQueue::pop() {
  if (storage.empty()) {
    throw std::runtime_error("Cannot pop from an empty priority queue");
  }

  std::vector<int> elements = storage.getElements();
  if (elements.empty()) {
    throw std::logic_error("Internal error: storage not empty but getElements is");
  }

  int minPriority = elements[0];
  for (size_t i = 1; i < elements.size(); ++i) {
    if (elements[i] < minPriority) {
      minPriority = elements[i];
    }
  }

  storage.remove(minPriority);
  return minPriority;
}

bool PriorityQueue::isEmpty() const {
  return storage.empty();
}

size_t PriorityQueue::getSize() const {
  return storage.size();
}
