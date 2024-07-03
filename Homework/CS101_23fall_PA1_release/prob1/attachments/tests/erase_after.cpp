#include "../slist.hpp"

#include <iostream>
#include <iterator>

int main() {
  Slist<int> l = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (auto n : l)
    std::cout << n << ' ';
  std::cout << '\n';

  auto fi = std::next(l.begin());
  auto la = std::next(fi, 3);

  l.erase_after(fi, la);

  for (auto n : l)
    std::cout << n << ' ';
  std::cout << '\n';

  l.erase_after(fi, l.end());

  for (auto n : l)
    std::cout << n << ' ';
  std::cout << '\n';
}