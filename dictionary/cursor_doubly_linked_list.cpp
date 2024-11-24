#include <iostream>

struct CursorDoubleNode {
  int key;
  int next;
  int prev;

  CursorDoubleNode(int k = 0, int n = -1, int p = -1)
      : key(k), next(n), prev(p) {
  }
};

class CursorDoublyLinkedList {
 private:
  CursorDoubleNode* nodes;
  int head;
  int free;
  int maxSize;

  void expand() {
    int oldSize = maxSize;
    maxSize *= 2;

    CursorDoubleNode* newNodes = new CursorDoubleNode[maxSize];
    for (int i = 0; i < oldSize; i++) {
      newNodes[i] = nodes[i];
    }
    for (int i = oldSize; i < maxSize - 1; ++i) {
      newNodes[i].next = i + 1;
    }
    newNodes[maxSize - 1].next = -1;

    delete[] nodes;
    nodes = newNodes;
    free = oldSize;
  }

 public:
  CursorDoublyLinkedList(int size = 5)
      : head(-1), free(0), maxSize(size) {
    nodes = new CursorDoubleNode[size];
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
    nodes[newNode].prev = -1;

    if (head != -1) {
      nodes[head].prev = newNode;
    }

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

    while (current != -1) {
      if (nodes[current].key == key) {
        if (nodes[current].prev != -1) {
          nodes[nodes[current].prev].next = nodes[current].next;
        }
        if (nodes[current].next != -1) {
          nodes[nodes[current].next].prev = nodes[current].prev;
        }
        if (current == head) {
          head = nodes[current].next;
        }

        nodes[current].next = free;
        free = current;
        return;
      }
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

  ~CursorDoublyLinkedList() {
    delete[] nodes;
  }
};
