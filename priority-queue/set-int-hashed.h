#ifndef SET_INT_HASHED_H
#define SET_INT_HASHED_H

#include <vector>
#include <forward_list>
#include <functional>
#include <cstddef>
#include <stdexcept>

class setIntHashed {
private:
    std::vector<std::forward_list<int>> table;
    size_t num_elements;
    size_t table_size;

    size_t _hash(int element) const;
    bool _find_in_list(const std::forward_list<int>& list, int element) const;

public:
    explicit setIntHashed(size_t initial_table_size = 10);

    void insert(int element);
    void remove(int element);
    bool contains(int element) const;
    size_t size() const;

    std::vector<int> get_elements() const;
    bool empty() const;
};

#endif // SET_INT_HASHED_H
