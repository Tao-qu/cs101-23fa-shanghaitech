#include <iostream>
#include <vector>

auto random_choice(auto begin, auto end)
{
  // This is only a possible implementation of random_choice.
  // You can use this for your convenience or design a better one.
  std::size_t length = end - begin;
  std::size_t idx = std::rand() % length;
  return begin + idx;
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end, auto compare)
{
  // TODO: IMPLEMENT THIS.
  if (end - begin > 1)
  {
    auto pivot = random_choice(begin, end);
    std::iter_swap(pivot, end - 1);
    auto i = begin - 1;
    for (auto j = begin; j < end - 1; ++j)
    {
      if (compare(*j, *(end - 1)))
      {
        ++i;
        std::iter_swap(i, j);
      }
    }
    std::iter_swap(i + 1, end - 1);
    quick_sort(begin, i + 1, compare);
    quick_sort(i + 2, end, compare);
  }
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end)
{
  quick_sort(begin, end, std::less<>{});
}
