#include "slist.hpp"

#include <array>
#include <concepts>
#include <iostream>
#include <iterator>
#include <memory>

struct EmptyType {};

struct LargeType {
  std::array<std::byte, 4096> content;
};

using ArithmeticType = long double;

using STLType = std::string;

struct UncopyableType {
  UncopyableType(const UncopyableType &) = delete;
  UncopyableType &operator=(const UncopyableType &) = delete;
};

struct OnlyCopyConstructibleType {
  OnlyCopyConstructibleType() = delete;
  OnlyCopyConstructibleType(const OnlyCopyConstructibleType &) {}
  OnlyCopyConstructibleType &
  operator=(const OnlyCopyConstructibleType &) = delete;
  ~OnlyCopyConstructibleType() {}
};

struct EqualityComparableType {
  bool operator==(const EqualityComparableType &) const = default;
};

struct ThreeWayComparableType {
  auto operator<=>(const ThreeWayComparableType &) const = default;
};

template <typename T> struct SomeInputIterator {
  using value_type = T;
  using reference = const T &;
  using pointer = const T *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  reference operator*() const;
  pointer operator->() const;
  SomeInputIterator &operator++();
  SomeInputIterator operator++(int);
  bool operator==(const SomeInputIterator<T> &) const = default;
};

struct Arg0Type {
  Arg0Type() = delete;
  Arg0Type(std::string) {}
};
struct Arg1Type {
  explicit Arg1Type() = default;
};
struct Arg2Type {};

struct SpeciallyConstructedType {
  explicit SpeciallyConstructedType(Arg0Type, const Arg1Type &, Arg2Type &&);
};

struct TypeWithId {
  int id;

  struct Comparator {
    bool operator()(const TypeWithId &a, const TypeWithId &b) const {
      return a.id < b.id;
    }
  };
};

template <typename T, typename From>
concept explicitly_constructible_from =
    std::constructible_from<T, From> && !std::convertible_to<From, T>;

namespace iterator_checks {

template <typename IterInstance, typename T, bool IsConst>
concept MemberTypesCorrectOnImpl =
    requires {
      typename IterInstance::value_type;
      typename IterInstance::difference_type;
      typename IterInstance::reference;
      typename IterInstance::pointer;
      typename IterInstance::iterator_category;
    } && std::same_as<typename IterInstance::value_type, T> &&
    std::same_as<typename IterInstance::difference_type, std::ptrdiff_t> &&
    std::same_as<typename IterInstance::reference,
                 std::conditional_t<IsConst, const T &, T &>> &&
    std::same_as<typename IterInstance::pointer,
                 std::conditional_t<IsConst, const T *, T *>> &&
    std::same_as<typename IterInstance::iterator_category,
                 std::forward_iterator_tag>;

template <template <typename, bool> class Iterator, typename T>
concept MemberTypesCorrectOn =
    MemberTypesCorrectOnImpl<Iterator<T, true>, T, true> &&
    MemberTypesCorrectOnImpl<Iterator<T, false>, T, false>;

template <template <typename, bool> class Iterator, typename... T>
concept SizeCorrectOn =
    ((sizeof(Iterator<T, true>) == sizeof(std::unique_ptr<T> *) &&
      sizeof(Iterator<T, false>) == sizeof(std::unique_ptr<T> *)) &&
     ...);

template <template <typename, bool> class Iterator, typename... T>
concept AllowsAddingLowLevelConstOn =
    (std::convertible_to<Iterator<T, false>, Iterator<T, true>> && ...);

template <typename IterInstance, typename T, bool IsConst>
concept InterfacesCorrectOnImpl =
    std::default_initializable<IterInstance> &&
    explicitly_constructible_from<IterInstance,
                                  const std::unique_ptr<SlistNode<T>> &> &&
    std::is_trivially_copyable_v<IterInstance> &&
    std::is_trivially_move_constructible_v<IterInstance> &&
    std::is_trivially_move_assignable_v<IterInstance> &&
    std::is_trivially_destructible_v<IterInstance> &&
    requires(IterInstance i, const IterInstance ci) {
      { *i } -> std::same_as<typename IterInstance::reference>;
      { *ci } -> std::same_as<typename IterInstance::reference>;
      { i.operator->() } -> std::same_as<typename IterInstance::pointer>;
      { ci.operator->() } -> std::same_as<typename IterInstance::pointer>;
      { ++i } -> std::same_as<IterInstance &>;
      { i++ } -> std::same_as<IterInstance>;
    };

template <template <typename, bool> class Iterator, typename... T>
concept InterfacesCorrectOn = ((
    InterfacesCorrectOnImpl<Iterator<T, true>, T, true> &&
    InterfacesCorrectOnImpl<Iterator<T, false>, T, false> &&
    std::equality_comparable_with<Iterator<T, true>, Iterator<T, false>> &&
    std::equality_comparable_with<Iterator<T, false>, Iterator<T, true>>)&&...);

template <template <typename, bool> class Iterator>
concept Correct = MemberTypesCorrectOn<Iterator, EmptyType> &&
                  SizeCorrectOn<Iterator, EmptyType, LargeType> &&
                  AllowsAddingLowLevelConstOn<Iterator, EmptyType, LargeType> &&
                  InterfacesCorrectOn<Iterator, EmptyType, LargeType>;

} // namespace iterator_checks

