#include <cmath>
#include <complex>
#include <iostream>
#include <numbers>

constexpr std::size_t maxn = 6e5;
constexpr auto pi = std::numbers::pi;

std::complex<double> a[maxn], b[maxn];
int l, r[maxn];

void fft(std::complex<double> *A, int *r, int lim) {
  for (auto i = 0; i < lim; ++i)
    if (i < r[i])
      std::swap(A[i], A[r[i]]);
  for (auto mid = 1; mid < lim; mid *= 2) {
    auto wn = std::polar(1.0, pi / mid);
    for (auto R = mid * 2, j = 0; j < lim; j += R) {
      std::complex<double> w = 1;
      for (auto k = 0; k < mid; ++k, w = w * wn) {
        auto x = A[j + k], y = w * A[j + mid + k];
        A[j + k] = x + y;
        A[j + mid + k] = x - y;
      }
    }
  }
}

int main() {
  int n, m;
  std::cin >> n >> m;
  --n;
  --m;
  for (auto i = 0; i <= n; ++i)
    std::cin >> a[i];
  for (auto i = 0; i <= m; ++i)
    std::cin >> b[i];
  auto lim = 1;
  while (lim <= n + m) {
    lim *= 2;
    ++l;
  }
  for (auto i = 0; i < lim; ++i)
    r[i] = (r[i >> 1] >> 1) | ((i & 1) << (l - 1));
  fft(a, r, lim);
  fft(b, r, lim);
  for (auto i = 0; i <= lim; ++i)
    a[i] *= b[i];
  fft(a, r, lim);
  for (auto i = 0; i <= n + m; ++i)
    std::cout << std::lround(a[(lim - i) % lim].real() / lim) << ' ';
  std::cout << std::endl;
  return 0;
}