#include "../bst.hpp"
#include <cassert>

int main()
{
    std::size_t n = 19;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 6);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(1, 7);

    SetOperation(2, 4);
    assert(SetOperation(3, 4) == false);

    SetOperation(1, 3);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(2, 5);
    assert(SetOperation(3, 5) == false);

    SetOperation(2, 1);
    assert(SetOperation(3, 1) == false);
    assert(SetOperation(3, 2) == true);
    SetOperation(2, 2);
    assert(SetOperation(3, 2) == false);
    printf("Accepted!");
}