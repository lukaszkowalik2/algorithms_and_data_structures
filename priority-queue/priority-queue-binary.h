#ifndef PRIORITY_QUEUE_BINARY_H
#define PRIORITY_QUEUE_BINARY_H

#include <cstddef>
#include <stdexcept>
#include <vector>

class PriorityQueueBinary {
  private:
  std::vector<int> heap;

  void heapifyUp(size_t index);
  void heapifyDown(size_t index);
  size_t parent(size_t index);
  size_t leftChild(size_t index);
  size_t rightChild(size_t index);

  public:
  PriorityQueueBinary();

  void insert(int element);
  int pop();
  bool isEmpty() const;
  size_t size() const;
};

#endif // PRIORITY_QUEUE_BINARY_H
