#include "E:\Users\Tao_qu\Documents\WeChat Files\wxid_wfahqzv5abc222\FileStorage\File\2023-12\bst.hpp"
#include <cassert>

int main()
{
    std::size_t n = 19;
    init(n);
    SetOperation(1, 1988291);
    SetOperation(1, 2188418);
    SetOperation(1, 2270563);
    SetOperation(1, 2577987);
    SetOperation(1, 2802989);
    SetOperation(1, 3512557);
    SetOperation(1, 4099911);
    SetOperation(1, 4563210);
    SetOperation(1, 4744313);
    std::cout << SetOperation(5, 98) << std::endl;
}

// int main()
// {
//     std::size_t n = 16;
//     init(n);

//     SetOperation(1, 1);
//     SetOperation(1, 2);
//     SetOperation(1, 5);
//     SetOperation(1, 6);
//     SetOperation(1, 3);
//     SetOperation(1, 4);
//     SetOperation(1, 7);

//     assert(SetOperation(5, 1) == 2);
//     assert(SetOperation(5, 2) == 2);
//     assert(SetOperation(5, 3) == 12);
//     assert(SetOperation(5, 4) == 2);

//     SetOperation(2, 1);
//     SetOperation(2, 5);
//     assert(SetOperation(5, 1) == 8);
//     assert(SetOperation(5, 2) == 5);
//     assert(SetOperation(5, 10) == 0);
//     printf("Accepted!");
// }