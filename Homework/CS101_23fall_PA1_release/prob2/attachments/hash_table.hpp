#ifndef CS10123F_PA1_HASHTABLE
#define CS10123F_PA1_HASHTABLE

#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "hash_types.hpp"

template <>
struct std::hash<Website>
{
  static const uint32_t base = 1009u; // 1009 is a prime number.
  const uint64_t mod = 1llu << 32;
  uint32_t operator()(const Website &S) const noexcept
  {
    // ------------------------START YOUR CODE----------------
    uint32_t hash_value = 0;
    uint32_t base_power = 1;
    for (int i = S.name.length(); i > 0; --i)
    {
      hash_value = (hash_value + S.name[i - 1] * base_power) % mod;
      base_power = (base_power * base) % mod;
    }
    return hash_value;
    // ------------------------END OF YOUR CODE----------------
  }
};

template <class Key = Website, class T = IPv4> // template for key type class
                                               // Key and value type class T
class HashTable
{
private:
  enum entrystate
  {
    Unused,
    Used,
    Erased
  };
  const size_t Mod = 1000037u;   // 1000037 is a prime number
  std::vector<T> Table;          // main table stores value
  std::vector<Key> Stored_Key;   // used for storing original key
  std::vector<entrystate> State; // keeps the state of every table entry : {Unused, Used or Erased};
  std::vector<size_t> Used_pos;  // hint: used for function clear()

public:
  static const std::size_t npos = -1;
  HashTable() : Table(Mod), Stored_Key(Mod), State(Mod, entrystate::Unused) {}

  const T &operator[](const Key &x) const
  {
    std::size_t pos = find(x);
    if (pos == npos)
      throw std::invalid_argument("Key does not exist");
    return Table[pos];
  }

  // ------------------------START YOUR CODE----------------

  std::size_t Search(const Key &x) const
  {
    std::size_t Start = std::hash<Key>{}(x) % Mod;
    std::size_t pos = Start;
    while (State[pos] != entrystate::Unused && Stored_Key[pos] != x)
    {
      pos = (pos + 1) % Mod;
      if (pos == Start)
      {
        while (State[pos] != entrystate::Erased)
        {
          pos = (pos + 1) % Mod;
          if (pos == Start)
            return npos;
        }
        return pos;
      }
    }
    return pos;
  }

  std::size_t find(const Key &x) const
  {
    std::size_t pos = Search(x);
    return (State[pos] == entrystate::Used && Stored_Key[pos] == x) ? pos : npos;
  }

  void insert(const Key &x, const T &value)
  {
    std::size_t pos = Search(x);
    Table[pos] = value;
    Stored_Key[pos] = x;
    State[pos] = entrystate::Used;
    Used_pos.push_back(pos);
  }

  bool erase(const Key &x)
  {
    std::size_t pos = find(x);
    if (pos == npos)
      return false;
    State[pos] = entrystate::Erased;
    return true;
  }

  void clear()
  {
    for (std::size_t pos : Used_pos)
    {
      State[pos] = entrystate::Unused;
    }
    Used_pos.clear();
  }
};

#endif // CS10123F_PA1_HASHTABLE