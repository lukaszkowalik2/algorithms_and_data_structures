#ifndef SET_LINKED_HPP
#define SET_LINKED_HPP

#include <iostream>
#include <vector>

struct Node {
  int value;
  Node *next;

  Node(int val) : value(val), next(nullptr) {}
  Node(int val, Node *next) : value(val), next(next) {}
};

class SetLinked {
  private:
  Node *head;

  void clear(Node *current) {
    if (!current)
      return;
    clear(current->next);
    delete current;
  }

  Node *insert(int val, Node *current) {
    if ((!current || current->value > val)) {
      Node *new_node = new Node(val);
      new_node->next = current;
      return new_node;
    }
    if (current->value == val)
      return current;
    current->next = insert(val, current->next);
    return current;
  }

  Node *remove(int val, Node *current) {
    if (!current)
      return nullptr;
    if (current->value == val) {
      Node *temp = current;
      current = current->next;
      delete temp;
      return current;
    }
    current->next = remove(val, current->next);
    return current;
  }

  bool contains(int val, Node *current) const {
    if (!current)
      return false;
    if (current->value == val)
      return true;
    return contains(val, current->next);
  }

  Node *copyRecursive(Node *current) const {
    if (!current)
      return nullptr;
    return new Node(current->value, copyRecursive(current->next));
  }

  Node *unionRecursive(Node *current1, Node *current2) const {
    if (!current1)
      return copyRecursive(current2);
    if (!current2)
      return copyRecursive(current1);
    if (current1->value < current2->value)
      return new Node(current1->value, unionRecursive(current1->next, current2));
    if (current2->value < current1->value)
      return new Node(current2->value, unionRecursive(current1, current2->next));
    return new Node(current1->value, unionRecursive(current1->next, current2->next));
  }

  Node *intersectionRecursive(Node *current1, Node *current2) const {
    if (!current1 || !current2)
      return nullptr;
    if (current1->value < current2->value)
      return intersectionRecursive(current1->next, current2);
    if (current2->value < current1->value)
      return intersectionRecursive(current1, current2->next);
    return new Node(current1->value, intersectionRecursive(current1->next, current2->next));
  }

  Node *differenceRecursive(Node *current1, Node *current2) const {
    if (!current1)
      return nullptr;
    if (!current2)
      return copyRecursive(current1);
    if (current1->value < current2->value)
      return new Node(current1->value, differenceRecursive(current1->next, current2));
    if (current1->value > current2->value)
      return differenceRecursive(current1, current2->next);
    return differenceRecursive(current1->next, current2->next);
  }

  bool equalRecursive(Node *current1, Node *current2) const {
    if (!current1 && !current2)
      return true;
    if (!current1 || !current2)
      return false;
    return (current1->value == current2->value) && equalRecursive(current1->next, current2->next);
  }

  public:
  SetLinked() : head(nullptr) {}
  SetLinked(const SetLinked &other) : head(copyRecursive(other.head)) {}

  void insert(int val) {
    if (val < 0)
      return;
    head = insert(val, head);
  }

  void remove(int val) {
    head = remove(val, head);
  }

  bool contains(int val) const {
    return contains(val, head);
  }

  SetLinked &operator=(const SetLinked &other) {
    if (this != &other) {
      clear(head);
      head = copyRecursive(other.head);
    }
    return *this;
  }

  bool operator==(const SetLinked &other) const {
    return equalRecursive(head, other.head);
  }

  SetLinked operator+(const SetLinked &other) const {
    SetLinked result;
    result.head = unionRecursive(head, other.head);
    return result;
  }

  SetLinked operator*(const SetLinked &other) const {
    SetLinked result;
    result.head = intersectionRecursive(head, other.head);
    return result;
  }

  SetLinked operator-(const SetLinked &other) const {
    SetLinked result;
    result.head = differenceRecursive(head, other.head);
    return result;
  }

  ~SetLinked() {
    clear(head);
  }
};

#endif // SET_LINKED_HPP
