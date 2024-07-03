#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <tuple>
#include <vector>
#include <algorithm>

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data)
{
  auto data_cp = data;
  std::sort(data_cp.begin(), data_cp.end());
  if (data_cp.size() == 1 && data_cp[0].second == 1)
  {
    return 0;
  }
  size_t comp = 0;
  while (data_cp[0].second % 2 == 0 && (data_cp.size() == 1 || 2 * data_cp[0].first < data_cp[1].first))
  {
    data_cp[0].first *= 2;
    data_cp[0].second /= 2;
    comp += data_cp[0].first * data_cp[0].second;
  }
  if (data_cp[0].second != 1)
  {
    comp += 2 * data_cp[0].first * (data_cp[0].second / 2);
    auto it = data_cp.begin() + 1;
    for (; it != data_cp.end(); ++it)
    {
      if (2 * data_cp[0].first == it->first)
      {
        it->second += data_cp[0].second / 2;
        break;
      }
      else if (2 * data_cp[0].first < it->first)
      {
        data_cp.insert(it, std::make_pair(2 * data_cp[0].first, data_cp[0].second / 2));
        break;
      }
    }
    if (it == data_cp.end())
    {
      data_cp.push_back(std::make_pair(2 * data_cp[0].first, data_cp[0].second / 2));
    }
    if (data_cp[0].second % 2 == 1)
    {
      data_cp[0].second = 1;
    }
    else
    {
      data_cp.erase(data_cp.begin());
    }
  }
  else
  {
    comp += data_cp[0].first + data_cp[1].first;
    data_cp[0].first += data_cp[1].first;
    data_cp[1].second -= 1;
    auto it = data_cp.begin() + 2;
    for (; it != data_cp.end(); ++it)
    {
      if (data_cp[0].first == it->first)
      {
        it->second += 1;
        data_cp.erase(data_cp.begin());
        break;
      }
      else if (data_cp[0].first < it->first)
      {
        data_cp.insert(it, std::make_pair(data_cp[0].first, 1));
        data_cp.erase(data_cp.begin());
        break;
      }
    }
    if (it == data_cp.end())
    {
      data_cp.push_back(std::make_pair(data_cp[0].first, 1));
      data_cp.erase(data_cp.begin());
    }
    if (data_cp[0].second == 0)
    {
      data_cp.erase(data_cp.begin());
    }
  }
  return get_huffman_length(data_cp) + comp;
}
#endif // HUFFMAN_CALCULATOR_HPP