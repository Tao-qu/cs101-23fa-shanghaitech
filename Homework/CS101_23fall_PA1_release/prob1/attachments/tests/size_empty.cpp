#include "../slist.hpp"

#include <iostream>

int main() {
  Slist<int> numbers;
  std::cout << std::boolalpha;
  std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n'
            << "numbers.size(): " << numbers.size() << '\n';

  numbers.push_front(42);
  numbers.push_front(13317);
  std::cout << "After adding elements, numbers.empty(): " << numbers.empty()
            << '\n'
            << "numbers.size(): " << numbers.size() << '\n';

  numbers.clear();
  std::cout << "After clearing, numbers.empty(): " << numbers.empty() << '\n'
            << "numbers.size(): " << numbers.size() << '\n';
}