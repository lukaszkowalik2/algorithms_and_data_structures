#include <algorithm>
#include <limits>

#include "set-int-hashed.h"

size_t SetIntHashed::hash(int element) const {
  std::hash<int> hasher;
  size_t hashVal = hasher(element);
  return hashVal % tableSize;
}

bool SetIntHashed::findInList(const std::forward_list<int> &list, int element) const {
  for (int item : list) {
    if (item == element) {
      return true;
    }
  }
  return false;
}

SetIntHashed::SetIntHashed(size_t initialTableSize) : numElements(0) {
  tableSize = (initialTableSize == 0) ? 1 : initialTableSize;
  table.resize(tableSize);
}

void SetIntHashed::insert(int element) {
  size_t index = hash(element);
  if (!findInList(table[index], element)) {
    table[index].push_front(element);
    numElements++;
  }
}

void SetIntHashed::remove(int element) {
  size_t index = hash(element);
  auto &list = table[index];
  auto beforeIt = list.before_begin();
  for (auto it = list.begin(); it != list.end(); ++it, ++beforeIt) {
    if (*it == element) {
      list.erase_after(beforeIt);
      numElements--;
      return;
    }
  }
}

bool SetIntHashed::contains(int element) const {
  size_t index = hash(element);
  return findInList(table[index], element);
}

size_t SetIntHashed::size() const {
  return numElements;
}

bool SetIntHashed::empty() const {
  return numElements == 0;
}

std::vector<int> SetIntHashed::getElements() const {
  std::vector<int> elements;
  elements.reserve(numElements);
  for (const auto &list : table) {
    for (int element : list) {
      elements.push_back(element);
    }
  }
  return elements;
}
