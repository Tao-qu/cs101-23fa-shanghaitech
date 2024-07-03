#include "../slist.hpp"

#include <iostream>
#include <string>

struct Sum
{
  std::string remark;
  int sum;

  Sum(std::string remark, int sum) : remark{std::move(remark)}, sum{sum} {}

  void print() const
  {
    std::cout << remark << " = " << sum << '\n';
  }
};

int main()
{
  Slist<Sum> list = {Sum("0", 0)};

  auto iter = list.begin();
  std::string str{"1"};

  for (int i{1}, sum{1}; i != 10; sum += i)
  {
    iter = list.emplace_after(iter, str, sum);
    ++i;
    str += " + " + std::to_string(i);
  }

  for (const Sum &s : list)
    s.print();
}