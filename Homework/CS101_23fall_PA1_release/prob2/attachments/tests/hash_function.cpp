#include "../hash_table.hpp"

#include <iostream>

int main()
{
    std::size_t hash_value1 = std::hash<Website>{}(Website(std::string("abcde")));
    
    std::cout << "hash on string abcde\n";
    std::cout << "Your hash_value: " << hash_value1 << '\n';
    std::cout << "Expected  value: " << 522585007 << '\n';

    std::size_t hash_value2 = std::hash<Website>{}(Website(std::string("acm.shanghaitech.edu.cn")));
    std::cout << "hash on string acm.shanghaitech.edu.cn\n";
    std::cout << "Your hash_value: " << hash_value2 << '\n';
    std::cout << "Expected  value: " << 3929495537 << '\n';
}