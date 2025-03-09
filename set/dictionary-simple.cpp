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

  private:
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
};

int main() {
  std::cout << "===== Testowanie klasy DictionarySimple =====" << std::endl;

  std::cout << "\n[Test 1] Dodawanie elementów... ";
  DictionarySimple dict(5);
  dict.insert(std::string(50, 'a'));
  dict.insert(std::string(50, 'b'));
  bool test1_ok = dict.contains(std::string(50, 'a')) && dict.contains(std::string(50, 'b')) && !dict.contains(std::string(50, 'c'));
  std::cout << (test1_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 2] Usuwanie elementu... ";
  dict.remove(std::string(50, 'a'));
  bool test2_ok = !dict.contains(std::string(50, 'a'));
  std::cout << (test2_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 3] Nieprawidłowa długość ciągu... ";
  bool test3_ok = true;
  try {
    dict.insert("test");
    test3_ok = false;
  } catch (const std::invalid_argument &) {
    test3_ok = true;
  }
  std::cout << (test3_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "\n===== Koniec testów =====" << std::endl;
  return 0;
}