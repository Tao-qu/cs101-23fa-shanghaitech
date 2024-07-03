#include "../slist.hpp"

#include <initializer_list>
#include <iostream>
#include <iterator>

void print(auto const comment, auto const &container) {
  auto size = std::ranges::distance(container);
  std::cout << comment << "{ ";
  for (auto const &element : container)
    std::cout << element << (--size ? ", " : " ");
  std::cout << "}\n";
}

int main() {
  Slist<int> x{1, 2, 3}, y, z;

  std::cout << "Initially:\n";
  print("x = ", x);
  print("y = ", y);
  print("z = ", z);

  std::cout << "Copy assignment copies data from x to y:\n";
  y = x;
  print("x = ", x);
  print("y = ", y);

  std::cout
      << "Move assignment moves data from x to z, modifying both x and z:\n";
  z = std::move(x);
  print("x = ", x);
  print("z = ", z);
}