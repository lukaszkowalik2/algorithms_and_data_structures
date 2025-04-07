#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "set-int-hashed.h"

class PriorityQueue {
  private:
  SetIntHashed storage;

  public:
  PriorityQueue(size_t initialCapacity = 10);

  void insert(int element);
  int pop();
  bool isEmpty() const;
  size_t getSize() const;
};

#endif // PRIORITY_QUEUE_H
