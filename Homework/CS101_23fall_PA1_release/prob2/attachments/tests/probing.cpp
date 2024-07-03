#include "../hash_table.hpp"

#include <iostream>

const std::size_t Mod = 1000037;
std::uint32_t probe_position(const Website &W)
{
    return std::hash<Website>{}(W);
}

int main()
{
    HashTable<Website,IPv4> table;
    Website A(std::string("eFsamu")),B(std::string("AR`v\\y")),C(std::string("_dOdpm")),D(std::string("hZZllr"));
    IPv4  ip1(1), ip2(2), ip3(3), ip4(4);

    std::cout << A << " starts probing at position " << probe_position(A) % Mod  << '\n';
    std::cout << B << " starts probing at position " << probe_position(B) % Mod  << '\n';
    std::cout << C << " starts probing at position " << probe_position(C) % Mod  << '\n';
    std::cout << D << " starts probing at position " << probe_position(D) % Mod  << '\n';

    table.insert(A,ip1);
    table.insert(B,ip2);
    table.insert(C,ip3);
    table.insert(D,ip4);

    std::cout << "from hash table find ip of " << C << " is " << table[C] << '\n';
    std::cout << "expected ip is " << ip3 << '\n';

    table.erase(B);

    std::cout << "from hash table find ip of " << C << " is " << table[C] << '\n';
    std::cout << "expected ip is " << ip3 << '\n';

    table.insert(D, ip2);

    std::cout << "from hash table find ip of " << D << " is " << table[D] << '\n';
    std::cout << "expected ip is " << ip2 << '\n';

    table.erase(D);

    std::cout << "from hash table find position of " << D <<  " is " << table.find(D) << '\n';
    std::cout << "expected position is (unsigned)-1 : " << (size_t)(-1) << '\n';

}