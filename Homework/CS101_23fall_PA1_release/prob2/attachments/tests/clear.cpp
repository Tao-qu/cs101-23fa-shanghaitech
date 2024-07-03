#include "../hash_table.hpp"

#include <iostream>

int main()
{
    HashTable<Website, IPv4> table;
    Website s1(std::string("string1")), s2(std::string("string2")), s3(std::string("string3"));
    IPv4 ip1(1),ip2(2),ip3(3);
    table.insert(s1, ip1);
    table.insert(s2, ip2);
    table.insert(s3, ip3);
    
    table.clear();

    std::size_t pos = table.find(s2);

    std::cout << "from hash table find position of " << s2 <<  " is " << pos << '\n';
    std::cout << "expected position is (unsigned)-1 : " << (size_t)(-1) << '\n' << '\n';

    std::cout << "after insertion:\n";
    table.insert(s2, IPv4(222));
    std::cout << "the ip of " << s2 << " is " << table[s2] << '\n';
    std::cout << "expected ip is " << "0.0.0.222" << '\n';
}