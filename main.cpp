#include <iostream>
#include <limits>
#include <cmath>
#include <cstring>
#include "vector.h"




/*int main() {
    pretty_vector::Vector<int> a(10, 2);
    a.debug_cout_data();
    std::cout << '\n';
    std::cout << a.front();
    std::cout << a.back();
    std::cout << '\n';
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    a[3] = 3;
    a[4] = 4;
    a[5] = 5;
    a[6] = 6;
    a[7] = 7;
    a[8] = 8;
    a[9] = 9;
    a.reserve(11);
    //a[9] = 3;
    //a.clear();
    pretty_vector::Vector<int>::iterator it(a.begin());
    a.insert(++it, pretty_vector::Vector<int>::size_type(3), 15);
    a.insert(++it, -5, 0);
    //a.emplace(it,100);
    //a.erase(it);
    a.push_back(100);

    for (it = a.begin(); it <= a.end(); ++it) {
        std::cout << *it << ' ';
    }

    return 0;
}*/