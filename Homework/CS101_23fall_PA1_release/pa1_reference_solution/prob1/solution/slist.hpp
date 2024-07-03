#ifndef CS101_SLIST_HPP
#define CS101_SLIST_HPP

#include <concepts>
#include <iterator>
#include <memory>

template <typename T> struct SlistNode {
  using node_uptr = std::unique_ptr<SlistNode<T>>;

  node_uptr next;
  T value;

  template <typename... Types>
  SlistNode(node_uptr &&nxt, Types &&...args)
      : next(std::move(nxt)), value(std::forward<Types>(args)...) {}
};

template <typename T, bool IsConst> struct SlistIterator {
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = std::conditional_t<IsConst, const T &, T &>;
  using pointer = std::conditional_t<IsConst, const T *, T *>;
  using iterator_category = std::forward_iterator_tag;

private:
  using node_type = SlistNode<T>;
  using self_type = SlistIterator<T, IsConst>;

  node_type *m_node{};

  template <typename OtherDecayed>
  static constexpr auto isAddingLowLevelConst =
      IsConst && std::is_same_v<OtherDecayed, SlistIterator<T, false>>;

public:
  SlistIterator() = default;

  explicit SlistIterator(const std::unique_ptr<node_type> &up)
      : m_node{up.get()} {}

  template <typename Other>
    requires isAddingLowLevelConst<std::decay_t<Other>>
  SlistIterator(Other &&other) : m_node{other.base()} {}

  auto base() const { return m_node; }

  reference operator*() const { return m_node->value; }
  pointer operator->() const { return std::addressof(operator*()); }
  self_type &operator++() {
    m_node = m_node->next.get();
    return *this;
  }
  self_type operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  template <bool C> bool operator==(const SlistIterator<T, C> &other) const {
    return base() == other.base();
  }
};

template <typename T> class Slist {
public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using iterator = SlistIterator<T, false>;
  using const_iterator = SlistIterator<T, true>;

private:
  using node_type = SlistNode<T>;
  using node_uptr = std::unique_ptr<node_type>;

  node_uptr m_head;

  template <typename... Types>
  static void s_create_after(node_uptr &prior_next, Types &&...args) {
    prior_next = std::make_unique<node_type>(std::move(prior_next),
                                             std::forward<Types>(args)...);
  }

  static void s_delete_after(node_uptr &prior_next) {
    prior_next = std::move(prior_next->next);
  }

public:
  Slist() = default;

  Slist(const Slist &other) : Slist(other.begin(), other.end()) {}

  void swap(Slist &other) noexcept { m_head.swap(other.m_head); }

  Slist &operator=(const Slist &other) {
    Slist(other).swap(*this);
    return *this;
  }

  Slist(Slist &&) noexcept = default;

  Slist &operator=(Slist &&) noexcept = default;

  ~Slist() { clear(); }

  Slist(size_type count, const T &value) {
    while (count--)
      push_front(value);
  }

  explicit Slist(size_type count) {
    while (count--)
      emplace_front();
  }

  Slist(std::input_iterator auto b, std::input_iterator auto e) {
    auto *prior_next = &m_head;
    while (b != e) {
      s_create_after(*prior_next, *b++);
      prior_next = &(*prior_next)->next;
    }
  }

  Slist(std::initializer_list<T> init) : Slist(init.begin(), init.end()) {}

  iterator begin() { return iterator(m_head); }
  iterator end() { return {}; }
  const_iterator cbegin() const { return const_iterator(m_head); }
  const_iterator cend() const { return {}; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  void push_front(const T &x) { emplace_front(x); }

  void push_front(T &&x) { emplace_front(std::move(x)); }

  template <typename... Types> reference emplace_front(Types &&...args) {
    s_create_after(m_head, std::forward<Types>(args)...);
    return m_head->value;
  }

  void pop_front() { s_delete_after(m_head); }

  void clear() {
    while (!empty())
      pop_front();
  }

  void insert_after(const_iterator pos, const T &x) { emplace_after(pos, x); }

  void insert_after(const_iterator pos, T &&x) {
    emplace_after(pos, std::move(x));
  }

  template <typename... Types>
  iterator emplace_after(const_iterator pos, Types &&...args) {
    auto &prior_next = pos.base()->next;
    s_create_after(prior_next, std::forward<Types>(args)...);
    return iterator(prior_next);
  }

  iterator erase_after(const_iterator pos) {
    auto &prior_next = pos.base()->next;
    s_delete_after(prior_next);
    return iterator(prior_next);
  }

  iterator erase_after(const_iterator pos, const_iterator last) {
    while (std::next(pos) != last)
      erase_after(pos);
    return iterator(pos.base()->next);
  }

  bool empty() const { return !m_head; }

  size_type size() const {
    return static_cast<size_type>(std::distance(begin(), end()));
  }

  reference front() { return m_head->value; }
  const_reference front() const { return m_head->value; }

  bool operator==(const Slist<T> &other) const {
    return std::equal(begin(), end(), other.begin(), other.end());
  }

  auto operator<=>(const Slist<T> &other) const {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(),
                                                  other.end());
  }

  void merge(Slist<T> &other) { merge(std::move(other)); }

  void merge(Slist<T> &&other) { merge(std::move(other), std::less<>{}); }

  void merge(Slist<T> &other, auto compare) {
    merge(std::move(other), compare);
  }

  void merge(Slist<T> &&other, auto compare) {
    if (&other == this)
      return;
    for (auto *prior_next = &m_head; !other.empty();
         prior_next = &(*prior_next)->next) {
      while (*prior_next && !compare(other.front(), (*prior_next)->value))
        prior_next = &(*prior_next)->next;
      prior_next->swap(other.m_head);
      other.m_head.swap((*prior_next)->next);
    }
  }

  void sort() { sort(std::less<>{}); }

  void sort(auto compare) { sort_impl(size(), compare); }

private:
  void sort_impl(size_type size, auto compare) {
    if (size <= 1)
      return;
    auto half = size / 2;
    auto mid = std::next(begin(), half - 1);
    Slist<T> list2;
    list2.m_head.swap(mid.base()->next);
    sort_impl(half, compare);
    list2.sort_impl(size - half, compare);
    merge(list2, compare);
  }
};

template <std::input_iterator Iterator>
Slist(Iterator, Iterator)
    -> Slist<typename std::iterator_traits<Iterator>::value_type>;

namespace std {
template <typename T> void swap(Slist<T> &a, Slist<T> &b) noexcept {
  a.swap(b);
}
} // namespace std

#endif // CS101_SLIST_HPP