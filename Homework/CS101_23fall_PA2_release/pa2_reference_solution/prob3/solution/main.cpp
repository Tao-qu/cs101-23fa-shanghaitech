#include <iostream>
#include <vector>
#include <map>

#include "huffman_calculator.hpp"

void expect(const bool a) {
  if (!a)
    std::cout << "Incorrect!" << std::endl;
}

using input_type = std::vector<std::pair<std::size_t, std::size_t>>;
using return_type = std::size_t;
std::map<int, std::pair<input_type, return_type>> testcases = {
    {1, {{{1, 6}, {2, 3}, {5, 4}, {6, 2}, {9, 1}, {100, 1}}, 347}},
    {2, {{{7, 1}, {3, 2}, {2, 1}, {1, 2}}, 39}},
    {3, {{{9, 1}, {3, 2}, {1, 2}, {2, 1}}, 41}},
    {4, {{{11, 6}, {32, 3}, {5, 10}, {6, 200}, {9, 1}, {100, 1}}, 11469}},
    {5, {{{1, 142857}}, 2452139}},
};

#define TRIVIAL_TEST(N)                                                        \
  void test_##N() {                                                            \
    expect(get_huffman_length(testcases[N].first) == testcases[N].second);     \
  }

TRIVIAL_TEST(1)
TRIVIAL_TEST(2)
TRIVIAL_TEST(3)
TRIVIAL_TEST(4)
TRIVIAL_TEST(5)

#undef TRIVIAL_TEST

void test_6() {
  input_type data{};
  for (int i = 1; i < 500; ++i) {
    data.emplace_back(i, i);
  }
  expect(get_huffman_length(data) == 700610678);
  std::cout << get_huffman_length(data) << std::endl;
}

void test_7() {
  input_type data{};
  for (int i = 1; i < 500; ++i) {
    data.emplace_back(i, 600 - i);
  }
  expect(get_huffman_length(data) == 568787249);
  std::cout << get_huffman_length(data) << std::endl;
}

void test_8() {
  input_type data{};
  for (int i = 1; i < 4000; ++i) {
    data.emplace_back(i, 10000);
  }
  expect(get_huffman_length(data) == 2000379808282);
  std::cout << get_huffman_length(data) << std::endl;
}

void test_9() {
  input_type data{};
  for (int i = 870001; i < 874000; ++i) {
    data.emplace_back(i, 10000);
  }
  expect(get_huffman_length(data) == 882988178063168);
  std::cout << get_huffman_length(data) << std::endl;
}

void test_10() {
  input_type data{};
  for (int i = 2462001; i < 2466000; ++i) {
    data.emplace_back(i, 10000);
  }
  expect(get_huffman_length(data) == 2495081026575168);
  std::cout << get_huffman_length(data) << std::endl;
}

int main() {
  test_1(); std::cout << 1 << std::endl;
  test_2(); std::cout << 2 << std::endl;
  test_3(); std::cout << 3 << std::endl;
  test_4(); std::cout << 4 << std::endl;
  test_5(); std::cout << 5 << std::endl;
  test_6(); std::cout << 6 << std::endl;
  test_7(); std::cout << 7 << std::endl;
  test_8(); std::cout << 8 << std::endl;
  test_9(); std::cout << 9 << std::endl;
  test_10(); std::cout << 10 << std::endl;
  return 0;
}
