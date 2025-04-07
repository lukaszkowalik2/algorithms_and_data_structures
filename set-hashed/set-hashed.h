#ifndef SET_HASHED_H
#define SET_HASHED_H

#include <vector>
#include <string>
#include <forward_list>
#include <functional>
#include <cstddef>

class setHashed {
private:
    std::vector<std::forward_list<std::string>> table;
    size_t num_elements;
    size_t table_size;

    size_t _hash(const std::string& element) const;
    bool _find_in_list(const std::forward_list<std::string>& list,
                       const std::string& element) const;

public:
    explicit setHashed(size_t initial_table_size = 10);

    void insert(const std::string& element);
    void remove(const std::string& element);
    bool contains(const std::string& element) const;
    size_t size() const;

    static setHashed setUnion(const setHashed& set1, const setHashed& set2);
    static setHashed intersection(const setHashed& set1, const setHashed& set2);
    static setHashed difference(const setHashed& set1, const setHashed& set2);
    static bool equals(const setHashed& set1, const setHashed& set2);

    std::vector<std::string> get_elements() const;
    size_t get_table_size() const;
};

#endif // SET_HASHED_H
