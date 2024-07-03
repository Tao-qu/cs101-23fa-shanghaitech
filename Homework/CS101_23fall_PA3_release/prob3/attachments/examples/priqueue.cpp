#include <queue>
#include <cstdint>
#include <iostream>

using VertexID = std::size_t;
using Weight = std::int64_t;

struct HeapEntry {
  VertexID vertex;
  Weight dist;

  auto operator<=>(const HeapEntry &rhs) const { return dist <=> rhs.dist; }
};

// Note: std::priority_queue<T> is a max-heap, which is by default
// std::priority_queue<T, std::vector<T>, std::less<>>.
// To create a min-heap, we need to pass std::greater<> as the third template
// argument.
template <typename T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<>>;

template <typename T> using MaxHeap = std::priority_queue<T>;

int main() {
  MinHeap<HeapEntry> minHeap;

  Weight dist[] = {3, 0, 1, 5, 2, 7, 9};
  for (VertexID i = 0; i != std::size(dist); ++i)
    minHeap.push({.vertex = i, .dist = dist[i]});

  while (!minHeap.empty()) {
    auto [v, d] = minHeap.top();
    minHeap.pop();
    std::cout << "Got vertex " << v << " with dist " << d << std::endl;
  }

  return 0;
}