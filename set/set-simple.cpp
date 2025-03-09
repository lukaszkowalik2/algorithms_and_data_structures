#include <iostream>
#include <stdexcept>
#include <vector>

class SetSimple {
  private:
  std::vector<bool> elements;
  size_t n;

  public:
  SetSimple(int N) : n(N), elements(N, false) {}

  void insert(int x) {
    if (x >= 0 && x < n)
      elements[x] = true;
  }

  void remove(int x) {
    if (x >= 0 && x < n)
      elements[x] = false;
  }

  bool contains(int x) const {
    return (x >= 0 && x < n) && elements[x];
  }

  size_t size() const {
    return n;
  }

  SetSimple operator+(const SetSimple &other) const {
    size_t new_size = std::max(n, other.n);
    SetSimple result(new_size);

    for (size_t i = 0; i < new_size; ++i) {
      bool in_this = (i < n) ? elements[i] : false;
      bool in_other = (i < other.n) ? other.elements[i] : false;
      result.elements[i] = in_this || in_other;
    }
    return result;
  }

  SetSimple operator*(const SetSimple &other) const {
    size_t new_size = std::min(n, other.n);
    SetSimple result(new_size);

    for (size_t i = 0; i < new_size; ++i) {
      result.elements[i] = elements[i] && other.elements[i];
    }
    return result;
  }

  SetSimple operator-(const SetSimple &other) const {
    size_t new_size = n;
    SetSimple result(new_size);

    for (size_t i = 0; i < new_size; ++i) {
      bool in_other = (i < other.n) ? other.elements[i] : false;
      result.elements[i] = elements[i] && !in_other;
    }
    return result;
  }

  bool operator==(const SetSimple &other) const {
    size_t max_size = std::max(n, other.n);

    for (size_t i = 0; i < max_size; ++i) {
      bool in_this = (i < n) ? elements[i] : false;
      bool in_other = (i < other.n) ? other.elements[i] : false;
      if (in_this != in_other)
        return false;
    }
    return true;
  }
};

int main() {
  std::cout << "===== Testowanie klasy SetSimple =====" << std::endl;

  std::cout << "\n[Test 1] Dodawanie elementów... ";
  SetSimple s1(5);
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
  SetSimple A(5), B(5);
  A.insert(1);
  B.insert(2);
  SetSimple C = A + B;
  bool test3_ok = C.contains(1) && C.contains(2);
  std::cout << (test3_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 4] Część wspólna... ";
  A.insert(2);
  SetSimple D = A * B;
  bool test4_ok = D.contains(2) && !D.contains(1);
  std::cout << (test4_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 5] Różnica zbiorów... ";
  SetSimple E = A - B;
  bool test5_ok = E.contains(1) && !E.contains(2);
  std::cout << (test5_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 6] Porównywanie zbiorów... ";
  SetSimple X(3), Y(3);
  X.insert(0);
  Y.insert(0);
  bool test6_ok = (X == Y);
  Y.insert(1);
  test6_ok = test6_ok && !(X == Y);
  std::cout << (test6_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 7] Nieprawidłowe indeksy... ";
  SetSimple s2(3);
  s2.insert(-1);
  s2.insert(10);
  bool test7_ok = !s2.contains(-1) && !s2.contains(10);
  std::cout << (test7_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 8] Różne rozmiary zbiorów... ";
  SetSimple Z(5), W(10);
  Z.insert(0);
  W.insert(0);
  W.insert(7);
  SetSimple Result = Z + W;
  bool test8_ok = Result.contains(0) && Result.contains(7);
  std::cout << (test8_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 9] Skrajny przypadek (N=1)... ";
  SetSimple s3(1);
  s3.insert(0);
  bool test9_ok = s3.contains(0);
  s3.remove(0);
  test9_ok = test9_ok && !s3.contains(0);
  std::cout << (test9_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "\n===== Koniec testów =====" << std::endl;
  return 0;
}