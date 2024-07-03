#include <iostream>
#include <vector>
#include <ranges>

void input(std::vector<int> &vec, std::size_t length) {
  vec.reserve(length);
  while (length--) {
    int x;
    std::cin >> x;
    vec.push_back(x);
  }
}

std::vector<int> mul(const std::vector<int> &a, const std::vector<int> &b) {
  auto n = a.size(), m = b.size();
  std::vector<int> result(n + m - 1);
  for (std::size_t i = 0; i != n; ++i)
    for (std::size_t j = 0; j != m; ++j)
      result[i + j] += a[i] * b[j];
  return result;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::size_t a_len, b_len;
  std::cin >> a_len >> b_len;
  std::vector<int> a, b;
  input(a, a_len);
  input(b, b_len);
  auto result = mul(a, b);
  for (auto x : result)
    std::cout << x << ' ';
  std::cout << '\n';
  return 0;
}