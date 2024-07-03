#include "../bst.hpp"
#include <cassert>

int main()
{
    std::size_t n = 293 + 180;
    init(n);
    
    for (int i=10;i<=100;++i)
        SetOperation(1, i);
    
    for (int i=10;i<=100;++i)
        assert(SetOperation(3,i)==true);

    for (int i=10;i<=20;++i)
        SetOperation(2, i);
    
    for (int i=200;i>100;--i)
        SetOperation(1, i);
    
    for (int i=21;i<=200;++i)
        assert(SetOperation(3,i)==true);
    printf("Accepted!");
}