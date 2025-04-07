#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

#include "priority-queue.h"
#include "priority-queue-binary.h"

void test_pq_set_basic() {
    std::cout << "Testing priorityQueue basic operations... ";
    priorityQueue pq;
    bool passed = true;

    try {
        assert(pq.empty());
        assert(pq.size() == 0);

        pq.insert(50);
        pq.insert(20);
        pq.insert(80);
        pq.insert(10);
        pq.insert(20);

        assert(!pq.empty());
        assert(pq.size() == 4);

        assert(pq.pop() == 10);
        assert(pq.size() == 3);
        assert(pq.pop() == 20);
        assert(pq.size() == 2);
        assert(pq.pop() == 50);
        assert(pq.size() == 1);
        assert(pq.pop() == 80);
        assert(pq.size() == 0);
        assert(pq.empty());
    } catch (...) {
        passed = false;
    }
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_pq_set_empty_pop() {
    std::cout << "Testing priorityQueue empty pop exception... ";
    priorityQueue pq;
    bool passed = false;

    try {
        pq.pop();
    } catch (const std::runtime_error& e) {
        passed = true;
    }
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_pq_set_large() {
    std::cout << "Testing priorityQueue with large number of elements... ";
    priorityQueue pq;
    bool passed = true;

    try {
        for(int i = 1000; i >= 0; i--) {
            pq.insert(i);
        }
        assert(pq.size() == 1001);

        for(int i = 0; i <= 1000; i++) {
            assert(pq.pop() == i);
        }
        assert(pq.empty());
    } catch (...) {
        passed = false;
    }
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_pq_binary_basic() {
    std::cout << "Testing priorityQueueBinary basic operations... ";
    priorityQueueBinary pq;
    bool passed = true;

    try {
        assert(pq.empty());
        assert(pq.size() == 0);

        pq.insert(50);
        pq.insert(20);
        pq.insert(80);
        pq.insert(10);
        pq.insert(20);

        assert(!pq.empty());
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
        assert(pq.empty());
    } catch (...) {
        passed = false;
    }
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_pq_binary_empty_pop() {
    std::cout << "Testing priorityQueueBinary empty pop exception... ";
    priorityQueueBinary pq;
    bool passed = false;

    try {
        pq.pop();
    } catch (const std::runtime_error& e) {
        passed = true;
    }
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_pq_binary_large() {
    std::cout << "Testing priorityQueueBinary with large number of elements... ";
    priorityQueueBinary pq;
    bool passed = true;

    try {
        for(int i = 1000; i >= 0; i--) {
            pq.insert(i);
        }
        assert(pq.size() == 1001);

        for(int i = 0; i <= 1000; i++) {
            assert(pq.pop() == i);
        }
        assert(pq.empty());
    } catch (...) {
        passed = false;
    }
    std::cout << (passed ? "PASSED" : "FAILED") << std::endl;
}

void test_pq_binary_duplicates() {
    std::cout << "Testing priorityQueueBinary duplicate handling... ";
    priorityQueueBinary pq;
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
    test_pq_set_basic();
    test_pq_set_empty_pop();
    test_pq_set_large();

    std::cout << "\n=== Testing Binary Heap Priority Queue ===\n";
    test_pq_binary_basic();
    test_pq_binary_empty_pop();
    test_pq_binary_large();
    test_pq_binary_duplicates();

    std::cout << "\nAll tests completed.\n";
    return 0;
}
