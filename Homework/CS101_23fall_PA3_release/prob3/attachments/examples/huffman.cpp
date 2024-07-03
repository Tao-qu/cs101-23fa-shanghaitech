#include <queue>
#include <iostream>
#include <memory>
#include <string>

struct HuffmanTreeNode {
  int frequency;
  std::size_t leftChild = -1; // -1 indicates null
  std::size_t rightChild = -1;

  explicit HuffmanTreeNode(int freq) : frequency{freq} {}
  HuffmanTreeNode(int freq, std::size_t left, std::size_t right)
      : frequency{freq}, leftChild{left}, rightChild{right} {}
};

void traverse(std::size_t cur, const std::string &code,
              const std::vector<HuffmanTreeNode> &nodes,
              std::vector<std::string> &result) {
  if (cur < result.size()) {
    result[cur] = code;
    return;
  }
  traverse(nodes[cur].leftChild, code + '0', nodes, result);
  traverse(nodes[cur].rightChild, code + '1', nodes, result);
}

std::vector<std::string> getHuffmanCodes(const std::vector<int> &frequencies) {
  // The Huffman Tree. Currently it contains only leaf nodes.
  std::vector<HuffmanTreeNode> nodes(frequencies.begin(), frequencies.end());

  auto cmp = [&nodes](std::size_t x, std::size_t y) {
    return nodes[x].frequency > nodes[y].frequency;
  };
  std::priority_queue<std::size_t, std::vector<std::size_t>, decltype(cmp)>
      minHeap(cmp);
  for (std::size_t i = 0; i != frequencies.size(); ++i)
    minHeap.push(i);

  while (minHeap.size() > 1) {
    auto first = minHeap.top();
    minHeap.pop();
    auto second = minHeap.top();
    minHeap.pop();
    nodes.emplace_back(nodes[first].frequency + nodes[second].frequency, first,
                       second);
    minHeap.push(nodes.size() - 1);
  }

  std::vector<std::string> result(frequencies.size());
  traverse(minHeap.top(), {}, nodes, result);
  return result;
}

int main() {
  std::vector frequencies = {45, 16, 9, 12, 13, 5};
  for (auto x : getHuffmanCodes(frequencies))
    std::cout << x << ' ';
  std::cout << std::endl;
  return 0;
}