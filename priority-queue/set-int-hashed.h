#ifndef SET_INT_HASHED_H
#define SET_INT_HASHED_H

#include <cstddef>
#include <forward_list>
#include <functional>
#include <stdexcept>
#include <vector>

class SetIntHashed {
  private:
  std::vector<std::forward_list<int>> table;
  size_t numElements;
  size_t tableSize;

  size_t hash(int element) const;
  bool findInList(const std::forward_list<int> &list, int element) const;

  public:
  explicit SetIntHashed(size_t initialTableSize = 10);

  void insert(int element);
  void remove(int element);
  bool contains(int element) const;
  size_t size() const;

  std::vector<int> getElements() const;
  bool empty() const;
};

#endif // SET_INT_HASHED_H
