#include "../slist.hpp"

#include <functional>
#include <iostream>

struct IntWithIndex {
  int value;
  std::size_t index;

  IntWithIndex(int v) : value{v} {}

  auto operator<=>(const IntWithIndex &other) const {
    return value <=> other.value;
  }
};

std::ostream &operator<<(std::ostream &ostr, const IntWithIndex &i) {
  return ostr << "(" << i.value << ", " << i.index << ")";
}

template <typename T>
std::ostream &operator<<(std::ostream &ostr, const Slist<T> &list) {
  for (const auto &i : list)
    ostr << ' ' << i;
  return ostr;
}

int main() {
  Slist<int> list{8, 7, 5, 9, 0, 1, 3, 2, 6, 4};
  std::cout << "initially: " << list << '\n';

  list.sort();
  std::cout << "ascending: " << list << '\n';

  list.sort(std::greater<>{});
  std::cout << "descending:" << list << '\n';

  std::cout << "Test stability:\n";

  Slist<IntWithIndex> l2(10, 0);
  for (std::size_t i = 0; auto &[_, index] : l2)
    index = i++;
  std::cout << "initially: " << l2 << '\n';

  l2.sort(std::greater<>{});
  std::cout << "descending:" << l2 << '\n';

  l2.sort();
  std::cout << "ascending: " << l2 << '\n';
}