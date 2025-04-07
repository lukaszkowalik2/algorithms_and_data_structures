#include "priority-queue.h"
#include <vector>
#include <limits>
#include <stdexcept>

priorityQueue::priorityQueue(size_t initial_capacity) : storage(initial_capacity) {}

void priorityQueue::insert(int element) {
    storage.insert(element);
}

int priorityQueue::pop() {
    if (storage.empty()) {
        throw std::runtime_error("Cannot pop from an empty priority queue");
    }

    std::vector<int> elements = storage.get_elements();
    if (elements.empty()) {
         throw std::logic_error("Internal error: storage not empty but get_elements is");
    }

    int min_priority = elements[0];
    for (size_t i = 1; i < elements.size(); ++i) {
        if (elements[i] < min_priority) {
            min_priority = elements[i];
        }
    }

    storage.remove(min_priority);
    return min_priority;
}

bool priorityQueue::empty() const {
    return storage.empty();
}

size_t priorityQueue::size() const {
    return storage.size();
}
