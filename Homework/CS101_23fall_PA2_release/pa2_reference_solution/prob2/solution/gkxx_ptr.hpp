#ifndef CS10123F_PA2_BST
#define CS10123F_PA2_BST

#include <vector>
#include <iostream>
#include <cassert>
#include <limits>
#include <utility>

class ScapegoatTree {
public:
  using value_type = long long;
  using size_type = std::size_t;

private:
  struct Node {
    value_type element;
    Node *left{}, *right{};
    size_type size{1};
    value_type sum;

    auto left_sum() const { return left ? left->sum : 0; }
    auto right_sum() const { return right ? right->sum : 0; }
    auto left_size() const { return left ? left->size : 0; }
    auto right_size() const { return right ? right->size : 0; }

    explicit Node(value_type x) : element{x}, sum{x} {}
  };

  Node *root{};

  static void recursive_delete(Node *node) {
    if (!node)
      return;
    recursive_delete(node->left);
    recursive_delete(node->right);
    delete node;
  }

  static constexpr auto alpha = 0.75;

  static bool is_alpha_balanced(Node *node) {
    assert(node);
    return (node->left_size() <= alpha * node->size) &&
           (node->right_size() <= alpha * node->size);
  }

  static Node **recursive_insert(Node *&node, value_type x) {
    if (!node) {
      node = new Node{x};
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
  static Node *recursive_rebuild(size_t l, size_t r,
                                 const std::vector<Node *> &nodes) {
    assert(l <= r);
    if (l == r)
      return nullptr;
    auto mid = (l + r) / 2;
    auto node = nodes[mid];
    node->left = recursive_rebuild(l, mid, nodes);
    node->right = recursive_rebuild(mid + 1, r, nodes);
    node->sum = node->left_sum() + node->right_sum() + node->element;
    node->size = node->left_size() + node->right_size() + 1;
    return node;
  }

  static Node *cut_minimum(Node *&node) {
    if (!node->left)
      return std::exchange(node, node->right);
    auto min = cut_minimum(node->left);
    --node->size;
    node->sum -= min->element;
    return min;
  }

  static void recursive_erase(Node *&node, value_type x) {
    assert(node);
    if (x == node->element) {
      if (node->left == nullptr && node->right == nullptr) {
        delete node;
        node = nullptr;
        return;
      } else if (node->left && node->right) {
        auto right_min = cut_minimum(node->right);
        right_min->left = node->left;
        right_min->right = node->right;
        delete std::exchange(node, right_min);
      } else
        delete std::exchange(node, node->left ? node->left : node->right);
    } else
      recursive_erase(x < node->element ? node->left : node->right, x);
    node->size = node->left_size() + node->right_size() + 1;
    node->sum = node->left_sum() + node->right_sum() + node->element;
  }

  static bool recursive_find(Node *node, value_type x) {
    if (!node)
      return false;
    if (x == node->element)
      return true;
    return recursive_find(x < node->element ? node->left : node->right, x);
  }

  static value_type recursive_kth(Node *node, size_type k) {
    auto left_size = node->left_size();
    if (left_size + 1 == k)
      return node->element;
    if (left_size >= k)
      return recursive_kth(node->left, k);
    return recursive_kth(node->right, k - left_size - 1);
  }

  static value_type recursive_query(Node *node, value_type x,
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

  static void dfs_to_vector(Node *node, std::vector<Node *> &nodes) {
    if (!node)
      return;
    dfs_to_vector(node->left, nodes);
    nodes.push_back(node);
    dfs_to_vector(node->right, nodes);
  }

  static bool verify(Node *cur) {
    return cur->sum == cur->left_sum() + cur->right_sum() + cur->element &&
           cur->size == cur->left_size() + cur->right_size() + 1;
  }

  static void recursive_debug(Node *cur) {
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
  ~ScapegoatTree() { clear(); }

  void clear() {
    recursive_delete(root);
    root = nullptr;
  }

  void insert(value_type x) {
    if (auto to_rebuild = recursive_insert(root, x)) {
      std::vector<Node *> nodes;
      dfs_to_vector(*to_rebuild, nodes);
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