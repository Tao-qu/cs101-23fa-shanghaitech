#include "hash_table.hpp"

#include <array>
#include <concepts>
#include <iostream>
#include <iterator>
#include <functional>
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

template <typename T, typename From>
concept explicitly_constructible_from =
    std::constructible_from<T, From> && !std::convertible_to<From, T>;

template <typename HashTableInstance, typename Key, typename Value>
concept InterfacesCorrectOnImpl = 
    std::default_initializable<HashTableInstance> && 
    requires(HashTableInstance table, const HashTableInstance ctable, 
             Key key, const Key ckey, Value x, const Value cx){
        { table[key] } -> std::same_as<const Value &>;
        { ctable[key] } -> std::same_as<const Value &>;
        { table[ckey] } -> std::same_as<const Value &>;
        { ctable[ckey] } -> std::same_as<const Value &>;

        { ctable.find(key) } -> std::same_as<std::size_t>;
        { ctable.find(ckey)} -> std::same_as<std::size_t>;

        { table.insert(key,x) } -> std::same_as<void>;
        { table.insert(key,cx) } -> std::same_as<void>;
        { table.insert(ckey,x) } -> std::same_as<void>;
        { table.insert(ckey,cx) } -> std::same_as<void>;

        { table.erase(key)}  ->  std::same_as<bool>;
        { table.erase(ckey)} ->  std::same_as<bool>;

        { table.clear()}    ->  std::same_as<void>;
        { std::hash<Key>{}(key)}  -> std::integral<>;
        { std::hash<Key>{}(ckey)} -> std::integral<>;
    };

template <template <typename,typename> class Table, typename Key, typename ...T>
concept InterfacesCorrectOnKey = (InterfacesCorrectOnImpl<Table<Key, T>, Key, T> && ...);

template <template <typename,typename> class Table, typename... Key>
concept InterfacesCorrectOn = (InterfacesCorrectOnKey<Table, Key, 
                        EmptyType, LargeType, ArithmeticType, STLType, IPv4> && ...);

template <template <typename,typename> class Table>
concept Correct = InterfacesCorrectOn<Table, std::string, Website>;

static_assert(Correct<HashTable>);

int main()
{
    std::cout << "Compile-time checks passed." << std::endl;
    return 0;
}