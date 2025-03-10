#ifndef DICTIONARY_SIMPLE_HPP
#define DICTIONARY_SIMPLE_HPP

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

class DictionarySimple {
  private:
  std::string *table;
  size_t capacity;
  size_t count;
  const size_t MAX_STRING_LENGTH = 50;

  bool isValidString(const std::string &s) const {
    return s.size() == MAX_STRING_LENGTH;
  }

  void resize() {
    size_t new_capacity = capacity * 2;
    std::string *new_table = new std::string[new_capacity];

    for (size_t i = 0; i < count; i++) {
      new_table[i] = table[i];
    }

    delete[] table;
    table = new_table;
    capacity = new_capacity;
  }

  public:
  DictionarySimple() : capacity(10), count(0) {
    table = new std::string[capacity];
  }

  DictionarySimple(size_t initial_capacity) : capacity(initial_capacity), count(0) {
    table = new std::string[capacity];
  }

  ~DictionarySimple() {
    delete[] table;
  }

  DictionarySimple(const DictionarySimple &other) : capacity(other.capacity), count(other.count) {
    table = new std::string[capacity];
    for (size_t i = 0; i < count; i++) {
      table[i] = other.table[i];
    }
  }

  DictionarySimple &operator=(const DictionarySimple &other) {
    if (this != &other) {
      delete[] table;
      capacity = other.capacity;
      count = other.count;
      table = new std::string[capacity];
      for (size_t i = 0; i < count; i++) {
        table[i] = other.table[i];
      }
    }
    return *this;
  }

  void insert(const std::string &s) {
    if (!isValidString(s)) {
      throw std::invalid_argument("String must be 50 characters long");
    }

    for (size_t i = 0; i < count; i++) {
      if (table[i] == s)
        return;
    }

    if (count >= capacity) {
      resize();
    }

    table[count] = s;
    count++;
  }

  void remove(const std::string &s) {
    for (size_t i = 0; i < count; i++) {
      if (table[i] == s) {
        for (size_t j = i; j < count - 1; j++) {
          table[j] = table[j + 1];
        }
        count--;
        return;
      }
    }
  }

  bool contains(const std::string &s) const {
    for (size_t i = 0; i < count; i++) {
      if (table[i] == s)
        return true;
    }
    return false;
  }
};

#endif // DICTIONARY_SIMPLE_HPP