#include "../slist.hpp"

struct SpecialMemberCountedType {
  static int default_ctor_count, copy_ctor_count, copy_assignment_count,
      move_ctor_count, move_assignment_count;
  SpecialMemberCountedType() { ++default_ctor_count; }
  SpecialMemberCountedType(const SpecialMemberCountedType &) {
    ++copy_ctor_count;
  }
  SpecialMemberCountedType(SpecialMemberCountedType &&) { ++move_ctor_count; }
  SpecialMemberCountedType &operator=(const SpecialMemberCountedType &) {
    ++copy_assignment_count;
    return *this;
  }
  SpecialMemberCountedType &operator=(SpecialMemberCountedType &&) {
    ++move_assignment_count;
    return *this;
  }
  // Add other constructors if you need ...
};

int SpecialMemberCountedType::default_ctor_count{},
    SpecialMemberCountedType::copy_ctor_count{},
    SpecialMemberCountedType::copy_assignment_count{},
    SpecialMemberCountedType::move_ctor_count{},
    SpecialMemberCountedType::move_assignment_count{};

void clear() {
  SpecialMemberCountedType::default_ctor_count = 0;
  SpecialMemberCountedType::copy_ctor_count = 0;
  SpecialMemberCountedType::copy_assignment_count = 0;
  SpecialMemberCountedType::move_ctor_count = 0;
  SpecialMemberCountedType::move_assignment_count = 0;
}

int main() {
  // Test the functions on Slist<SpecialMemberCountedType>
  // Write your own tests here ...
}