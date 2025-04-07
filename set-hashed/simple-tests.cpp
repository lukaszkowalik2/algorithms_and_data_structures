#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "simple-mapping.h"
#include "utilities.h"

int main() {
  std::cout << "--- Running Simple Mapping Function Tests ---\n";
  int totalTests = 0;
  int passedTests = 0;

  std::cout << "\nTesting mapIntRangeSeq...\n";
  testResult(mapIntRangeSeq(5, 5, 10) == 0, "mapIntRangeSeq(5, 5, 10)");
  passedTests++;
  testResult(mapIntRangeSeq(7, 5, 10) == 2, "mapIntRangeSeq(7, 5, 10)");
  passedTests++;
  testResult(mapIntRangeSeq(10, 5, 10) == 5, "mapIntRangeSeq(10, 5, 10)");
  passedTests++;
  try {
    mapIntRangeSeq(4, 5, 10);
    testResult(false, "mapIntRangeSeq out of range low");
  } catch (const std::out_of_range &) {
    testResult(true, "mapIntRangeSeq out of range low");
    passedTests++;
  }
  try {
    mapIntRangeSeq(11, 5, 10);
    testResult(false, "mapIntRangeSeq out of range high");
  } catch (const std::out_of_range &) {
    testResult(true, "mapIntRangeSeq out of range high");
    passedTests++;
  }
  totalTests += 5;
  std::cout << "mapIntRangeSeq: " << passedTests << "/" << totalTests << " tests passed\n";

  std::cout << "\nTesting mapIntRangeStep2...\n";
  int prevPassed = passedTests;
  testResult(mapIntRangeStep2(10, 10, 20) == 0, "mapIntRangeStep2(10, 10, 20)");
  passedTests++;
  testResult(mapIntRangeStep2(14, 10, 20) == 2, "mapIntRangeStep2(14, 10, 20)");
  passedTests++;
  testResult(mapIntRangeStep2(20, 10, 20) == 5, "mapIntRangeStep2(20, 10, 20)");
  passedTests++;
  try {
    mapIntRangeStep2(9, 10, 20);
    testResult(false, "mapIntRangeStep2 out of range low");
  } catch (const std::out_of_range &) {
    testResult(true, "mapIntRangeStep2 out of range low");
    passedTests++;
  }
  try {
    mapIntRangeStep2(21, 10, 20);
    testResult(false, "mapIntRangeStep2 out of range high");
  } catch (const std::out_of_range &) {
    testResult(true, "mapIntRangeStep2 out of range high");
    passedTests++;
  }
  try {
    mapIntRangeStep2(11, 10, 20);
    testResult(false, "mapIntRangeStep2 odd step");
  } catch (const std::invalid_argument &) {
    testResult(true, "mapIntRangeStep2 odd step");
    passedTests++;
  }
  totalTests += 6;
  std::cout << "mapIntRangeStep2: " << (passedTests - prevPassed) << "/6 tests passed\n";

  std::cout << "\nTesting mapCharAz...\n";
  prevPassed = passedTests;
  testResult(mapCharAz('a') == 0, "mapCharAz('a')");
  passedTests++;
  testResult(mapCharAz('c') == 2, "mapCharAz('c')");
  passedTests++;
  testResult(mapCharAz('z') == 25, "mapCharAz('z')");
  passedTests++;
  try {
    mapCharAz('A');
    testResult(false, "mapCharAz uppercase");
  } catch (const std::out_of_range &) {
    testResult(true, "mapCharAz uppercase");
    passedTests++;
  }
  try {
    mapCharAz(' ');
    testResult(false, "mapCharAz space");
  } catch (const std::out_of_range &) {
    testResult(true, "mapCharAz space");
    passedTests++;
  }
  totalTests += 5;
  std::cout << "mapCharAz: " << (passedTests - prevPassed) << "/5 tests passed\n";

  std::cout << "\nTesting mapStringAz2...\n";
  prevPassed = passedTests;
  testResult(mapStringAz2("aa") == 0, "mapStringAz2('aa')");
  passedTests++;
  testResult(mapStringAz2("ac") == 2, "mapStringAz2('ac')");
  passedTests++;
  testResult(mapStringAz2("az") == 25, "mapStringAz2('az')");
  passedTests++;
  testResult(mapStringAz2("ba") == 26, "mapStringAz2('ba')");
  passedTests++;
  testResult(mapStringAz2("bc") == 28, "mapStringAz2('bc')");
  passedTests++;
  testResult(mapStringAz2("zz") == 675, "mapStringAz2('zz')");
  passedTests++; // 25*26 + 25
  try {
    mapStringAz2("a");
    testResult(false, "mapStringAz2 single char");
  } catch (const std::invalid_argument &) {
    testResult(true, "mapStringAz2 single char");
    passedTests++;
  }
  try {
    mapStringAz2("abc");
    testResult(false, "mapStringAz2 three chars");
  } catch (const std::invalid_argument &) {
    testResult(true, "mapStringAz2 three chars");
    passedTests++;
  }
  try {
    mapStringAz2("aA");
    testResult(false, "mapStringAz2 uppercase");
  } catch (const std::invalid_argument &) {
    testResult(true, "mapStringAz2 uppercase");
    passedTests++;
  }
  try {
    mapStringAz2(" a");
    testResult(false, "mapStringAz2 space");
  } catch (const std::invalid_argument &) {
    testResult(true, "mapStringAz2 space");
    passedTests++;
  }
  totalTests += 10;
  std::cout << "mapStringAz2: " << (passedTests - prevPassed) << "/10 tests passed\n";

  std::cout << "\n--- Simple Mapping Tests Complete: " << passedTests << "/" << totalTests << " tests passed ---\n\n";
  return 0;
}
