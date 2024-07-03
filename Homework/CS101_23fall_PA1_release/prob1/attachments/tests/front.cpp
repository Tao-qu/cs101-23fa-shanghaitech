#include "../slist.hpp"

#include <iomanip>
#include <iostream>
#include <string>

int main() {
  Slist<std::string> letters;

  letters.push_front("abc");
  std::string s{"def"};
  letters.push_front(std::move(s));

  letters.emplace_front();
  letters.emplace_front(10, 'c');

  std::cout << "Slist letters holds: ";
  for (auto &&e : letters)
    std::cout << std::quoted(e) << ' ';

  std::cout << "\nMoved-from string s holds: " << std::quoted(s) << '\n';

  Slist<char> chars{'A', 'B', 'C', 'D'};

  for (; !chars.empty(); chars.pop_front())
    std::cout << "chars.front(): '" << chars.front() << "'\n";
}