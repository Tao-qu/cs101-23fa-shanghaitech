#include "../hash_table.hpp"

#include <iostream>

int main()
{
    HashTable<Website, IPv4> table;
    Website acm(std::string("acm.shanghaitech.edu.cn"));
    IPv4 ip(168760789);
    table.insert(acm, ip);
    table.erase(acm);

    std::size_t pos = table.find(acm);
    
    std::cout << "from hash table find position of " << acm <<  " is " << pos << '\n';
    std::cout << "expected position is (unsigned)-1 : " << (size_t)(-1) << '\n';
}