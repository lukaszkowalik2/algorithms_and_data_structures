#include <stdexcept>

#include "simple-mapping.h"

size_t mapIntRangeSeq(int x, int n, int m) {
  if (x < n || x > m) {
    throw std::out_of_range("Input integer out of range [n, m]");
  }
  return static_cast<size_t>(x - n);
}

size_t mapIntRangeStep2(int x, int n, int m) {
  if (x < n || x > m) {
    throw std::out_of_range("Input integer out of range [n, m]");
  }
  if ((x - n) % 2 != 0) {
    throw std::invalid_argument("Input integer does not match step sequence");
  }
  return static_cast<size_t>((x - n) / 2);
}

size_t mapCharAz(char c) {
  if (c < 'a' || c > 'z') {
    throw std::out_of_range("Input character out of range [a, z]");
  }
  return static_cast<size_t>(c - 'a');
}

size_t mapStringAz2(const std::string &s) {
  if (s.length() != 2) {
    throw std::invalid_argument("Input string must have length 2");
  }
  size_t val1, val2;
  try {
    val1 = mapCharAz(s[0]);
    val2 = mapCharAz(s[1]);
  } catch (const std::out_of_range &e) {
    throw std::invalid_argument("String contains characters out of range [a, z]");
  }
  return val1 * 26 + val2;
}
