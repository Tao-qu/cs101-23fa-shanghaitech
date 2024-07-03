#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <vector>
#include <iostream>
#include <cassert>
#include <limits>
#include <utility>
#include <memory>

class ScapegoatTree {
public:
  using value_type = long long;
  using size_type = std::size_t;

private:
  struct Node {
    value_type element;
    std::unique_ptr<Node> left, right;
    size_type size{1};
    value_type sum;

    auto left_sum() const { return left ? left->sum : 0; }
    auto right_sum() const { return right ? right->sum : 0; }
    auto left_size() const { return left ? left->size : 0; }
    auto right_size() const { return right ? right->size : 0; }

    void update() {
      size = left_size() + right_size() + 1;
      sum = left_sum() + right_sum() + element;
    }

    explicit Node(value_type x) : element{x}, sum{x} {}
  };

  using node_uptr = std::unique_ptr<Node>;

  node_uptr root;

  static constexpr auto alpha = 0.75;

  static bool is_alpha_balanced(const node_uptr &node) {
    assert(node);
    return (node->left_size() <= alpha * node->size) &&
           (node->right_size() <= alpha * node->size);
  }

  static node_uptr *recursive_insert(node_uptr &node, value_type x) {
    if (!node) {
      node = std::make_unique<Node>(x);
      return nullptr;
    }
    ++node->size;
    node->sum += x;
    auto ret =
        recursive_insert(x < node->element ? node->left : node->right, x);
    if (!is_alpha_balanced(node))
      return &node;
    else
      return ret;
  }

  // [l, r)
  static node_uptr recursive_rebuild(std::size_t l, std::size_t r,
                                     std::vector<node_uptr> &nodes) {
    assert(l <= r);
    if (l == r)
      return nullptr;
    auto mid = (l + r) / 2;
    auto node = std::move(nodes[mid]);
    node->left = recursive_rebuild(l, mid, nodes);
    node->right = recursive_rebuild(mid + 1, r, nodes);
    node->update();
    return node;
  }

  static node_uptr cut_minimum(node_uptr &node) {
    if (!node->left)
      return std::exchange(node, std::move(node->right));
    auto min = cut_minimum(node->left);
    --node->size;
    node->sum -= min->element;
    return min;
  }

  static void recursive_erase(node_uptr &node, value_type x) {
    assert(node);
    if (x == node->element) {
      if (!node->left && !node->right) {
        node.reset();
        return;
      } else if (node->left && node->right) {
        auto right_min = cut_minimum(node->right);
        right_min->left = std::move(node->left);
        right_min->right = std::move(node->right);
        node = std::move(right_min);
      } else
        node = std::move(node->left ? node->left : node->right);
    } else
      recursive_erase(x < node->element ? node->left : node->right, x);
    node->update();
  }

  static bool recursive_find(const node_uptr &node, value_type x) {
    if (!node)
      return false;
    if (x == node->element)
      return true;
    return recursive_find(x < node->element ? node->left : node->right, x);
  }

  static value_type recursive_kth(const node_uptr &node, size_type k) {
    auto left_size = node->left_size();
    if (left_size + 1 == k)
      return node->element;
    if (left_size >= k)
      return recursive_kth(node->left, k);
    return recursive_kth(node->right, k - left_size - 1);
  }

  static value_type recursive_query(const node_uptr &node, value_type x,
                                    value_type left_sum, value_type right_sum) {
    if (!node)
      return std::numeric_limits<value_type>::max();
    auto cur = x * (left_sum + node->left_sum() + node->element) -
               (right_sum + node->right_sum());
    if (cur >= 0)
      return std::min(
          cur, recursive_query(node->left, x, left_sum,
                               node->element + right_sum + node->right_sum()));
    else
      return recursive_query(node->right, x,
                             node->element + left_sum + node->left_sum(),
                             right_sum);
  }

  static void dfs_to_vector(node_uptr node, std::vector<node_uptr> &nodes) {
    if (!node)
      return;
    dfs_to_vector(std::move(node->left), nodes);
    nodes.push_back(std::move(node));
    dfs_to_vector(std::move(nodes.back()->right), nodes);
  }

  static bool verify(const node_uptr &cur) {
    return cur->sum == cur->left_sum() + cur->right_sum() + cur->element &&
           cur->size == cur->left_size() + cur->right_size() + 1;
  }

  static void recursive_debug(const node_uptr &cur) {
    if (!cur)
      return;
    std::cout << "(";
    recursive_debug(cur->left);
    std::cout << ", ";
    if (!verify(cur))
      std::cout << '!';
    std::cout << cur->element << ", ";
    recursive_debug(cur->right);
    std::cout << ")";
  }

public:
  ScapegoatTree() = default;
  ScapegoatTree(const ScapegoatTree &) = delete;
  ScapegoatTree &operator=(const ScapegoatTree &) = delete;
  ScapegoatTree(ScapegoatTree &&) = delete;
  ScapegoatTree &operator=(ScapegoatTree &&) = delete;
  ~ScapegoatTree() = default;

  void clear() { root.reset(); }

  void insert(value_type x) {
    if (auto to_rebuild = recursive_insert(root, x)) {
      std::vector<node_uptr> nodes;
      dfs_to_vector(std::move(*to_rebuild), nodes);
      *to_rebuild = recursive_rebuild(0, nodes.size(), nodes);
    }
  }

  void erase(value_type x) { recursive_erase(root, x); }

  bool find(value_type x) const { return recursive_find(root, x); }

  value_type kth(size_type k) const { return recursive_kth(root, k); }

  value_type ksum(value_type x) const { return recursive_query(root, x, 0, 0); }

  void debug() const {
    recursive_debug(root);
    std::cout << std::endl;
  }
};

ScapegoatTree tree;

void init(std::size_t n [[maybe_unused]]) {}

void clear() { tree.clear(); }

long long SetOperation(std::size_t opt, long long x) {
  // tree.debug();
  switch (opt) {
  case 1:
    tree.insert(x);
    return 0;
  case 2:
    tree.erase(x);
    return 0;
  case 3:
    return tree.find(x);
  case 4:
    return tree.kth(x);
  case 5:
    return tree.ksum(x);
  }
  return -1;
}

#endif // CS10123F_PA2_BST