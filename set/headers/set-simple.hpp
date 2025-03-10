#ifndef SET_SIMPLE_HPP
#define SET_SIMPLE_HPP

#include <vector>

class SetSimple {
  private:
  int n;
  std::vector<bool> elements;

  public:
  SetSimple(int N = 1000000) : n(N), elements(N, false) {}

  void insert(int x) {
    if (x >= 0 && x < n) {
      elements[x] = true;
    }
  }

  void remove(int x) {
    if (x >= 0 && x < n) {
      elements[x] = false;
    }
  }

  bool contains(int x) const {
    return (x >= 0 && x < n) ? elements[x] : false;
  }

  SetSimple operator+(const SetSimple &other) const {
    SetSimple result(n);
    for (int i = 0; i < n; i++) {
      result.elements[i] = elements[i] || other.elements[i];
    }
    return result;
  }

  SetSimple operator*(const SetSimple &other) const {
    SetSimple result(n);
    for (int i = 0; i < n; i++) {
      result.elements[i] = elements[i] && other.elements[i];
    }
    return result;
  }

  SetSimple operator-(const SetSimple &other) const {
    SetSimple result(n);
    for (int i = 0; i < n; i++) {
      result.elements[i] = elements[i] && !other.elements[i];
    }
    return result;
  }

  bool operator==(const SetSimple &other) const {
    for (int i = 0; i < n; i++) {
      if (elements[i] != other.elements[i]) {
        return false;
      }
    }
    return true;
  }
};

#endif // SET_SIMPLE_HPP
