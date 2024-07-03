#include "../slist.hpp"

struct OnlyDefaultConstructibleType
{
  OnlyDefaultConstructibleType() = default;
  OnlyDefaultConstructibleType(const OnlyDefaultConstructibleType &) = delete;
  OnlyDefaultConstructibleType &operator=(const OnlyDefaultConstructibleType &) = delete;
  OnlyDefaultConstructibleType(OnlyDefaultConstructibleType &&) = delete;
  OnlyDefaultConstructibleType &operator=(OnlyDefaultConstructibleType &&) = delete;
};

int main()
{
  Slist<OnlyDefaultConstructibleType> list(10); // Your code must compile.
  return 0;
}