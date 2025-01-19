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
  unsigned int PRIME1 = 0xA5A5u;
  unsigned int PRIME2 = 17;
  unsigned int PRIME3 = 37;

  x &= MASK_16_BITS;
  x ^= (x << 7);
  x ^= (x >> 9);
  x = (x + 9407) * 791 / 41;
  x = (x << 3) - (x >> 2);
  x *= PRIME1;
  x ^= ((x >> 16) * PRIME1) & MASK_16_BITS;
  x ^= (x >> 5);
  x = (x * PRIME2 + 88) ^ (x << 3);
  x += (x >> 7) * PRIME3;
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

  HashTable ht(50);

  int number;
  while (inFile >> number) {
    ht.insertKey(number);
  }
  inFile.close();

  std::cout << "Hash table contents:\n";
  ht.printTable();

  std::cout << "\nDoes key 16 exist? ";
  std::cout << (ht.contains(16) ? "YES\n" : "NO\n");

  std::cout << "Removing key 16...\n";
  ht.removeKey(16);

  std::cout << "Does key 16 exist? ";
  std::cout << (ht.contains(16) ? "YES\n" : "NO\n");

  int searchKey = 11;
  auto found = ht.searchKey(searchKey);
  if (found.has_value()) {
    std::cout << "\nFound key: " << found.value() << "\n";
  } else {
    std::cout << "\nKey not found: " << searchKey << "\n";
  }

  return 0;
}
