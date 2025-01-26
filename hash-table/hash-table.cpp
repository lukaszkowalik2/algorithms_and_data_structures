#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <vector>

unsigned int hashFunction(unsigned int x) {
  unsigned int MASK_16_BITS = 0xFFFF;
  unsigned int PRIME1 = 0x8003;  // 32771
  unsigned int PRIME2 = 0x9E3B;  // 40507
  unsigned int PRIME3 = 0x00FB;  // 251
  unsigned int PRIME4 = 0xA3D7;  // 41943

  x ^= (x << 7) | (x >> 9);
  x *= PRIME1;
  x ^= x >> 5;

  x += 0x7A89;
  x = (x ^ (x << 3)) * PRIME2;
  x = (x >> 11) | (x << 5);

  x ^= (x << 9) ^ (x >> 7);
  x *= PRIME3;
  x ^= (x << 1) | (x >> 15);
  x += x * PRIME4;

  return x & MASK_16_BITS;
}

class HashTable {
 private:
  std::vector<std::list<int>> table;
  unsigned int capacity;

 public:
  HashTable(unsigned int cap = 10)
      : capacity(cap) {
    table.resize(capacity);
  }

  void insertKey(int key) {
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;
    table[index].push_front(key);
  }

  bool removeKey(int key) {
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;
    auto& bucket = table[index];
    auto it = std::find(bucket.begin(), bucket.end(), key);
    if (it != bucket.end()) {
      bucket.erase(it);
      return true;
    }
    return false;
  }

  bool contains(int key) const {
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;
    const auto& bucket = table[index];
    return std::find(bucket.begin(), bucket.end(), key) != bucket.end();
  }

  std::optional<int> searchKey(int key) const {
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;
    const auto& bucket = table[index];
    auto it = std::find(bucket.begin(), bucket.end(), key);
    return it != bucket.end() ? std::optional<int>(*it) : std::nullopt;
  }

  void printTable() const {
    for (size_t i = 0; i < capacity; i++) {
      std::cout << "Index " << i << ": ";
      if (table[i].empty()) {
        std::cout << "empty\n";
      } else {
        for (const auto& key : table[i]) {
          std::cout << key << " -> ";
        }
        std::cout << "end\n";
      }
    }
  }
};

class ClosedHashTable {
  std::vector<int> table;
  unsigned int capacity;
  unsigned int size;

  const int EMPTY_MARK = INT32_MIN;

 public:
  ClosedHashTable(unsigned int cap = 10)
      : capacity(cap), size(0) {
    table.resize(capacity);
    std::fill(table.begin(), table.end(), EMPTY_MARK);
  }

  bool insertKey(int key) {
    if (size >= capacity) return false;
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;
    unsigned int originalIndex = index;

    while (true) {
      if (table[index] == EMPTY_MARK) {
        table[index] = key;
        size++;
        return true;
      }

      index = (index + 1) % capacity;
      if (index == originalIndex) {
        return false;
      }
    }
  }

  bool removeKey(int key) {
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;

    while (table[index] != EMPTY_MARK) {
      if (table[index] == key) {
        size--;
        moveToLeftFromIndex(index);
        return true;
      }

      index = (index + 1) % capacity;
    }
  }

  bool contains(int key) const {
    unsigned int index = hashFunction(static_cast<unsigned int>(key)) % capacity;
    unsigned int originalIndex = index;
    while (true) {
      if (table[index] == key) {
        return true;
      }

      index = (index + 1) % capacity;
      if (index == originalIndex || table[index] == EMPTY_MARK) {
        return false;
      }
    }
  }

  void printTable() const {
    for (size_t i = 0; i < capacity; i++) {
      std::cout << "Index " << i << ": ";
      if (table[i] == EMPTY_MARK) {
        std::cout << "empty";
      } else {
        std::cout << table[i];
      }
      std::cout << "\n";
    }
  }

  void moveToLeftFromIndex(unsigned int index) {
    unsigned int originalIndex = index;
    while (true) {
      int nextKey = table[(index + 1) % capacity];
      table[index] = nextKey;

      index = (index + 1) % capacity;
      if (index == originalIndex || table[index] == INT32_MIN) {
        return;
      }
    }
  }
};

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file.txt>\n";
    return EXIT_FAILURE;
  }

  std::ifstream inFile(argv[1]);
  if (!inFile.is_open()) {
    std::cerr << "Failed to open file: " << argv[1] << "\n";
    return EXIT_FAILURE;
  }

  HashTable openTable(50);
  ClosedHashTable closedTable(50);

  int number;
  while (inFile >> number) {
    openTable.insertKey(number);
    closedTable.insertKey(number);
  }
  inFile.close();

  std::cout << "Open hashing (separate chaining) table contents:\n";
  openTable.printTable();

  std::cout << "\nClosed hashing (linear probing) table contents:\n";
  closedTable.printTable();

  std::cout << "\nTesting Open Hashing:\n";
  std::cout << "Does key 16 exist? " << (openTable.contains(16) ? "YES" : "NO") << "\n";
  std::cout << "Removing key 16...\n";
  openTable.removeKey(16);
  std::cout << "Does key 16 exist? " << (openTable.contains(16) ? "YES" : "NO") << "\n";

  int searchKey = 11;
  auto found = openTable.searchKey(searchKey);
  if (found) {
    std::cout << "Found key " << searchKey << " in open hashing table: " << found.value() << "\n";
  } else {
    std::cout << "Key " << searchKey << " not found in open hashing table.\n";
  }

  std::cout << "\nTesting Closed Hashing:\n";
  std::cout << "Does key 99 exist? " << (closedTable.contains(99) ? "YES" : "NO") << "\n";
  std::cout << "Removing key 99...\n";
  closedTable.removeKey(99);
  std::cout << "Does key 99 exist? " << (closedTable.contains(99) ? "YES" : "NO") << "\n";

  if (found) {
    std::cout << "Found key " << searchKey << " in closed hashing table: " << found.value() << "\n";
  } else {
    std::cout << "Key " << searchKey << " not found in closed hashing table.\n";
  }

  std::cout << "Open hashing (separate chaining) table contents:\n";
  openTable.printTable();

  std::cout << "\nClosed hashing (linear probing) table contents:\n";
  closedTable.printTable();

  return 0;
}