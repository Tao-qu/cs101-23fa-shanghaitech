#include <iostream>
#include <vector>
#include <ranges>

inline constexpr std::size_t operator""_zu(unsigned long long x) {
  return x;
}

void input(std::vector<int> &vec, std::size_t length) {
  vec.reserve(length);
  while (length--) {
    int x;
    std::cin >> x;
    vec.push_back(x);
  }
}

void fill_zero(std::vector<int> &a, std::vector<int> &b) {
  auto n = std::max(a.size(), b.size());
  a.resize(n);
  b.resize(n);
}

auto plus(const std::vector<int> &a, const std::vector<int> &b) {
  auto result = a;
  if (b.size() > a.size())
    result.resize(b.size());
  for (auto i = 0_zu; i != b.size(); ++i)
    result[i] += b[i];
  return result;
}

std::vector<int> mul(const std::vector<int> &a, const std::vector<int> &b) {
  auto n = a.size();
  if (n <= 100) {
    std::vector result(2 * n - 1, 0);
    for (auto i = 0_zu; i != n; ++i)
      for (auto j = 0_zu; j != n; ++j)
        result[i + j] += a[i] * b[j];
    return result;
  }
  auto half = n / 2;
  std::vector a_low(a.begin(), a.begin() + half),
      a_high(a.begin() + half, a.end()), b_low(b.begin(), b.begin() + half),
      b_high(b.begin() + half, b.end());
  auto z0 = mul(a_low, b_low);
  auto z1 = mul(plus(a_low, a_high), plus(b_low, b_high));
  auto z2 = mul(a_high, b_high);
  std::vector result(2 * n - 1, 0);
  for (auto i = 0_zu; i != z0.size(); ++i)
    result[i] += z0[i];
  for (auto i = 0_zu; i != z1.size(); ++i)
    result[i + half] += z1[i];
  for (auto i = 0_zu; i != z0.size(); ++i)
    result[i + half] -= z0[i];
  for (auto i = 0_zu; i != z2.size(); ++i)
    result[i + half] -= z2[i];
  for (auto i = 0_zu; i != z2.size(); ++i)
    result[i + half * 2] += z2[i];
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
  fill_zero(a, b);
  auto result = mul(a, b);
  for (auto x : result | std::views::take(a_len + b_len - 1))
    std::cout << x << ' ';
  std::cout << '\n';
  return 0;
}