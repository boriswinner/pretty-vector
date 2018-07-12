#define CATCH_CONFIG_MAIN
#include <iostream>
#include <limits>
#include <cmath>
#include <cstring>
#include "vector.h"
#include "catch.hpp"

template <class T, class U, class = typename T::iterator, class = typename U::iterator>
bool is_same(const T& a, const U& s)
{
    if (s.size() != a.size()) {
        return false;
    }

    for (typename pretty_vector::vector<T>::size_type i = 0; i < a.size(); i++) {
        if (a[i] != s[i]) {
            return false;
        }
    }
    return true;
}

class ClassForTesting
{
public:
    ClassForTesting(int p, char c) :p_(p), c_(c) {}
    int get_int() { return p_; }
    char get_char() { return c_;}
    friend bool operator==(const ClassForTesting& lhs, const ClassForTesting& rhs) { return ((lhs.p_ == rhs.p_) && (lhs.c_ == rhs.c_));}
    friend bool operator!=(const ClassForTesting& lhs, const ClassForTesting& rhs) { return !(lhs == rhs);}
private:
    int p_;
    char c_;
};

TEST_CASE("Constructors") {
    SECTION("vector(size_type n)") {
        pretty_vector::vector<double> test_vector(500);
        REQUIRE(test_vector.capacity() == 500);
        REQUIRE(test_vector.size() == 500);
        REQUIRE_FALSE(test_vector.empty());
    }

    SECTION("vector(size_type 0)") {
        pretty_vector::vector<bool> test_vector(0);
        REQUIRE(test_vector.capacity() == 0);
        REQUIRE(test_vector.size() == 0);
        REQUIRE(test_vector.empty());
    }

    SECTION("construct with iterators") {
        std::vector<int> std_vec = {0, 10, 132, 228};
        pretty_vector::vector<int> test_vec(std_vec.begin(), std_vec.end());
        REQUIRE(test_vec.capacity() == std_vec.size());

        bool sjfnjak = is_same(test_vec, std_vec);
    }
}




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