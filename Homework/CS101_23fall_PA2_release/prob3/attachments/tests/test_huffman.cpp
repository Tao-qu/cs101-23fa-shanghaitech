#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "../huffman_calculator.hpp"

using input_type = std::vector<std::pair<std::size_t, std::size_t>>;
using return_type = std::size_t;
std::map<int, std::pair<input_type, return_type>> testcases = {
    {1, {{{7, 1}, {3, 2}, {2, 1}, {1, 2}}, 39}},
    {2, {{{9, 1}, {3, 2}, {1, 2}, {2, 1}}, 41}},
    {3, {{{1, 142857}}, 2452139}},
};

int main() {
  for (const auto &item : testcases) {
    std::cout << "Local testcase: " << item.first
              << "  Your: " << get_huffman_length(item.second.first)
              << "  Answer: " << item.second.second << std::endl;
  }
  return 0;
}