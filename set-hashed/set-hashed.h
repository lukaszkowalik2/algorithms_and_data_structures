#ifndef SET_HASHED_H
#define SET_HASHED_H

#include <cstddef>
#include <forward_list>
#include <functional>
#include <string>
#include <vector>

class setHashed {
  private:
  std::vector<std::forward_list<std::string>> table;
  size_t numElements;
  size_t tableSize;

  size_t hash(const std::string &element) const;
  bool findInList(const std::forward_list<std::string> &list,
                  const std::string &element) const;

  public:
  explicit setHashed(size_t initialTableSize = 10);

  void insert(const std::string &element);
  void remove(const std::string &element);
  bool contains(const std::string &element) const;
  size_t size() const;

  static setHashed setUnion(const setHashed &set1, const setHashed &set2);
  static setHashed intersection(const setHashed &set1, const setHashed &set2);
  static setHashed difference(const setHashed &set1, const setHashed &set2);
  static bool equals(const setHashed &set1, const setHashed &set2);

  std::vector<std::string> getElements() const;
  size_t getTableSize() const;
};

#endif // SET_HASHED_H
