#include <iostream>
#include <vector>
#include <ranges>
#include <thread>

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

  static const auto num_threads = std::thread::hardware_concurrency() - 1;

  std::vector results(num_threads, std::vector(n + m - 1, 0));

  {
    std::vector<std::jthread> th;
    th.reserve(num_threads);

    auto work = [&a, &b, m](std::vector<int> &result, std::size_t i,
                            std::size_t i_end) {
      while (i != i_end) {
        for (std::size_t j = 0; j != m; ++j)
          result[i + j] += a[i] * b[j];
        ++i;
      }
    };

    const auto each_size = n / num_threads;

    for (auto i = 0u; i != num_threads; ++i)
      th.emplace_back(work, std::ref(results[i]), each_size * i,
                      i == num_threads - 1 ? n : each_size * (i + 1));
  }

  std::vector result(n + m - 1, 0);

  for (const auto &r : results)
    for (std::size_t i = 0; i != r.size(); ++i)
      result[i] += r[i];

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