namespace container_checks {

template <typename ListInstance, typename T>
concept MemberTypesCorrectOnImpl =
    requires {
      typename ListInstance::value_type;
      typename ListInstance::size_type;
      typename ListInstance::difference_type;
      typename ListInstance::reference;
      typename ListInstance::const_reference;
      typename ListInstance::pointer;
      typename ListInstance::const_pointer;
      typename ListInstance::iterator;
      typename ListInstance::const_iterator;
    } && std::same_as<typename ListInstance::value_type, T> &&
    std::same_as<typename ListInstance::size_type, std::size_t> &&
    std::same_as<typename ListInstance::difference_type, std::ptrdiff_t> &&
    std::same_as<typename ListInstance::reference, T &> &&
    std::same_as<typename ListInstance::const_reference, const T &> &&
    std::same_as<typename ListInstance::pointer, T *> &&
    std::same_as<typename ListInstance::const_pointer, const T *> &&
    std::same_as<typename ListInstance::iterator, SlistIterator<T, false>> &&
    std::same_as<typename ListInstance::const_iterator, SlistIterator<T, true>>;

template <template <typename> class List, typename... T>
concept MemberTypesCorrectOn = (MemberTypesCorrectOnImpl<List<T>, T> && ...);

template <template <typename> class List, typename T>
concept SizeCorrectImpl =
    (sizeof(List<T>) == sizeof(std::unique_ptr<SlistNode<T>>));

template <template <typename> class List, typename... T>
concept SizeCorrectOn = (SizeCorrectImpl<List, T> && ...);

template <typename ListInstance, typename T>
concept InterfacesCorrectOnImpl =
    std::default_initializable<ListInstance> &&
    std::is_nothrow_move_constructible_v<ListInstance> &&
    std::is_nothrow_move_assignable_v<ListInstance> &&
    !std::is_nothrow_copy_constructible_v<ListInstance> &&
    !std::is_nothrow_copy_assignable_v<ListInstance> &&
    std::constructible_from<ListInstance, std::size_t, const T &> &&
    explicitly_constructible_from<ListInstance, std::size_t> &&
    std::constructible_from<ListInstance, SomeInputIterator<T>,
                            SomeInputIterator<T>> &&
    std::constructible_from<ListInstance, std::initializer_list<T>> &&
    requires(ListInstance list, const ListInstance clist, T x, const T cx,
             typename ListInstance::const_iterator pos) {
      { list.begin() } -> std::same_as<typename ListInstance::iterator>;
      { clist.begin() } -> std::same_as<typename ListInstance::const_iterator>;
      { list.cbegin() } -> std::same_as<typename ListInstance::const_iterator>;
      { clist.cbegin() } -> std::same_as<typename ListInstance::const_iterator>;
      { list.end() } -> std::same_as<typename ListInstance::iterator>;
      { clist.end() } -> std::same_as<typename ListInstance::const_iterator>;
      { list.cend() } -> std::same_as<typename ListInstance::const_iterator>;
      { clist.cend() } -> std::same_as<typename ListInstance::const_iterator>;
      { list.push_front(x) } -> std::same_as<void>;
      { list.push_front(cx) } -> std::same_as<void>;
      { list.push_front(std::move(x)) } -> std::same_as<void>;
      { list.pop_front() } -> std::same_as<void>;
      { list.clear() } -> std::same_as<void>;
      { list.insert_after(pos, x) } -> std::same_as<void>;
      { list.insert_after(pos, cx) } -> std::same_as<void>;
      { list.insert_after(pos, std::move(x)) } -> std::same_as<void>;
      {
        list.erase_after(pos)
      } -> std::same_as<typename ListInstance::iterator>;
      {
        list.erase_after(pos, pos)
      } -> std::same_as<typename ListInstance::iterator>;
      { list.empty() } -> std::same_as<bool>;
      { clist.empty() } -> std::same_as<bool>;
      { list.size() } -> std::same_as<std::size_t>;
      { clist.size() } -> std::same_as<std::size_t>;
      { list.front() } -> std::same_as<T &>;
      { clist.front() } -> std::same_as<const T &>;
    } &&
    std::swappable<ListInstance>;

template <template <typename> class List, typename... T>
concept InterfacesCorrectOn = (InterfacesCorrectOnImpl<List<T>, T> && ...);

template <typename ListInstance, typename T, typename... Args>
concept EmplaceCorrectOn =
    requires(ListInstance list, typename ListInstance::const_iterator pos,
             Args... args) {
      { list.emplace_front(std::forward<Args>(args)...) } -> std::same_as<T &>;
      {
        list.emplace_after(pos, std::forward<Args>(args)...)
      } -> std::same_as<typename ListInstance::iterator>;
    };

template <template <typename> class List>
concept EmplaceCorrect =
    EmplaceCorrectOn<List<EmptyType>, EmptyType> &&
    EmplaceCorrectOn<List<SpeciallyConstructedType>, SpeciallyConstructedType,
                     Arg0Type, const Arg1Type &, Arg2Type &&>;

template <typename ListInstance, typename Compare>
concept MergeSortCompareCorrectImpl = requires(ListInstance l, Compare cmp) {
  { l.merge(l, cmp) } -> std::same_as<void>;
  { l.merge(std::move(l), cmp) } -> std::same_as<void>;
  { l.sort(cmp) } -> std::same_as<void>;
};

template <typename ListInstance>
concept MergeSortCorrectImpl = requires(ListInstance l) {
  { l.merge(l) } -> std::same_as<void>;
  { l.merge(std::move(l)) } -> std::same_as<void>;
  { l.sort() } -> std::same_as<void>;
};

template <template <typename> class List>
concept MergeSortCorrect =
    MergeSortCorrectImpl<List<int>> &&
    MergeSortCorrectImpl<List<ThreeWayComparableType>> &&
    MergeSortCorrectImpl<List<std::string>> &&
    MergeSortCompareCorrectImpl<List<int>, std::ranges::less> &&
    MergeSortCompareCorrectImpl<List<std::string>,
                                decltype([](const std::string &a,
                                            const std::string &b) {
                                  return a.size() < b.size();
                                })> &&
    MergeSortCompareCorrectImpl<List<TypeWithId>, TypeWithId::Comparator>;

template <template <typename> class List>
concept Correct =
    MemberTypesCorrectOn<List, EmptyType, LargeType, ArithmeticType, STLType> &&
    SizeCorrectOn<List, EmptyType, LargeType, ArithmeticType, STLType> &&
    InterfacesCorrectOn<List, EmptyType, LargeType, ArithmeticType, STLType,
                        UncopyableType> &&
    std::copyable<List<OnlyCopyConstructibleType>> && EmplaceCorrect<List> &&
    std::equality_comparable<List<EqualityComparableType>> &&
    std::three_way_comparable<List<ThreeWayComparableType>> &&
    MergeSortCorrect<List>;

} // namespace container_checks

static_assert(iterator_checks::Correct<SlistIterator>);

static_assert(container_checks::Correct<Slist>);

int main() {
  std::cout << "Compile-time checks passed." << std::endl;
  return 0;
}