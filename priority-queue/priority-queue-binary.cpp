#include <algorithm>

#include "priority-queue-binary.h"

priorityQueueBinary::priorityQueueBinary() {}

size_t priorityQueueBinary::parent(size_t index) {
    return (index - 1) / 2;
}

size_t priorityQueueBinary::left_child(size_t index) {
    return 2 * index + 1;
}

size_t priorityQueueBinary::right_child(size_t index) {
    return 2 * index + 2;
}

void priorityQueueBinary::heapify_up(size_t index) {
    while (index > 0 && heap[index] < heap[parent(index)]) {
        std::swap(heap[index], heap[parent(index)]);
        index = parent(index);
    }
}

void priorityQueueBinary::heapify_down(size_t index) {
    size_t smallest = index;
    size_t left = left_child(index);
    size_t right = right_child(index);

    if (left < heap.size() && heap[left] < heap[smallest]) {
        smallest = left;
    }

    if (right < heap.size() && heap[right] < heap[smallest]) {
        smallest = right;
    }

    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        heapify_down(smallest);
    }
}

void priorityQueueBinary::insert(int element) {
    heap.push_back(element);
    heapify_up(heap.size() - 1);
}

int priorityQueueBinary::pop() {
    if (heap.empty()) {
        throw std::runtime_error("Cannot pop from an empty priority queue");
    }

    int min_element = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapify_down(0);
    }

    return min_element;
}

bool priorityQueueBinary::empty() const {
    return heap.empty();
}

size_t priorityQueueBinary::size() const {
    return heap.size();
}
