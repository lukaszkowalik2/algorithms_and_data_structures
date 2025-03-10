#ifndef DICTIONARY_SIMPLE_HPP
#define DICTIONARY_SIMPLE_HPP

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

struct Bucket {
  std::list<std::string> entries;
};

class DictionarySimple {
  private:
  std::vector<Bucket> table;
  size_t capacity;
  size_t size;
  const size_t MAX_STRING_LENGTH = 50;

  size_t hash(const std::string &s) const {
    return std::hash<std::string>{}(s) % capacity;
  }

  bool isValidString(const std::string &s) const {
    return s.size() == MAX_STRING_LENGTH;
  }

  void rehash() {
    size_t new_capacity = capacity * 2;
    std::vector<Bucket> new_table(new_capacity);

    for (const auto &bucket : table) {
      for (const auto &entry : bucket.entries) {
        size_t new_index = std::hash<std::string>{}(entry) % new_capacity;
        new_table[new_index].entries.push_back(entry);
      }
    }

    table = std::move(new_table);
    capacity = new_capacity;
  }

  public:
  DictionarySimple(size_t initial_capacity = 100)
      : capacity(initial_capacity), size(0) {
    table.resize(capacity);
  }

  void insert(const std::string &s) {
    if (!isValidString(s)) {
      throw std::invalid_argument("String must be 50 characters long");
    }

    size_t index = hash(s);
    for (const auto &entry : table[index].entries) {
      if (entry == s)
        return;
    }

    table[index].entries.push_back(s);
    size++;

    if (size > 0.7 * capacity) {
      rehash();
    }
  }

  void remove(const std::string &s) {
    size_t index = hash(s);
    auto &entries = table[index].entries;

    for (auto it = entries.begin(); it != entries.end(); ++it) {
      if (*it == s) {
        entries.erase(it);
        size--;
        return;
      }
    }
  }

  bool contains(const std::string &s) const {
    size_t index = hash(s);
    const auto &entries = table[index].entries;

    for (const auto &entry : entries) {
      if (entry == s)
        return true;
    }
    return false;
  }
};

#endif // DICTIONARY_SIMPLE_HPP