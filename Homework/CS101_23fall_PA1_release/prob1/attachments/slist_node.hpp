#ifndef CS101_SLIST_NODE_HPP
#define CS101_SLIST_NODE_HPP

#include <memory>
#include <utility>

template <typename T> struct SlistNode {
  using node_uptr = std::unique_ptr<SlistNode<T>>;

  node_uptr next;
  T value;

  template <typename... Types>
  SlistNode(node_uptr &&nxt, Types &&...args)
      : next(std::move(nxt)), value(std::forward<Types>(args)...) {}
};

#endif // CS101_SLIST_NODE_HPP