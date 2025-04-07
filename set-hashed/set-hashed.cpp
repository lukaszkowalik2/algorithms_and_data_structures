#include "set-hashed.h"
#include <algorithm>
#include <iostream>

size_t setHashed::_hash(const std::string& element) const {
    std::hash<std::string> hasher;
    return hasher(element) % table_size;
}

bool setHashed::_find_in_list(const std::forward_list<std::string>& list,
                              const std::string& element) const {
    for (const std::string& item : list) {
        if (item == element) {
            return true;
        }
    }
    return false;
}

setHashed::setHashed(size_t initial_table_size) : num_elements(0) {
    table_size = (initial_table_size == 0) ? 1 : initial_table_size;
    table.resize(table_size);
}

void setHashed::insert(const std::string& element) {
    if (element.length() != 4) return;
    size_t index = _hash(element);
    if (!_find_in_list(table[index], element)) {
        table[index].push_front(element);
        num_elements++;
    }
}

void setHashed::remove(const std::string& element) {
     if (element.length() != 4) return;
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

bool setHashed::contains(const std::string& element) const {
    if (element.length() != 4) return false;
    size_t index = _hash(element);
    return _find_in_list(table[index], element);
}

size_t setHashed::size() const {
    return num_elements;
}

setHashed setHashed::setUnion(const setHashed& set1, const setHashed& set2) {
    setHashed result(std::max(set1.table_size, set2.table_size));
    for (const auto& list : set1.table) {
        for (const std::string& element : list) {
            result.insert(element);
        }
    }
    for (const auto& list : set2.table) {
        for (const std::string& element : list) {
            result.insert(element);
        }
    }
    return result;
}

setHashed setHashed::intersection(const setHashed& set1,
                                  const setHashed& set2) {
    setHashed result(std::min(set1.table_size, set2.table_size));
    const setHashed& smaller = (set1.size() < set2.size()) ? set1 : set2;
    const setHashed& larger = (set1.size() < set2.size()) ? set2 : set1;

    for (const auto& list : smaller.table) {
        for (const std::string& element : list) {
            if (larger.contains(element)) {
                result.insert(element);
            }
        }
    }
    return result;
}

setHashed setHashed::difference(const setHashed& set1, const setHashed& set2) {
    setHashed result(set1.table_size);
    for (const auto& list : set1.table) {
        for (const std::string& element : list) {
            result.insert(element);
        }
    }
    for (const auto& list : set2.table) {
        for (const std::string& element : list) {
            result.remove(element);
        }
    }
    return result;
}

bool setHashed::equals(const setHashed& set1, const setHashed& set2) {
    if (set1.size() != set2.size()) {
        return false;
    }
    for (const auto& list : set1.table) {
        for (const std::string& element : list) {
            if (!set2.contains(element)) {
                return false;
            }
        }
    }
    return true;
}

std::vector<std::string> setHashed::get_elements() const {
    std::vector<std::string> elements;
    elements.reserve(num_elements);
    for (const auto& list : table) {
        for (const std::string& element : list) {
            elements.push_back(element);
        }
    }
    std::sort(elements.begin(), elements.end());
    return elements;
}

size_t setHashed::get_table_size() const {
    return table_size;
}
