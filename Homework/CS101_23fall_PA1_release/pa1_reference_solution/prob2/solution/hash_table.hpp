#ifndef CS10123F_PA1_HASHTABLE_HPP
#define CS10123F_PA1_HASHTABLE_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "hash_types.hpp"

template <> struct std::hash<Website> {
  static constexpr uint32_t base = 1009u; // 1009 is a prime number.

  uint32_t operator()(const Website &S) const noexcept {
    uint32_t value = 0u, len = S.name.size();
    for (auto i = 0u; i < len; ++i)
      value = value * base + S.name[i];
    return value;
  }
};

// implement Insert, find, Lazy erase, clear
template <class Key = Website, class T = IPv4> class HashTable {
private:
  enum class EntryState { ES_Unused, ES_Used, ES_Erased };
  static constexpr std::size_t Mod = 1000037; // 20141111
  std::vector<T> table;
  std::vector<Key> stored_key;
  std::vector<EntryState> states;
  std::vector<std::size_t> used_pos;

public:
  static constexpr std::size_t npos = -1;

  HashTable() : table(Mod), stored_key(Mod), states(Mod, EntryState::ES_Unused) {}

  std::size_t probe_pos(std::size_t Start, std::size_t i) const {
    return (Start + i) % Mod;
  }
  std::size_t Search(const Key &x) const {
    std::size_t Start = std::hash<Key>{}(x) % Mod;
    std::size_t hole = npos;
    for (std::size_t i = 0; i < Mod; ++i) {
      std::size_t pos = probe_pos(Start, i);
      if (states[pos] == EntryState::ES_Used && stored_key[pos] == x)
        return pos;
      if (states[pos] == EntryState::ES_Erased && hole == npos)
        hole = pos;
      if (states[pos] == EntryState::ES_Unused)
        return hole == npos ? pos : hole;
    }
    return hole;
  }
  std::size_t find(const Key &x) const {
    std::size_t pos = Search(x);
    if (states[pos] != EntryState::ES_Used)
      return npos;
    return pos;
  }
  void insert(const Key &x, const T &value) {
    std::size_t pos = Search(x);
    if (states[pos] == EntryState::ES_Unused) {
      states[pos] = EntryState::ES_Used;
      stored_key[pos] = x;
      used_pos.push_back(pos);
    } else if (states[pos] == EntryState::ES_Erased) {
      states[pos] = EntryState::ES_Used;
      stored_key[pos] = x;
    }
    table[pos] = value;
  }
  bool erase(const Key &x) {
    std::size_t pos = Search(x);
    if (states[pos] != EntryState::ES_Used)
      return false;
    states[pos] = EntryState::ES_Erased;
    return true;
  }

  void clear() {
    for (auto pos : used_pos)
      states[pos] = EntryState::ES_Unused;
    used_pos.clear();
  }

  const T &operator[](const Key &x) const {
    auto pos = find(x);
    if (pos == npos)
      throw std::invalid_argument("Key does not exist");
    return table[pos];
  }
};

#endif // CS10123F_PA1_HASHTABLE_HPP
// HashTable<int, int> mytable;

// // testing std
// std::unordered_map<int, int> stdtable;
// int Rand() { return rand() << 16 | rand(); }
// constexpr auto U = 500000
// int main() {
//   size_t hash_value =
//   std::hash<Website>{}(Website(std::string("acm.shanghaitech.edu.cn")));
//   std::cout << "Your hash_value: " << hash_value << '\n';
//   // srand(time(0) + 20011111);
//   // for (int i = 0; i < U; ++i) {
//   //   int Key = Rand(), Value = Rand();
//   //   bool SS = stdtable.find(Key) != stdtable.end();
//   //   bool VV = mytable.find(Key) != mytable.npos;
//   //   if (SS != VV)
//   //     return !printf("GG! at %d", i);
//   //   if (!SS) {
//   //     stdtable[Key] = Value;
//   //     mytable.insert(Key, Value);
//   //   } else {
//   //     if (stdtable[Key] != mytable[Key])
//   //       return !printf("GG at %d", i);
//   //     if ((rand() & 7)) {
//   //       stdtable.erase(Key);
//   //       mytable.erase(Key);
//   //     }
//   //   }
//   // }
//   // for (auto &x : stdtable) {
//   //   int Key = x.first, Value = x.second;
//   //   if (mytable[Key] != Value)
//   //     printf("GG");
//   // }
//   return 0;
// }