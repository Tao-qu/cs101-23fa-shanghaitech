#ifndef HASH_TYPES_HPP
#define HASH_TYPES_HPP

#include <string>
#include <iostream>
#include <functional>
#include <cstdint>

class Website {
private:
  std::string name;

public:
  Website() {}
  Website(std::string name_) : name(std::move(name_)) {}
  bool operator==(const Website &rhs) const { return name == rhs.name; }

  friend std::ostream &operator<<(std::ostream &, const Website &);
  friend class std::hash<Website>;
};

std::ostream &operator<<(std::ostream &out, const Website &rhs) {
  return out << rhs.name;
}

class IPv4 {
private:
  uint32_t ip;

public:
  IPv4(uint32_t ip_ = 0) : ip(ip_) {}
  bool operator==(const IPv4 &rhs) const { return ip == rhs.ip; }

  friend std::ostream &operator<<(std::ostream &, const IPv4 &);
};

std::ostream &operator<<(std::ostream &out, const IPv4 &rhs) {
  uint32_t A = (rhs.ip >> 24) & 255, B = (rhs.ip >> 16) & 255,
           C = (rhs.ip >> 8) & 255, D = rhs.ip & 255;
  out << A << '.' << B << '.' << C << '.' << D;
  return out;
}

#endif // HASH_TYPES_HPP