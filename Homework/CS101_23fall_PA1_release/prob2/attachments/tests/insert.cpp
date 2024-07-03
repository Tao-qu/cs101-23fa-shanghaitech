#include "../hash_table.hpp"

#include <iostream>

int main()
{
    HashTable<Website, IPv4> table;
    Website acm(std::string("acm.shanghaitech.edu.cn"));
    IPv4 ip(168760789);
    table.insert(acm, ip);
    
    std::cout << "from hash table find ip of " << acm <<  " is " << table[acm] << '\n';
    std::cout << "expected ip : " << ip << '\n';
}