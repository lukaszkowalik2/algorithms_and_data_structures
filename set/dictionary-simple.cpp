#include "headers/dictionary-simple.hpp"

#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
  std::cout << "===== Testowanie klasy DictionarySimple =====" << std::endl;

  std::cout << "\n[Test 1] Dodawanie elementów... ";
  DictionarySimple dict(5);
  dict.insert(std::string(50, 'a'));
  dict.insert(std::string(50, 'b'));
  bool test1_ok = dict.contains(std::string(50, 'a')) && dict.contains(std::string(50, 'b')) && !dict.contains(std::string(50, 'c'));
  std::cout << (test1_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 2] Usuwanie elementu... ";
  dict.remove(std::string(50, 'a'));
  bool test2_ok = !dict.contains(std::string(50, 'a'));
  std::cout << (test2_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "[Test 3] Nieprawidłowa długość ciągu... ";
  bool test3_ok = true;
  try {
    dict.insert("test");
    test3_ok = false;
  } catch (const std::invalid_argument &) {
    test3_ok = true;
  }
  std::cout << (test3_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "\n===== Koniec testów =====" << std::endl;
  return 0;
}