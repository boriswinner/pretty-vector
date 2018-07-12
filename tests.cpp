#include "tests.h"
#include "vector.h"

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

