#include <iostream>

struct CursorNode {
  int key;
  int next;

  CursorNode(int k = 0, int n = -1)
      : key(k), next(n) {
  }
};

class CursorSinglyLinkedList {
 private:
  CursorNode* nodes;
  int head;
  int free;
  int maxSize;

  void expand() {
    int oldSize = maxSize;
    maxSize *= 2;

    CursorNode* newNodes = new CursorNode[maxSize];
    for (int i = 0; i < oldSize; i++) {
      newNodes[i] = nodes[i];
    }
    for (int i = oldSize; i < maxSize - 1; ++i) {
      newNodes[i].next = i + 1;
    }
    newNodes[maxSize - 1].next = -1;

    delete[] nodes;
    nodes = newNodes;

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

 public:
  CursorSinglyLinkedList(int size = 5)
      : head(-1), free(0), maxSize(size) {
    nodes = new CursorNode[size];
    for (int i = 0; i < size - 1; ++i) {
      nodes[i].next = i + 1;
    }
    nodes[size - 1].next = -1;
  }

  void insert(int key) {
    if (free == -1) {
      expand();
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

  void print() const {
    int current = head;
    while (current != -1) {
      std::cout << nodes[current].key << " ";
      current = nodes[current].next;
    }
    std::cout << "\n";
  }

  ~CursorSinglyLinkedList() {
    delete[] nodes;
  }
};
