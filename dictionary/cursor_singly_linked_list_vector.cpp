#include <iostream>
#include <vector>

struct CursorNodeVector {
  int key;
  int next;

  CursorNodeVector(int k = 0, int n = -1)
      : key(k), next(n) {
  }
};

class CursorSinglyLinkedListVector {
 private:
  std::vector<CursorNodeVector> nodes;
  int head;
  int free;
  int maxSize;

 public:
  CursorSinglyLinkedListVector()
      : nodes(5, CursorNodeVector()), head(-1), free(0), maxSize(5) {
    for (int i = 0; i < maxSize - 1; ++i) {
      nodes[i].next = i + 1;
    }
    nodes[maxSize - 1].next = -1;
  }

  void resize() {
    int oldSize = maxSize;
    maxSize *= 2;
    nodes.resize(maxSize, CursorNodeVector());

    for (int i = oldSize; i < maxSize - 1; ++i) {
      nodes[i].next = i + 1;
    }
    nodes[maxSize - 1].next = -1;

    if (free == -1) {
      free = oldSize;
    } else {
      int current = free;
      while (nodes[current].next != -1) {
        current = nodes[current].next;
      }
      nodes[current].next = oldSize;
    }
  }

  void insert(int key) {
    if (free == -1) {
      resize();
    }

    int newNode = free;
    free = nodes[free].next;

    nodes[newNode].key = key;
    nodes[newNode].next = head;
    head = newNode;
  }

  bool contains(int key) const {
    int current = head;
    while (current != -1) {
      if (nodes[current].key == key) return true;
      current = nodes[current].next;
    }
    return false;
  }

  void remove(int key) {
    int current = head;
    int prev = -1;

    while (current != -1) {
      if (nodes[current].key == key) {
        if (prev != -1) {
          nodes[prev].next = nodes[current].next;
        } else {
          head = nodes[current].next;
        }

        nodes[current].next = free;
        free = current;
        return;
      }
      prev = current;
      current = nodes[current].next;
    }
  }

  ~CursorSinglyLinkedListVector() {
    nodes.clear();
  }

  void printList() {
    int current = head;
    while (current != -1) {
      std::cout << nodes[current].key << " ";
      current = nodes[current].next;
    }
    std::cout << std::endl;
  }
};