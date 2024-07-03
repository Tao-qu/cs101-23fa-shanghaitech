#include "../bst.hpp"
#include <cassert>

int main()
{
    std::size_t n = 22;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 3);
    SetOperation(1, 4);

    assert(SetOperation(3, 1) == true);
    assert(SetOperation(3, 2) == true);
    assert(SetOperation(3, 3) == true);
    assert(SetOperation(3, 4) == true);
    assert(SetOperation(3, 5) == true);
    assert(SetOperation(3, 6) == false);

    SetOperation(1, 6);
    SetOperation(1, 10);
    SetOperation(1, 8);
    SetOperation(1, 7);
    SetOperation(1, 9);

    assert(SetOperation(3, 6) == true);
    assert(SetOperation(3, 7) == true);
    assert(SetOperation(3, 8) == true);
    assert(SetOperation(3, 9) == true);
    assert(SetOperation(3, 10) == true);
    assert(SetOperation(3, 10000) == false);

    printf("Accepted!");
}