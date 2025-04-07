#include "set-hashed.h"
#include <algorithm>
#include <iostream>

size_t setHashed::hash(const std::string &element) const {
  std::hash<std::string> hasher;
  return hasher(element) % tableSize;
}

bool setHashed::findInList(const std::forward_list<std::string> &list,
                           const std::string &element) const {
  for (const std::string &item : list) {
    if (item == element) {
      return true;
    }
  }
  return false;
}

setHashed::setHashed(size_t initialTableSize) : numElements(0) {
  tableSize = (initialTableSize == 0) ? 1 : initialTableSize;
  table.resize(tableSize);
}

void setHashed::insert(const std::string &element) {
  if (element.length() != 4)
    return;
  size_t index = hash(element);
  if (!findInList(table[index], element)) {
    table[index].push_front(element);
    numElements++;
  }
}

void setHashed::remove(const std::string &element) {
  if (element.length() != 4)
    return;
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

bool setHashed::contains(const std::string &element) const {
  if (element.length() != 4)
    return false;
  size_t index = hash(element);
  return findInList(table[index], element);
}

size_t setHashed::size() const {
  return numElements;
}

setHashed setHashed::setUnion(const setHashed &set1, const setHashed &set2) {
  setHashed result(std::max(set1.tableSize, set2.tableSize));
  for (const auto &list : set1.table) {
    for (const std::string &element : list) {
      result.insert(element);
    }
  }
  for (const auto &list : set2.table) {
    for (const std::string &element : list) {
      result.insert(element);
    }
  }
  return result;
}

setHashed setHashed::intersection(const setHashed &set1,
                                  const setHashed &set2) {
  setHashed result(std::min(set1.tableSize, set2.tableSize));
  const setHashed &smaller = (set1.size() < set2.size()) ? set1 : set2;
  const setHashed &larger = (set1.size() < set2.size()) ? set2 : set1;

  for (const auto &list : smaller.table) {
    for (const std::string &element : list) {
      if (larger.contains(element)) {
        result.insert(element);
      }
    }
  }
  return result;
}

setHashed setHashed::difference(const setHashed &set1, const setHashed &set2) {
  setHashed result(set1.tableSize);
  for (const auto &list : set1.table) {
    for (const std::string &element : list) {
      result.insert(element);
    }
  }
  for (const auto &list : set2.table) {
    for (const std::string &element : list) {
      result.remove(element);
    }
  }
  return result;
}

bool setHashed::equals(const setHashed &set1, const setHashed &set2) {
  if (set1.size() != set2.size()) {
    return false;
  }
  for (const auto &list : set1.table) {
    for (const std::string &element : list) {
      if (!set2.contains(element)) {
        return false;
      }
    }
  }
  return true;
}

std::vector<std::string> setHashed::getElements() const {
  std::vector<std::string> elements;
  elements.reserve(numElements);
  for (const auto &list : table) {
    for (const std::string &element : list) {
      elements.push_back(element);
    }
  }
  std::sort(elements.begin(), elements.end());
  return elements;
}

size_t setHashed::getTableSize() const {
  return tableSize;
}
