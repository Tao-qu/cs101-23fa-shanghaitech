#include "../bst.hpp"
#include <cassert>

int main()
{
    std::size_t n = 21;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 6);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(1, 7);

    for (int i = 1; i <= 7; ++i)
        assert(SetOperation(4, i) == i);

    SetOperation(2, 1);
    SetOperation(2, 5);
    assert(SetOperation(4, 1) == 2);
    assert(SetOperation(4, 2) == 3);
    assert(SetOperation(4, 3) == 4);
    assert(SetOperation(4, 4) == 6);
    assert(SetOperation(4, 5) == 7);
    printf("Accepted!");
}