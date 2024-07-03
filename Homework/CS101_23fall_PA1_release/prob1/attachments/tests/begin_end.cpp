#include "../slist.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

int main() {
  Slist<int> nums{1, 2, 4, 8, 16};
  Slist<std::string> fruits{"orange", "apple", "raspberry"};
  Slist<char> empty;

  // Print Slist.
  std::for_each(nums.begin(), nums.end(),
                [](const int n) { std::cout << n << ' '; });
  std::cout << '\n';

  // Sums all integers in the Slist nums (if any), printing only the result.
  std::cout << "Sum of nums: " << std::accumulate(nums.begin(), nums.end(), 0)
            << '\n';

  // Prints the first fruit in the Slist fruits, checking if there is any.
  if (!fruits.empty())
    std::cout << "First fruit: " << *fruits.begin() << '\n';

  if (empty.begin() == empty.end())
    std::cout << "Slist 'empty' is indeed empty.\n";
}