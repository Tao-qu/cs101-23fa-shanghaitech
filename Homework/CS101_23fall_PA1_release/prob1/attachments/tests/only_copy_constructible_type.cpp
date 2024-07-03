#include "../slist.hpp"

#include <iostream>
#include <string>
#include <vector>

struct OnlyCopyConstructibleType {
  std::string member1;
  const std::vector<int> member2;

  OnlyCopyConstructibleType() = delete;
  OnlyCopyConstructibleType(const OnlyCopyConstructibleType &) = default;
  ~OnlyCopyConstructibleType() = default;

  OnlyCopyConstructibleType(const std::string &m1, const std::vector<int> &m2)
      : member1(m1), member2(m2) {}

  auto operator<=>(const OnlyCopyConstructibleType &) const = default;
  bool operator==(const OnlyCopyConstructibleType &) const = default;
};

int main() {
  std::string s{"hello"};
  std::vector<int> v{1, 2, 3};

  Slist list(3, OnlyCopyConstructibleType(s, v));
  list.emplace_front(s, v);
  list.emplace_after(list.begin(), s, v);
  list.insert_after(list.cbegin(), OnlyCopyConstructibleType(s, v)); // copy, not move
  list.pop_front();
  list.erase_after(list.begin());

  Slist list2(list);

  list.sort();
  list.merge(list2);

  std::cout << "`list2' should be empty: list2.empty() == " << std::boolalpha
            << list2.empty() << '\n';
  bool wrong = false;
  for (const auto &elem : list) {
    if (elem != OnlyCopyConstructibleType(s, v)) {
      std::cout << "The elements are incorrect!\n";
      wrong = true;
      break;
    }
  }
  if (!wrong)
    std::cout << "The elements are correct.\n";
  return 0;
}