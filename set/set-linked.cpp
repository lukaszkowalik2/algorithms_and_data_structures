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

int main() {
  std::cout << "===== Testowanie klasy SetLinked =====" << std::endl;

  std::cout << "\n[Test 1] Dodawanie elementów... ";
  SetLinked s1;
  s1.insert(0);
  s1.insert(3);
  s1.insert(4);
  bool test1_ok = s1.contains(0) && s1.contains(3) && s1.contains(4) && !s1.contains(1);
  std::cout << (test1_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 2] Usuwanie elementu... ";
  s1.remove(3);
  bool test2_ok = !s1.contains(3);
  std::cout << (test2_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 3] Suma zbiorów... ";
  SetLinked A, B;
  A.insert(1);
  B.insert(2);
  SetLinked C = A + B;
  bool test3_ok = C.contains(1) && C.contains(2);
  std::cout << (test3_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 4] Część wspólna... ";
  A.insert(2);
  SetLinked D = A * B;
  bool test4_ok = D.contains(2) && !D.contains(1);
  std::cout << (test4_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 5] Różnica zbiorów... ";
  SetLinked E = A - B;
  bool test5_ok = E.contains(1) && !E.contains(2);
  std::cout << (test5_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 6] Porównywanie zbiorów... ";
  SetLinked X, Y;
  X.insert(0);
  Y.insert(0);
  bool test6_ok = (X == Y);
  Y.insert(1);
  test6_ok = test6_ok && !(X == Y);
  std::cout << (test6_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 7] Nieprawidłowe indeksy... ";
  SetLinked s2;
  s2.insert(-1);
  s2.insert(10);
  bool test7_ok = !s2.contains(-1) && s2.contains(10);
  std::cout << (test7_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 8] Różne rozmiary zbiorów... ";
  SetLinked Z, W;
  Z.insert(0);
  W.insert(0);
  W.insert(7);
  SetLinked Result = Z + W;
  bool test8_ok = Result.contains(0) && Result.contains(7);
  std::cout << (test8_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 9] Skrajny przypadek (N=1)... ";
  SetLinked s3;
  s3.insert(0);
  bool test9_ok = s3.contains(0);
  s3.remove(0);
  test9_ok = test9_ok && !s3.contains(0);
  std::cout << (test9_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "\n===== Koniec testów =====" << std::endl;
  return 0;
}