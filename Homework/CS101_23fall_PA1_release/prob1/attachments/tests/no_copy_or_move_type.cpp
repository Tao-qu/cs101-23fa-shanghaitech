#include "../slist.hpp"

#include <iostream>
#include <string>
#include <vector>

struct NoCopyOrMoveType {
  std::string member1;
  const std::vector<int> member2;

  NoCopyOrMoveType() = delete;
  NoCopyOrMoveType(const NoCopyOrMoveType &) = delete;
  ~NoCopyOrMoveType() = default;

  NoCopyOrMoveType(const std::string &m1, const std::vector<int> &m2)
      : member1(m1), member2(m2) {}

  auto operator<=>(const NoCopyOrMoveType &) const = default;
  bool operator==(const NoCopyOrMoveType &) const = default;
};

int main() {
  std::string s{"hello"};
  std::vector<int> v{1, 2, 3};

  Slist<NoCopyOrMoveType> list;
  list.emplace_front(s, v);
  list.emplace_front(s, v);
  list.emplace_after(list.begin(), s, v);
  list.pop_front();
  list.erase_after(list.begin());

  Slist<NoCopyOrMoveType> list2;
  for (auto i = 0; i != 3; ++i)
    list2.emplace_front(s, v);

  list.sort();
  list.merge(list2);

  std::cout << "`list2' should be empty: list2.empty() == " << std::boolalpha
            << list2.empty() << '\n';
  bool wrong = false;
  for (const auto &elem : list) {
    if (elem != NoCopyOrMoveType(s, v)) {
      std::cout << "The elements are incorrect!\n";
      wrong = true;
      break;
    }
  }
  if (!wrong)
    std::cout << "The elements are correct.\n";
  return 0;
}