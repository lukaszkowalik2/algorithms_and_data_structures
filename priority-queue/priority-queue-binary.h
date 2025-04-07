#ifndef PRIORITY_QUEUE_BINARY_H
#define PRIORITY_QUEUE_BINARY_H

#include <vector>
#include <stdexcept>
#include <cstddef>

class priorityQueueBinary {
private:
    std::vector<int> heap;

    void heapify_up(size_t index);
    void heapify_down(size_t index);
    size_t parent(size_t index);
    size_t left_child(size_t index);
    size_t right_child(size_t index);

public:
    priorityQueueBinary();

    void insert(int element);
    int pop();
    bool empty() const;
    size_t size() const;
};

#endif // PRIORITY_QUEUE_BINARY_H
