#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <vector>

struct Data {
  std::size_t freq{};
  std::size_t amount{};
  std::size_t sum{};

  auto operator<=>(const Data other) const { return freq <=> other.freq; }

  Data operator+(const Data &other) const {
    return {freq + other.freq, 1,
            amount * (freq + sum) + other.amount * (other.freq + other.sum)};
  }
};

class MinHeap {
private:
  std::vector<Data> m_data;

  static constexpr std::size_t get_parent_idx(const std::size_t i) {
    return (i - 1) / 2;
  }

  static constexpr std::size_t get_left_child_idx(const std::size_t i) {
    return i * 2 + 1;
  }

  static constexpr std::size_t get_right_child_idx(const std::size_t i) {
    return i * 2 + 2;
  }

  std::size_t find_small_child_idx(const std::size_t i) const {
    const auto left = get_left_child_idx(i);
    const auto right = get_right_child_idx(i);
    if (left >= size()) {
      return size();
    } else if (right >= size()) {
      return left;
    } else {
      return m_data[left] < m_data[right] ? left : right;
    }
  }

  void percolate_down_from_top() {
    std::size_t percolating = 0;
    std::size_t small_child = find_small_child_idx(0);
    while (small_child < size()) {
      if (m_data[small_child] < m_data[percolating]) {
        std::swap(m_data[percolating], m_data[small_child]);
        percolating = small_child;
        small_child = find_small_child_idx(percolating);
      } else {
        break;
      }
    }
  }

  void percolate_up_from_last() {
    auto cur = size() - 1;
    while (cur != 0) {
      const auto parent = get_parent_idx(cur);
      if (m_data[cur] < m_data[parent]) {
        std::swap(m_data[cur], m_data[parent]);
        cur = parent;
      } else {
        break;
      }
    }
  }

public:
  MinHeap() = default;

  constexpr std::size_t size() const { return m_data.size(); }

  void push(Data data) {
    m_data.push_back(data);
    percolate_up_from_last();
  }

  Data pop() {
    const auto return_value = m_data[0];
    m_data[0] = m_data[size() - 1];
    m_data.pop_back();
    percolate_down_from_top();
    return return_value;
  }

  [[nodiscard]] constexpr Data top() const { return m_data[0]; }
};

std::size_t get_huffman_length(
    const std::vector<std::pair<std::size_t, std::size_t>> &data) {
  MinHeap h;
  for (const auto &[freq, amt] : data) {
    h.push({.freq = freq, .amount = amt});
  }
  size_t maxSize = 0;
  while (h.size() > 1 || h.top().amount > 1) {
    maxSize = std::max(maxSize, h.size());
    if (h.top().amount > 1) {
      // Simply consider the smallest.
      auto smallest = h.pop();
      h.push({.freq = smallest.freq * 2,
              .amount = smallest.amount / 2,
              .sum = (smallest.sum + smallest.freq) * 2});
      smallest.amount %= 2;
      if (smallest.amount != 0) {
        h.push(smallest);
      }
    } else {
      const auto first = h.pop();
      auto second = h.pop();
      if (second.amount != 1) {
        // Put back
        auto put_back = second;
        --put_back.amount;
        h.push(put_back);
      }
      second.amount = 1;
      h.push({.freq = first.freq + second.freq,
              .amount = 1,
              .sum = first.freq + second.freq + first.sum + second.sum});
    }
  }
  return h.top().sum;
}
#endif // HUFFMAN_CALCULATOR_HPP