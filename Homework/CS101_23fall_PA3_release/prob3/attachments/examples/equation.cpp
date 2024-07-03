#include <optional>
#include <iostream>
#include <utility>
#include <cmath>

/// @brief Solve the equation @a ax^2+bx+c=0 in the field of real numbers.
/// @return A pair of solutions @a (x1,x2), or @c std::nullopt if no solutions.
std::optional<std::pair<double, double>> solve(double a, double b, double c) {
  auto delta = b * b - 4 * a * c;
  if (delta < 0)
    return std::nullopt;
  auto sqrtDelta = std::sqrt(delta);
  return std::pair{(-b - sqrtDelta) / (2 * a), (-b + sqrtDelta) / (2 * a)};
}

void printSolution(const std::optional<std::pair<double, double>> &sln) {
  // conversion to bool tests whether it contains an object (a solution)
  if (sln) {
    // 'auto [x1, x2] = ...' is the syntax of "structured binding", which means
    //     auto tmp = sln.value();
    //     auto x1 = tmp.first;
    //     auto x2 = tmp.second;
    auto [x1, x2] = sln.value(); // .value() returns the contained object.
    std::cout << "The solutions are " << x1 << " and " << x2 << '.'
              << std::endl;
  } else
    std::cout << "No solutions." << std::endl;
}

int main() {
  auto sln1 = solve(1, -2, -3);
  printSolution(sln1);
  auto sln2 = solve(1, 0, 1);
  printSolution(sln2);
  return 0;
}