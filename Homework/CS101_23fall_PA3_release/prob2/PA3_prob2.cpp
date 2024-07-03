#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    int n, m;
    std::vector<std::pair<long long, int>> buyers;
    std::vector<int> winners;
    std::vector<long long> payments;
    std::vector<std::vector<long long>> dp, dp_reversed;

    std::cin >> n >> m;
    for (auto i = 0; i < n; ++i)
    {
        long long valuation;
        int count;
        std::cin >> valuation >> count;
        buyers.push_back(std::make_pair(valuation, count));
    }

    dp.resize(n + 1, std::vector<long long>(m + 1, 0));
    dp_reversed.resize(n + 1, std::vector<long long>(m + 1, 0));
    for (auto buyer_cnt = 1; buyer_cnt <= n; ++buyer_cnt)
    {
        for (auto item_cnt = 1; item_cnt <= m; ++item_cnt)
        {
            if (item_cnt < buyers[buyer_cnt - 1].second)
            {
                dp[buyer_cnt][item_cnt] = dp[buyer_cnt - 1][item_cnt];
            }
            else
            {
                dp[buyer_cnt][item_cnt] = std::max(dp[buyer_cnt - 1][item_cnt],
                                                   dp[buyer_cnt - 1][item_cnt - buyers[buyer_cnt - 1].second] + buyers[buyer_cnt - 1].first);
            }

            if (item_cnt < buyers[n - buyer_cnt].second)
            {
                dp_reversed[buyer_cnt][item_cnt] = dp_reversed[buyer_cnt - 1][item_cnt];
            }
            else
            {
                dp_reversed[buyer_cnt][item_cnt] = std::max(dp_reversed[buyer_cnt - 1][item_cnt],
                                                            dp_reversed[buyer_cnt - 1][item_cnt - buyers[n - buyer_cnt].second] + buyers[n - buyer_cnt].first);
            }
        }
    }

    auto buyer_cnt = n, item_cnt = m;
    while (buyer_cnt > 0 && item_cnt > 0)
    {
        if (buyers[n - buyer_cnt].second <= item_cnt &&
            dp_reversed[buyer_cnt][item_cnt] == dp_reversed[buyer_cnt - 1][item_cnt - buyers[n - buyer_cnt].second] + buyers[n - buyer_cnt].first)
        {
            winners.push_back(n - buyer_cnt + 1);
            item_cnt -= buyers[n - buyer_cnt].second;
        }
        --buyer_cnt;
    }
    for (auto winner : winners)
    {
        std::cout << winner << " ";
    }
    std::cout << std::endl;

    long long MSV = dp[n][m];
    for (auto winner : winners)
    {
        long long eMSV = 0;
        for (auto item_cnt = 0; item_cnt <= m; ++item_cnt)
        {
            eMSV = std::max(eMSV, dp[winner - 1][item_cnt] + dp_reversed[n - winner][m - item_cnt]);
        }
        payments.push_back(buyers[winner - 1].first + eMSV - MSV);
    }
    for (auto payment : payments)
    {
        std::cout << payment << " ";
    }
    std::cout << std::endl;

    return 0;
}