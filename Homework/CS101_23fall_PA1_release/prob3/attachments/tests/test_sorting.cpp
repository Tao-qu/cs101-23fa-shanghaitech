//
// Created by Suting Chen on 2023/10/20.
//

#include "../quick_sort.hpp"
#include <iostream>

class NoCopyConstructorClass {
public:
  int value{};
  explicit NoCopyConstructorClass(int value) : value(value){};
  NoCopyConstructorClass(const NoCopyConstructorClass &) = delete;
  NoCopyConstructorClass(NoCopyConstructorClass &&) = default;

  //  auto operator<=>(const NoCopyConstructorClass &other) const = default;
  bool operator==(const NoCopyConstructorClass &other) const = default;

  NoCopyConstructorClass &
  operator=(const NoCopyConstructorClass &other) = default;

  friend std::ostream &operator<<(std::ostream &os,
                                  const NoCopyConstructorClass &self) {
    os << self.value;
    return os;
  }

  struct Comparator {
    bool operator()(const NoCopyConstructorClass &i,
                    const NoCopyConstructorClass &j) {
      return i.value < j.value;
    }
  };
};

template <typename T>
void print_vector(const std::string &s, const std::vector<T> &v) {
  std::cout << s;
  for (const auto &item : v) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::vector origin = {1, 4, 2, 8, 5, 7, 1};

  std::vector ints(origin);
  print_vector("Before sorting:  ", ints);
  quick_sort(ints.begin(), ints.end());
  print_vector("Sort ascending:  ", ints);
  quick_sort(ints.begin(), ints.end(), std::greater<>{});
  print_vector("Sort descending: ", ints);

  std::vector<NoCopyConstructorClass> noCopyConstructorClass;
  noCopyConstructorClass.reserve(origin.size());
  for (const auto &item : origin) {
    noCopyConstructorClass.emplace_back(item);
  }

  print_vector("Before sorting:  ", noCopyConstructorClass);
  quick_sort(noCopyConstructorClass.begin(), noCopyConstructorClass.end(),
             NoCopyConstructorClass::Comparator());
  print_vector("After sorting:   ", noCopyConstructorClass);

  return 0;
}
