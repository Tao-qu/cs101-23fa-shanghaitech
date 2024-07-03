#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <numbers>

class FFT_Multi_Poly
{
private:
    std::vector<int> coef;

public:
    FFT_Multi_Poly(std::vector<int> coef) : coef(coef) {}

    FFT_Multi_Poly operator*(const FFT_Multi_Poly &other)
    {
        auto n = coef.size() + other.coef.size() - 1;
        size_t m = 1;
        while (m < n)
        {
            m <<= 1;
        }
        std::vector<std::complex<double>> a(m), b(m);
        for (size_t i = 0; i < coef.size(); ++i)
        {
            a[i] = coef[i];
        }
        for (size_t i = 0; i < other.coef.size(); ++i)
        {
            b[i] = other.coef[i];
        }
        fft(a, false);
        fft(b, false);
        for (size_t i = 0; i < m; ++i)
        {
            a[i] *= b[i];
        }
        fft(a, true);
        std::vector<int> ans(n);
        for (size_t i = 0; i < n; ++i)
        {
            ans[i] = std::round(a[i].real());
        }
        FFT_Multi_Poly poly_ans(ans);
        return poly_ans;
    }

    void fft(std::vector<std::complex<double>> &a, bool inv)
    {
        auto n = a.size();
        for (size_t i = 1, j = 0; i < n; ++i)
        {
            auto bit = n >> 1;
            for (; j >= bit; bit >>= 1)
            {
                j -= bit;
            }
            j += bit;
            if (i < j)
            {
                std::swap(a[i], a[j]);
            }
        }
        for (size_t len = 2; len <= n; len <<= 1)
        {
            double ang = 2 * std::numbers::pi / len * (inv ? -1 : 1);
            std::complex<double> wlen(std::cos(ang), std::sin(ang));
            for (size_t i = 0; i < n; i += len)
            {
                std::complex<double> w(1);
                for (size_t j = 0; j < len / 2; ++j)
                {
                    std::complex<double> u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
        if (inv)
        {
            for (size_t i = 0; i < n; ++i)
            {
                a[i] /= n;
            }
        }
    }

    void print()
    {
        auto n = coef.size();
        for (size_t i = 0; i < n; ++i)
        {
            std::cout << coef[i] << " ";
        }
    }
};

int main()
{
    int n, m;
    std::cin >> n >> m;
    std::vector<int> a(n), b(m);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> a[i];
    }
    for (int i = 0; i < m; ++i)
    {
        std::cin >> b[i];
    }
    FFT_Multi_Poly poly_a(a), poly_b(b);
    FFT_Multi_Poly poly_ans = poly_a * poly_b;
    poly_ans.print();
    return 0;
}