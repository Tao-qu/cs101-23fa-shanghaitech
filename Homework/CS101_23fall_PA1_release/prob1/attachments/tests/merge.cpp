#include "../slist.hpp"

#include <iostream>

std::ostream &operator<<(std::ostream &ostr, const Slist<int> &list)
{
  for (const int i : list)
    ostr << ' ' << i;
  return ostr;
}

int main()
{
  Slist<int> list1 = {5, 9, 1, 3, 3};
  Slist<int> list2 = {8, 7, 2, 3, 4, 4};

  list1.sort();
  list2.sort();

  std::cout << "list1: " << list1 << '\n';
  std::cout << "list2: " << list2 << '\n';

  list1.merge(list2);
  std::cout << "merged:" << list1 << '\n';

  list1.merge(list1); // This should do nothing.
  std::cout << list1 << '\n';
}