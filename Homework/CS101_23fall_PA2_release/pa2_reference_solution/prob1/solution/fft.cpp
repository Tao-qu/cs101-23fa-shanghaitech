#include <complex>
#include <vector>
#include <iostream>
#include <numbers>
#include <cmath>

using complex = std::complex<double>;
constexpr auto pi = std::numbers::pi;

void input(std::vector<complex> &vec, std::size_t length) {
  vec.reserve(length);
  while (length--) {
    int x;
    std::cin >> x;
    vec.push_back(x);
  }
}

auto calculate_n(std::size_t n, std::size_t m) {
  auto larger = std::max(n, m);
  std::size_t ret = 1;
  while (ret < larger)
    ret *= 2;
  return ret;
}

void fft(std::vector<complex> &coeffs) {
  auto n = coeffs.size() / 2;
  if (n == 0)
    return;
  std::vector<complex> even, odd;
  even.reserve(n);
  odd.reserve(n);
  for (std::size_t i = 0; i != 2 * n; i += 2) {
    even.emplace_back(coeffs[i]);
    odd.emplace_back(coeffs[i + 1]);
  }
  fft(even);
  fft(odd);
  auto omega = std::polar(1.0, pi / n);
  complex omega_k = 1;
  for (std::size_t k = 0; k != n; ++k, omega_k *= omega) {
    coeffs[k] = even[k] + omega_k * odd[k];
    coeffs[k + n] = even[k] - omega_k * odd[k];
  }
}

int main() {
  std::size_t a_len, b_len;
  std::cin >> a_len >> b_len;
  std::vector<complex> a, b, c;
  input(a, a_len);
  input(b, b_len);
  auto n = calculate_n(a_len, b_len);
  a.resize(2 * n);
  b.resize(2 * n);
  fft(a);
  fft(b);
  c.reserve(2 * n);
  for (std::size_t i = 0; i != 2 * n; ++i)
    c.push_back(a[i] * b[i]);
  fft(c);
  for (std::size_t k = 0; k != a_len + b_len - 1; ++k)
    std::cout << std::lround(c[(2 * n - k) % (2 * n)].real() / (2 * n)) << ' ';
  std::cout << '\n';
  return 0;
}