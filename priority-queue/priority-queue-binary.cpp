#include <algorithm>

#include "priority-queue-binary.h"

PriorityQueueBinary::PriorityQueueBinary() {}

size_t PriorityQueueBinary::parent(size_t index) {
  return (index - 1) / 2;
}

size_t PriorityQueueBinary::leftChild(size_t index) {
  return 2 * index + 1;
}

size_t PriorityQueueBinary::rightChild(size_t index) {
  return 2 * index + 2;
}

void PriorityQueueBinary::heapifyUp(size_t index) {
  while (index > 0 && heap[index] < heap[parent(index)]) {
    std::swap(heap[index], heap[parent(index)]);
    index = parent(index);
  }
}

void PriorityQueueBinary::heapifyDown(size_t index) {
  size_t smallest = index;
  size_t left = leftChild(index);
  size_t right = rightChild(index);

  if (left < heap.size() && heap[left] < heap[smallest]) {
    smallest = left;
  }

  if (right < heap.size() && heap[right] < heap[smallest]) {
    smallest = right;
  }

  if (smallest != index) {
    std::swap(heap[index], heap[smallest]);
    heapifyDown(smallest);
  }
}

void PriorityQueueBinary::insert(int element) {
  heap.push_back(element);
  heapifyUp(heap.size() - 1);
}

int PriorityQueueBinary::pop() {
  if (heap.empty()) {
    throw std::runtime_error("Cannot pop from an empty priority queue");
  }

  int minElement = heap[0];

  heap[0] = heap.back();
  heap.pop_back();

  if (!heap.empty()) {
    heapifyDown(0);
  }

  return minElement;
}

bool PriorityQueueBinary::isEmpty() const {
  return heap.empty();
}

size_t PriorityQueueBinary::size() const {
  return heap.size();
}
