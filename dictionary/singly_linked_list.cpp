#include <iostream>

struct Node {
  int key;
  Node* next;

  Node(int _key)
      : key(_key), next(nullptr) {
  }
};

class SinglyLinkedList {
 private:
  Node* head;

 public:
  SinglyLinkedList()
      : head(nullptr) {
  }

  void insert(int key) {
    Node* newNode = new Node(key);
    newNode->next = head;
    head = newNode;
  }

  bool contains(int key) {
    Node* current = head;
    while (current) {
      if (current->key == key) return true;
      current = current->next;
    }
    return false;
  }

  void remove(int key) {
    Node* current = head;
    Node* prev = nullptr;

    while (current) {
      if (current->key == key) {
        if (prev) {
          prev->next = current->next;
        } else {
          head = current->next;
        }
        delete current;
        return;
      }
      prev = current;
      current = current->next;
    }
  }

  ~SinglyLinkedList() {
    while (head) {
      Node* temp = head;
      head = head->next;
      delete temp;
    }
  }

  void printList() {
    Node* current = head;
    while (current) {
      std::cout << current->key << " ";
      current = current->next;
    }
    std::cout << std::endl;
  }
};
