#ifndef HASHTABLE_HASH_TYPE_HPP
#define HASHTABLE_HASH_TYPE_HPP

#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>

class Website {
private:
  std::string name;
  friend class std::hash<Website>;

public:
  Website() {}
  Website(std::string NAME) : name(std::move(NAME)) {}
  bool operator == (const Website &rhs) const { return name == rhs.name; }
  friend std::ostream& operator << (std::ostream &, const Website &);
};

std::ostream &operator<<(std::ostream &out, const Website &rhs) {
  return out << rhs.name;
}

class IPv4 {
private:
  uint32_t IP;

public:
  IPv4(uint32_t _ip = 0) : IP(_ip) {}
  friend std::ostream &operator<<(std::ostream &, const IPv4 &);
  bool operator == (const IPv4 &rhs) const { return IP==rhs.IP;}
};

std::ostream &operator<<(std::ostream &out, const IPv4 &rhs) {
  uint32_t A = (rhs.IP >> 24) & 255, B = (rhs.IP >> 16) & 255,
           C = (rhs.IP >> 8) & 255, D = rhs.IP & 255;
  out << A << '.' << B << '.' << C << '.' << D;
  return out;
}

#endif //HASHTABLE_HASH_TYPE_HPP