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

class SomeClass
{
public:
    SomeClass(char p) :p_(p) {}
    SomeClass() :p_('k') {}
    char getP() { return p_; }
    friend bool operator==(const SomeClass& lhs, const SomeClass& rhs) { return lhs.p_ == rhs.p_;}
    friend bool operator!=(const SomeClass& lhs, const SomeClass& rhs) { return !(lhs == rhs);}
private:
    char p_;
};

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

class NotIntegralType
{
public:
    NotIntegralType() : large_field_a(new int[100]), large_field_b(new int[100]) {}
    NotIntegralType(NotIntegralType&& a)
    {
        large_field_a = a.large_field_a;
        large_field_b = a.large_field_b;
        a.large_field_a = nullptr;
        a.large_field_b = nullptr;
    }

    NotIntegralType(const NotIntegralType& other) : NotIntegralType()
    {
        std::memcpy(large_field_a, other.large_field_a, 100 * ( sizeof(int)));
        std::memcpy(large_field_b, other.large_field_b, 100 * ( sizeof(int)));
    }

    ~NotIntegralType() {
        delete[] large_field_a;
        delete[] large_field_b;
    }

    int* large_field_a;
    int* large_field_b;
};

class NoCopyConstructor{
public:
    NoCopyConstructor(): a_(1), b_('b'){}
    NoCopyConstructor(const NoCopyConstructor&) = delete;
    NoCopyConstructor(NoCopyConstructor&&) = default;
    int a_;
    char b_;
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

    SECTION("vector filled with smth") {
        pretty_vector::vector<char> test_vector(100,'c');
        REQUIRE(test_vector.capacity() == 100);
        REQUIRE(test_vector.size() == 100);
        for (pretty_vector::vector<char>::const_iterator it = test_vector.begin(); it != test_vector.end(); ++it){
            REQUIRE(*it == 'c');
        }
    }

    SECTION("construct with iterators") {
        std::vector<int> std_vec = {0, 10, 132, 228};
        pretty_vector::vector<int> test_vec(std_vec.begin(), std_vec.end());
        REQUIRE(test_vec.size() == std_vec.size());
        REQUIRE(is_same(test_vec, std_vec));
    }

    SECTION("copy constuctor without allocator")
    {
        pretty_vector::vector<NotIntegralType> test_vector_a(10);
        test_vector_a[4].large_field_a[4] = 127;

        pretty_vector::vector<NotIntegralType> test_vector_b(test_vector_a);

        test_vector_a[4].large_field_a[4] = 128;
        REQUIRE(test_vector_b[4].large_field_a[4] == 127);
    }

    SECTION("brace-enclosed initializer"){
        pretty_vector::vector<int> a({1, 2, 3});
        int i = 1;
        for (pretty_vector::vector<int>::const_iterator it = a.begin(); it != a.end(); ++it){
            REQUIRE(*it == i);
            ++i;
        }
    }

}

TEST_CASE("Inserts and emplaces") {
    SECTION ("Inserts") {
        pretty_vector::vector<char> a(pretty_vector::vector<char>::size_type(1));
        a[0] = 'a';
        a.insert(a.begin(), 'z');
        a.insert(a.begin(), 'y');
        a.insert(a.begin() + 1, 'x');
        pretty_vector::vector<char> other(pretty_vector::vector<char>::size_type(3));
        other[0] = 'x';
        other[1] = 'y';
        other[2] = 'z';
        a.insert(a.begin(), other.begin() + 1, other.begin() + 2);
        a.insert(a.begin(), 3, 'p');
        a.insert(a.begin(), {'p', 'p'});
        std::vector<char> std_a{'p', 'p', 'p', 'p', 'p', 'y', 'y', 'x', 'z', 'a'};
        REQUIRE(is_same(a, std_a));
    }

    SECTION("Emplace") {
        pretty_vector::vector<int> myvector = {10, 20, 30};

        auto it = myvector.emplace(myvector.begin() + 1, 100);
        myvector.emplace(it, 200);
        myvector.emplace(myvector.end(), 300);

        std::vector<int> std_myvector{10, 200, 100, 20, 30, 300};
        REQUIRE(is_same(myvector, std_myvector));
    }

    SECTION("Emplace Back") {
        pretty_vector::vector<int> myvector = {10, 20, 30};
        myvector.emplace_back(100);
        myvector.emplace_back(200);
        std::vector<int> std_myvector{10, 20, 30, 100, 200};
        REQUIRE(is_same(myvector, std_myvector));
    }

    SECTION("Emplace without copy constructor"){
        pretty_vector::vector<NoCopyConstructor> myvector(0);
        myvector.emplace_back(NoCopyConstructor());
        myvector.emplace(myvector.begin(),NoCopyConstructor());
        REQUIRE(myvector.size() == 2);
    }
}

