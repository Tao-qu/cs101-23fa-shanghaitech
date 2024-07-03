#include <stdio.h>
#include <stdlib.h>

#define L 50000000
int mem[L], *p = mem;

int* newArray(int size) {
    int* ret = p;
    p += size;
    return ret;
}

int* solve(int* a, int* b, int n) {
    int* ans = newArray(n * 2 - 1);
    if (n < 100) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                ans[i + j] += a[i] * b[j];
        return ans;
    }
    int m = (n + 1) / 2;
    int* ansl = solve(a, b, m);
    int* ansr = solve(a + m, b + m, n - m);
    for (int i = m; i < n; ++i) {
        a[i - m] += a[i];
        b[i - m] += b[i];
    }
    int* anss = solve(a, b, m);
    for (int i = m; i < n; ++i) {
        a[i - m] -= a[i];
        b[i - m] -= b[i];
    }
    for (int i = 0; i < 2 * m - 1; ++i) {
        ans[i] += ansl[i];
        ans[i + m] += anss[i] - ansl[i];
    }
    for (int i = 0; i < 2 * (n - m) - 1; ++i) {
        ans[i + 2 * m] += ansr[i];
        ans[i + m] -= ansr[i];
    }
    return ans;
}

int main(void) {
    freopen("karatsuba.in", "r", stdin);
    int n, m;
    scanf("%d%d", &n, &m);
    n += 1;
    m += 1;
    int maxn = n > m ? n : m;
    int* a = newArray(maxn);
    int* b = newArray(maxn);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d", &b[i]);
    }
    int* ans = solve(a, b, maxn);
    for (int i = 0; i < n + m - 1; ++i) {
        printf("%d ", ans[i]);
    }
    fclose(stdin);
    return 0;
}