#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "set-int-hashed.h"

class priorityQueue {
private:
    setIntHashed storage;

public:
    priorityQueue(size_t initial_capacity = 10);

    void insert(int element);
    int pop();
    bool empty() const;
    size_t size() const;
};

#endif // PRIORITY_QUEUE_H
