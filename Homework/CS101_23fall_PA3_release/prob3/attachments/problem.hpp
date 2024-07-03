#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <vector>
#include <algorithm>
#include <cstdint>

class Problem {
public:
  using VarID = std::size_t;
  using Value = std::int64_t;

  /// @brief A solution is a vector of values, in which the @a i -th element is
  /// the value assigned to the variable @a x_i .
  using Solution = std::vector<Value>;

  /// @brief A constraint 'var1 - var2 <= constant'.
  struct Constraint {
    VarID var1, var2;
    Value constant;

    constexpr bool isSatisfied(const Solution &solution) const {
      return solution[var1] - solution[var2] <= constant;
    }
  };

private:
  VarID mNumVars;
  std::vector<Constraint> mConstraints;

public:
  explicit constexpr Problem(VarID numVars) : mNumVars{numVars} {}
  constexpr Problem(VarID numVars, std::vector<Constraint> constraints)
      : mNumVars(numVars), mConstraints(std::move(constraints)) {}

  constexpr void addConstraint(VarID var1, VarID var2, Value constant) {
    mConstraints.push_back({.var1 = var1, .var2 = var2, .constant = constant});
  }
  constexpr void addConstraint(const Constraint &constraint) {
    mConstraints.push_back(constraint);
  }

  /// @brief Checks whether any of the constraints has a negative constant term.
  constexpr bool hasNegativeConstant() const {
    return std::ranges::any_of(
        mConstraints, [](Value v) { return v < 0; }, &Constraint::constant);
  }

  constexpr auto getNumVars() const { return mNumVars; }
  constexpr const auto &getConstraints() const { return mConstraints; }

  constexpr bool isFeasible(const Solution &solution) const {
    return std::ranges::all_of(mConstraints, [&solution](const Constraint &ct) {
      return ct.isSatisfied(solution);
    });
  }
};

#endif // PROBLEM_HPP