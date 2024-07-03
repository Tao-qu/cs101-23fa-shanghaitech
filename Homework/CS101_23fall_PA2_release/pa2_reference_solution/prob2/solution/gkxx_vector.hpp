#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <vector>
#include <iostream>
#include <cassert>
#include <utility>
#include <format>
#include <limits>

class ScapegoatTree {
public:
  using value_type = long long;
  using size_type = std::size_t;
  using index = std::size_t;

private:
  struct Node {
    value_type value;
    index left, right; // index
    size_type size;    // number of nodes in the subtree
    value_type sum;    // sum of elements in the subtree

    explicit constexpr Node(value_type x)
        : value{x}, left{}, right{}, size{1}, sum{x} {}

    static constexpr auto null_node() {
      Node ret(0);
      ret.size = 0;
      return ret;
    }
  };

  std::vector<Node> nodes;
  index root;

  static constexpr auto alpha = 0.75;

  auto &value(index node) const { return nodes[node].value; }
  auto &left(index node) const { return nodes[node].left; }
  auto &right(index node) const { return nodes[node].right; }
  auto &size(index node) const { return nodes[node].size; }
  auto &sum(index node) const { return nodes[node].sum; }
  auto &value(index node) { return nodes[node].value; }
  auto &left(index node) { return nodes[node].left; }
  auto &right(index node) { return nodes[node].right; }
  auto &size(index node) { return nodes[node].size; }
  auto &sum(index node) { return nodes[node].sum; }

  bool is_alpha_balanced(index node) const {
    return size(left(node)) <= alpha * size(node) &&
           size(right(node)) <= alpha * size(node);
  }

  index new_node(value_type x) {
    nodes.emplace_back(x);
    return nodes.size() - 1;
  }

  void update_node(index node) {
    size(node) = size(left(node)) + size(right(node)) + 1;
    sum(node) = sum(left(node)) + sum(right(node)) + value(node);
  }

  bool recursive_find(index cur, value_type x) const {
    if (!cur)
      return false;
    if (x == value(cur))
      return true;
    if (x < value(cur))
      return recursive_find(left(cur), x);
    else
      return recursive_find(right(cur), x);
  }

  index *recursive_insert(index &cur, value_type x) {
    if (!cur) {
      cur = new_node(x);
      return nullptr;
    }
    ++size(cur);
    sum(cur) += x;
    auto ret = recursive_insert(x < value(cur) ? left(cur) : right(cur), x);
    return is_alpha_balanced(cur) ? ret : &cur;
  }

  void traverse(index cur, std::vector<index> &subtree) const {
    if (!cur)
      return;
    traverse(left(cur), subtree);
    subtree.push_back(cur);
    traverse(right(cur), subtree);
  }

  index recursive_rebuild(const std::vector<index> &subtree, std::size_t b,
                          std::size_t e) {
    assert(b <= e);
    if (b == e)
      return 0;
    auto mid = (b + e) / 2;
    auto cur = subtree[mid];
    left(cur) = recursive_rebuild(subtree, b, mid);
    right(cur) = recursive_rebuild(subtree, mid + 1, e);
    update_node(cur);
    return cur;
  }

  void rebuild(index *goat) {
    std::vector<index> subtree;
    traverse(*goat, subtree);
    *goat = recursive_rebuild(subtree, 0, subtree.size());
  }

  index cut_minimum(index &cur) {
    if (!left(cur))
      return std::exchange(cur, right(cur));
    auto min = cut_minimum(left(cur));
    --size(cur);
    sum(cur) -= value(min);
    return min;
  }

  void recursive_erase(index &cur, value_type x) {
    sum(cur) -= x;
    --size(cur);
    if (x == value(cur)) {
      if (!left(cur))
        cur = right(cur);
      else if (!right(cur))
        cur = left(cur);
      else {
        auto new_node = cut_minimum(right(cur));
        left(new_node) = left(cur);
        right(new_node) = right(cur);
        cur = new_node;
        update_node(cur);
      }
      return;
    }
    if (x < value(cur))
      recursive_erase(left(cur), x);
    else
      recursive_erase(right(cur), x);
  }

  value_type recursive_kth(index cur, size_type k) const {
    assert(cur);
    if (k <= size(left(cur)))
      return recursive_kth(left(cur), k);
    else if (k == size(left(cur)) + 1)
      return value(cur);
    else
      return recursive_kth(right(cur), k - size(left(cur)) - 1);
  }

  value_type recursive_query(index cur, value_type x,
                             value_type helper = 0) const {
    static constexpr auto infty = std::numeric_limits<value_type>::max();
    if (!cur)
      return infty;
    auto less_sum = helper + sum(left(cur)) + value(cur);
    if (auto result = x * less_sum - (sum(root) - less_sum); result >= 0)
      return std::min(result, recursive_query(left(cur), x, helper));
    else
      return recursive_query(right(cur), x, less_sum);
  }

  bool verify_node(index cur) const {
    return sum(cur) == sum(left(cur)) + sum(right(cur)) + value(cur) &&
           size(cur) == size(left(cur)) + size(right(cur)) + 1;
  }

  void debug_traverse(index cur) const {
    if (!cur)
      return;
    std::cout << "(";
    debug_traverse(left(cur));
    std::cout << std::format(", {}{}, ", verify_node(cur) ? "" : "!",
                             value(cur));
    debug_traverse(right(cur));
    std::cout << ")";
  }

public:
  constexpr ScapegoatTree() : nodes{Node::null_node()}, root{} {}

  void insert(value_type x) {
    if (auto goat = recursive_insert(root, x))
      rebuild(goat);
  }

  void erase(value_type x) { recursive_erase(root, x); }

  bool find(value_type x) const { return recursive_find(root, x); }

  value_type kth(size_type k) const { return recursive_kth(root, k); }

  value_type query(value_type x) const { return recursive_query(root, x); }

  void clear() {
    root = 0;
    nodes = {Node::null_node()};
  }

  void debug_traverse() const {
    debug_traverse(root);
    std::cout << std::endl;
  }

  void prepare(std::size_t n) { nodes.reserve(n + 1); }
};

ScapegoatTree g_tree;

void init(std::size_t n [[maybe_unused]]) { g_tree.prepare(n); }

void clear() { g_tree.clear(); }

long long SetOperation(std::size_t opt, long long x) {
  // g_tree.debug_traverse();
  switch (opt) {
  case 1:
    g_tree.insert(x);
    break;
  case 2:
    g_tree.erase(x);
    break;
  case 3:
    return g_tree.find(x);
  case 4:
    return g_tree.kth(x);
  case 5:
    return g_tree.query(x);
  }
  return 0;
}

#endif // CS10123F_PA2_BST