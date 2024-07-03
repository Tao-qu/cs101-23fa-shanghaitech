#include <iostream>
#include <vector>
#include <algorithm>

class Vector
{
public:
    long long value;
    long long left;
    long long right;
};

bool compare(Vector a, Vector b)
{
    return a.value > b.value;
}

class DisjointSet
{
private:
    std::vector<long long> parent;

public:
    DisjointSet(long long n)
    {
        parent.resize(n + 1);
        for (long long i = 0; i <= n; ++i)
        {
            parent[i] = i;
        }
    }

    long long find(long long x)
    {
        if (parent[x] != x)
        {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void merge(long long x, long long y)
    {
        long long xroot = find(x);
        long long yroot = find(y);
        parent[yroot] = xroot;
    }

    bool connected(long long x, long long y)
    {
        return find(x) == find(y);
    }
};

int main()
{
    long long n, d;
    std::cin >> n >> d;
    std::vector<Vector> v;
    for (long long i = 0; i < n; ++i)
    {
        Vector temp;
        std::cin >> temp.left >> temp.right >> temp.value;
        v.push_back(temp);
    }
    std::sort(v.begin(), v.end(), compare);
    DisjointSet ds(n);
    long long ans = 0;
    for (long long i = 0; i < n; ++i)
    {
        if (!ds.connected(v[i].left - 1, v[i].right))
        {
            ds.merge(v[i].left - 1, v[i].right);
            ans += v[i].value;
        }
    }
    std::cout << ans << std::endl;
    return 0;
}