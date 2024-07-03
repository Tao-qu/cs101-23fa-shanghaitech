#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define L 1200001
int a[L], b[L], mem[L], tmp[L];

int* solve(int* a, int* b, int n, int* ans) {
    if (n < 30) {
        memset(ans, 0, sizeof(int) * (n * 2 - 1));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                ans[i + j] += a[i] * b[j];
        return ans;
    }
    int m = (n + 1) / 2;
    int* ansl = solve(a, b, m, ans);
    int* ansr = solve(a + m, b + m, n - m, ansl + m * 2 - 1);
    for (int i = m; i < n; ++i) {
        a[i - m] += a[i];
        b[i - m] += b[i];
    }
    int* anss = solve(a, b, m, ansr + (n - m) * 2 - 1);
    for (int i = m; i < n; ++i) {
        a[i - m] -= a[i];
        b[i - m] -= b[i];
    }
    memset(tmp, 0, sizeof(int) * (n * 2 - 1));
    for (int i = 0; i < 2 * m - 1; ++i) {
        tmp[i] += ansl[i];
        tmp[i + m] += anss[i] - ansl[i];
    }
    for (int i = 0; i < 2 * (n - m) - 1; ++i) {
        tmp[i + 2 * m] += ansr[i];
        tmp[i + m] -= ansr[i];
    }
    memcpy(ans, tmp, sizeof(int) * (n * 2 - 1));
    return ans;
}

int main(void) {
    freopen("karatsuba.in", "r", stdin);
    int n, m;
    scanf("%d%d", &n, &m);
    int maxn = n > m ? n : m;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
    for (int i = 0; i < m; ++i) {
        scanf("%d", &b[i]);
    }
    int* ans = solve(a, b, maxn, mem);
    for (int i = 0; i < n + m - 1; ++i) {
        printf("%d ", ans[i]);
    }
    fclose(stdin);
    return 0;
}