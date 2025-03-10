
#include "headers/set-linked.hpp"

#include <iostream>
#include <vector>

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