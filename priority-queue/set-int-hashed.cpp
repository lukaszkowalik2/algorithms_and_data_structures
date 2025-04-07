#include <algorithm>
#include <limits>

#include "set-int-hashed.h"

size_t setIntHashed::_hash(int element) const {
    std::hash<int> hasher;
    size_t hash_val = hasher(element);
    return hash_val % table_size;
}


bool setIntHashed::_find_in_list(const std::forward_list<int>& list, int element) const {
    for (int item : list) {
        if (item == element) {
            return true;
        }
    }
    return false;
}

setIntHashed::setIntHashed(size_t initial_table_size) : num_elements(0) {
    table_size = (initial_table_size == 0) ? 1 : initial_table_size;
    table.resize(table_size);
}

void setIntHashed::insert(int element) {
    size_t index = _hash(element);
    if (!_find_in_list(table[index], element)) {
        table[index].push_front(element);
        num_elements++;
    }
}

void setIntHashed::remove(int element) {
    size_t index = _hash(element);
    auto& list = table[index];
    auto before_it = list.before_begin();
    for (auto it = list.begin(); it != list.end(); ++it, ++before_it) {
        if (*it == element) {
            list.erase_after(before_it);
            num_elements--;
            return;
        }
    }
}

bool setIntHashed::contains(int element) const {
    size_t index = _hash(element);
    return _find_in_list(table[index], element);
}

size_t setIntHashed::size() const {
    return num_elements;
}

bool setIntHashed::empty() const {
    return num_elements == 0;
}


std::vector<int> setIntHashed::get_elements() const {
    std::vector<int> elements;
    elements.reserve(num_elements);
    for (const auto& list : table) {
        for (int element : list) {
            elements.push_back(element);
        }
    }
    return elements;
}
