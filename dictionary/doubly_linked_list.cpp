#include <iostream>

struct DoubleNode {
  int key;
  DoubleNode* next;
  DoubleNode* prev;

  DoubleNode(int _key)
      : key(_key), next(nullptr), prev(nullptr) {
  }
};

class DoublyLinkedList {
 private:
  DoubleNode* head;

 public:
  DoublyLinkedList()
      : head(nullptr) {
  }

  void insert(int key) {
    DoubleNode* newNode = new DoubleNode(key);
    newNode->next = head;
    if (head) head->prev = newNode;
    head = newNode;
  }

  bool contains(int key) {
    DoubleNode* current = head;
    while (current) {
      if (current->key == key) return true;
      current = current->next;
    }
    return false;
  }

  void remove(int key) {
    DoubleNode* current = head;

    while (current) {
      if (current->key == key) {
        if (current->prev) current->prev->next = current->next;
        if (current->next) current->next->prev = current->prev;
        if (current == head) head = current->next;
        delete current;
        return;
      }
      current = current->next;
    }
  }

  ~DoublyLinkedList() {
    while (head) {
      DoubleNode* temp = head;
      head = head->next;
      delete temp;
    }
  }

  void printList() {
    DoubleNode* current = head;
    while (current) {
      std::cout << current->key << " ";
      current = current->next;
    }
    std::cout << std::endl;
  }
};