TEST_CASE ("push and pop back"){
    SECTION("push_back")
    {
        pretty_vector::vector<int> test_vector;
        test_vector.push_back(5);
        test_vector.push_back(10);

        REQUIRE(test_vector.size() == 2);
        REQUIRE(test_vector.capacity() > 2);

        REQUIRE(test_vector[0] == 5);
        REQUIRE(test_vector[1] == 10);

        for (int i = 0; i < 10; i++) {
            test_vector.push_back(i);
        }

        REQUIRE(test_vector.size() == 12);
        REQUIRE(test_vector.capacity() == 15);
        for (size_t i = 2; i < test_vector.size(); i++)
        {
            REQUIRE(test_vector[i] == i-2);
        }
    }

    SECTION("pop_back"){
        pretty_vector::vector<int> test_vector;
        test_vector.push_back(5);
        test_vector.push_back(10);
        test_vector.pop_back();
        REQUIRE(test_vector.size() == 1);
        REQUIRE(test_vector[0] == 5);
    }

}

TEST_CASE ("Resize") {
    SECTION("resize"){
        pretty_vector::vector<int> test_vector;
        test_vector.push_back(5);
        test_vector.push_back(10);
        test_vector.resize(5,1488);
        const int cint = 228;
        test_vector.resize(6,cint);
        REQUIRE(test_vector.size() == 6);
        REQUIRE(test_vector[0] == 5);
        REQUIRE(test_vector[1] == 10);
        REQUIRE(test_vector[4] == 1488);
        REQUIRE(test_vector[5] == 228);
    }

    SECTION("swap"){
        pretty_vector::vector<int> test_vector1;
        test_vector1.push_back(5);
        test_vector1.push_back(10);

        pretty_vector::vector<int> test_vector2;
        test_vector2.push_back(45);
        test_vector2.push_back(23);

        test_vector1.swap(test_vector2);
        REQUIRE(test_vector1[0] == 45);
        REQUIRE(test_vector1[1] == 23);
        REQUIRE(test_vector2[0] == 5);
        REQUIRE(test_vector2[1] == 10);

        test_vector1.swap(test_vector2);
        REQUIRE(test_vector1[0] == 5);
        REQUIRE(test_vector1[1] == 10);
    }

    SECTION("swap with copy constructor"){
        pretty_vector::vector<NoCopyConstructor> myvector(0);
        myvector.emplace_back(NoCopyConstructor());
        pretty_vector::vector<NoCopyConstructor> myvector2(0);
        myvector2.emplace_back(NoCopyConstructor());
        myvector.swap(myvector2);
        myvector2.swap(myvector);
    }

    SECTION("front, back"){
        pretty_vector::vector<int> test_vector1;
        test_vector1.push_back(5);
        test_vector1.push_back(10);
        REQUIRE(test_vector1.front() == 5);
        REQUIRE(test_vector1.back() == 10);
    }

    SECTION("shrink_to_fit")
    {
        pretty_vector::vector<int> test_vector({1,2,3,4,5,6,7,8,9,10,11});
        test_vector.reserve(500);
        REQUIRE(test_vector.size() == 11);
        REQUIRE(test_vector.capacity() == 500);
//        test_vector.push_back(89);

        test_vector.shrink_to_fit();
        REQUIRE(test_vector.size() == 11);
        REQUIRE(test_vector.capacity() == 11);
    }

    SECTION("erase(pos)")
    {
        pretty_vector::vector<char > test_vector;
        std::vector<char > std_vector;

        test_vector = {'k', 'l', 'b', 'r', 'm', 'n', 'j', 'v', 'l'};
        std_vector = {'k', 'l', 'b', 'r', 'm', 'n', 'j', 'v', 'l'};

        auto itA = test_vector.begin() + 2;//b
        auto itB = std_vector.begin() + 2;//b

        auto endA = test_vector.erase(itA);

        auto endB = std_vector.erase(itB);

        REQUIRE(is_same(test_vector, std_vector));
    }
}

TEST_CASE("reverse iterator"){
    SECTION("reverse iterator"){
        pretty_vector::vector<int> test_vector1({7,6,5,4,3,2,1});
        int i = 1;
        for (auto it = test_vector1.rbegin(); it != test_vector1.rend(); ++it){
            REQUIRE(*it == i);
            i++;
        }
    }
}

TEST_CASE("assign"){
    SECTION("assign"){
        pretty_vector::vector<int> test_vector1({7,6,5,4,3,2,1});
        test_vector1.assign(pretty_vector::vector<int>::size_type(7),5);
        std::cout << '\n';
    }

    SECTION("assign with iterator"){
        pretty_vector::vector<int> test_vector1({7,6,5,4,3,2,1});
        pretty_vector::vector<int> test_vector2({1,2,3,4,5,6,7});
        test_vector1.assign(test_vector2.begin(),test_vector2.end());
        int i = 7;
        for (auto it = test_vector1.rbegin(); it != test_vector1.rend(); ++it){
            REQUIRE(*it == i);
            i--;
        }
    }

    SECTION("assign with brace-enclosed"){
        pretty_vector::vector<int> test_vector1({7,6,5,4,3,2,1});
        test_vector1.assign({1,2,3,4,5,6,7});
        int i = 7;
        for (auto it = test_vector1.rbegin(); it != test_vector1.rend(); ++it){
            REQUIRE(*it == i);
            i--;
        }
    }
}