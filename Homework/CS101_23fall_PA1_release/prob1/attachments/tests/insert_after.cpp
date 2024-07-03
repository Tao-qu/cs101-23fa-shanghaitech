#include "../slist.hpp"

#include <iostream>
#include <string>
#include <vector>

struct IntWrapper {
  int value;

  IntWrapper() = default;
  IntWrapper(int x) : value{x} {}
  IntWrapper(const IntWrapper &other) : value{other.value} {
    std::cout << "Copy constructor called.\n";
  }
  IntWrapper(IntWrapper &&other) noexcept : value{other.value} {
    std::cout << "Move constructor called.\n";
  }
};

void print(const Slist<IntWrapper> &list) {
  std::cout << "list: {";
  for (char comma[3] = {'\0', ' ', '\0'}; const auto &i : list) {
    std::cout << comma << i.value;
    comma[0] = ',';
  }
  std::cout << "}\n";
}

int main() {
  Slist<IntWrapper> ints{1, 2};
  print(ints);

  auto beginIt = ints.begin();
  IntWrapper six(6);
  ints.insert_after(beginIt, six); // copy
  print(ints);

  auto anotherIt = beginIt;
  ++anotherIt;
  ints.insert_after(anotherIt, std::move(six)); // move
  print(ints);
}