#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define L 1000009
#define S 6
#define B 1000000

long long mem[4 * L], tmp[4 * L];

long long* solve(long long* a, long long* b, int n, long long* ans) {
    if (n < 30) {
        memset(ans, 0, sizeof(long long) * (n * 2 - 1));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                ans[i + j] += a[i] * b[j];
        return ans;
    }
    int m = (n + 1) / 2;
    long long* ansl = solve(a, b, m, ans);
    long long* ansr = solve(a + m, b + m, n - m, ansl + m * 2 - 1);
    for (int i = m; i < n; ++i) {
        a[i - m] += a[i];
        b[i - m] += b[i];
    }
    long long* anss = solve(a, b, m, ansr + (n - m) * 2 - 1);
    for (int i = m; i < n; ++i) {
        a[i - m] -= a[i];
        b[i - m] -= b[i];
    }
    memset(tmp, 0, sizeof(long long) * (n * 2 - 1));
    for (int i = 0; i < 2 * m - 1; ++i) {
        tmp[i] += ansl[i];
        tmp[i + m] += anss[i] - ansl[i];
    }
    for (int i = 0; i < 2 * (n - m) - 1; ++i) {
        tmp[i + 2 * m] += ansr[i];
        tmp[i + m] -= ansr[i];
    }
    memcpy(ans, tmp, sizeof(long long) * (n * 2 - 1));
    return ans;
}

char sa[L], sb[L];
long long a[L], b[L];

int main(void) {
    freopen("AmulB.in", "r", stdin);
    scanf("%s%s", sa, sb);
    int len_sa = strlen(sa), len_sb = strlen(sb);
    int n = 0, m = 0;
    for (int i = len_sa; i > 0; i -= S, ++n)
        for (int j = i < S ? 0 : i - S; j < i; ++j)
            a[n] = a[n] * 10 + sa[j] - '0';
    for (int i = len_sb; i > 0; i -= S, ++m)
        for (int j = i < S ? 0 : i - S; j < i; ++j)
            b[m] = b[m] * 10 + sb[j] - '0';
    int maxn = n > m ? n : m;
    long long* ans = solve(a, b, maxn, mem);
    for (int i = 0; i < n + m - 2; ++i) {
        ans[i + 1] += ans[i] / B;
        ans[i] %= B;
    }
    printf("%lld", ans[n + m - 2]);
    for (int i = n + m - 3; i >= 0; --i) {
        printf("%06lld", ans[i]);
    }
    fclose(stdin);
    return 0;
}