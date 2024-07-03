#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <tuple>
#include <vector>
#include <algorithm>

class MinHeap
{
private:
    std::vector<std::pair<size_t, size_t>> data;

public:
    MinHeap(std::vector<std::pair<size_t, size_t>> data)
    {
        data.push_back(std::make_pair(0, 0));
        std::sort(data.begin(), data.end());
        for (auto &item : data)
        {
            this->data.push_back(item);
        }
    }

    size_t size()
    {
        return data.size() - 1;
    }

    std::pair<size_t, size_t> &top()
    {
        if (size() != 0)
        {
            return data[1];
        }
        return data[0];
    }

    std::pair<size_t, size_t> &qtop()
    {
        if (size() == 2)
        {
            return data[2];
        }
        else if (size() > 2)
        {
            if (data[2] < data[3])
            {
                return data[2];
            }
            else
            {
                return data[3];
            }
        }
        return data[0];
    }

    void pop()
    {
        if (size() == 1)
        {
            data.pop_back();
            return;
        }
        else
        {
            data[1] = data.back();
            data.pop_back();
            size_t i = 1;
            while (i * 2 <= size())
            {
                if (i * 2 + 1 <= size())
                {
                    if (data[i * 2] < data[i * 2 + 1])
                    {
                        if (data[i] > data[i * 2])
                        {
                            std::swap(data[i], data[i * 2]);
                            i *= 2;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (data[i] > data[i * 2 + 1])
                        {
                            std::swap(data[i], data[i * 2 + 1]);
                            i = i * 2 + 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if (data[i] > data[i * 2])
                    {
                        std::swap(data[i], data[i * 2]);
                        i *= 2;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    void push(std::pair<size_t, size_t> item)
    {
        data.push_back(item);
        size_t i = size();
        while (i > 1)
        {
            if (data[i] < data[i / 2])
            {
                std::swap(data[i], data[i / 2]);
                i /= 2;
            }
            else
            {
                break;
            }
        }
    }
};

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data)
{
    MinHeap data_cp(data);
    size_t comp = 0;
    while (!(data_cp.size() == 1 && data_cp.top().second == 1))
    {
        while (data_cp.top().second % 2 == 0 && (data_cp.size() == 1 || 2 * data_cp.top().first < data_cp.qtop().first))
        {
            data_cp.top().first *= 2;
            data_cp.top().second /= 2;
            comp += data_cp.top().first * data_cp.top().second;
        }
        if (data_cp.top().second != 1)
        {
            comp += 2 * data_cp.top().first * (data_cp.top().second / 2);
            data_cp.push(std::make_pair(2 * data_cp.top().first, data_cp.top().second / 2));
            if (data_cp.top().second % 2 == 1)
            {
                data_cp.top().second = 1;
            }
            else
            {
                data_cp.pop();
            }
        }
        else
        {
            size_t tmp = data_cp.top().first;
            data_cp.pop();
            comp += tmp + data_cp.top().first;
            data_cp.push(std::make_pair(tmp + data_cp.top().first, 1));
            if (data_cp.top().second == 1)
            {
                data_cp.pop();
            }
            else
            {
                data_cp.top().second -= 1;
            }
        }
    }
    return comp;
}
#endif // HUFFMAN_CALCULATOR_HPP