#include <iostream>
#include <vector>

auto random_choice(auto begin, auto end) {
  // This is only a possible implementation of random_choice.
  // You can use this for your convenience or design a better one.
  std::size_t length = end - begin;
  std::size_t idx = std::rand() % length;
  return begin + idx;
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end, auto compare) {
  if (begin == end || begin + 1 == end)
    return;
  std::iter_swap(random_choice(begin, end), end - 1);

  auto front_it = begin;
  auto back_it = end - 1;
  auto pivot = end - 1;

  while (front_it != back_it) {
    if (compare(*pivot, *front_it) && compare(*back_it, *pivot)) {
      std::iter_swap(front_it, back_it);
    } else if (!compare(*pivot, *front_it)) {
      ++front_it;
    } else if (!compare(*back_it, *pivot)) {
      --back_it;
    }
  }
  std::iter_swap(front_it, pivot);

  quick_sort(begin, front_it, compare);
  quick_sort(front_it, end, compare);
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end) {
  quick_sort(begin, end, std::less<>{});
}
