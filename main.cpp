#include <iostream>


namespace pretty_vector {


    template<class T, class Allocator = std::allocator<T>>
    class Vector {
        using data_type = T;
        using allocator_type = Allocator;
        using size_type = int;
        using difference_type = unsigned int;
        using reference = T &;
        using const_reference = const T &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

        template<typename V>
        class MyVectorIterator : public std::iterator<std::random_access_iterator_tag, V, int> {

        public:
            Vector *c;
            unsigned int i;

            MyVectorIterator(Vector *c) : c(c), i(0) {}
            MyVectorIterator(Vector *c, unsigned int i) : c(c), i(i) {}

        public:
            typedef typename std::iterator<std::random_access_iterator_tag, V, int>::pointer pointer;
            typedef typename std::iterator<std::random_access_iterator_tag, V, int>::reference reference;
            typedef typename std::iterator<std::random_access_iterator_tag, V, int>::difference_type difference_type;

            template<typename V2>
            MyVectorIterator(const MyVectorIterator<V2> &other) : c(other.c), i(other.i) {}

            template<typename V2>
            MyVectorIterator &operator=(const MyVectorIterator<V2> &other) {
                c = other.c;
                i = other.i;
                return *this;
            }

            reference operator*() const {
                return (*c)[i];
            }

            pointer operator->() const {
                return &(*c)[i];
            }

            MyVectorIterator &operator++() {
                ++i;
                return *this;
            }

            MyVectorIterator &operator--() {
                --i;
                return *this;
            }

            MyVectorIterator operator++(int) {
                return MyVectorIterator(c, i++);
            }

            MyVectorIterator operator--(int) {
                return MyVectorIterator(c, i--);
            }

            MyVectorIterator operator+(const difference_type &n) const {
                return MyVectorIterator(c, (i + n));
            }

            MyVectorIterator &operator+=(const difference_type &n) {
                i += n;
                return *this;
            }

            MyVectorIterator operator-(const difference_type &n) const {
                return MyVectorIterator(c, (i - n));
            }

            MyVectorIterator &operator-=(const difference_type &n) {
                i -= n;
                return *this;
            }

            reference operator[](const difference_type &n) const {
                return (*c)[i + n];
            }

            bool operator==(const MyVectorIterator &other) const {
                return i == other.i;
            }

            bool operator!=(const MyVectorIterator &other) const {
                return i != other.i;
            }

            bool operator<(const MyVectorIterator &other) const {
                return i < other.i;
            }

            bool operator>(const MyVectorIterator &other) const {
                return i > other.i;
            }

            bool operator<=(const MyVectorIterator &other) const {
                return i <= other.i;
            }

            bool operator>=(const MyVectorIterator &other) const {
                return i >= other.i;
            }

            difference_type operator+(const MyVectorIterator &other) const {
                return i + other.i;
            }

            difference_type operator-(const MyVectorIterator &other) const {
                return i - other.i;
            }
        };

        typedef MyVectorIterator<data_type > iterator;
        typedef MyVectorIterator<const data_type> const_iterator;

    public:
        explicit Vector(const Allocator &alloc = Allocator()) {

        };

        explicit Vector(size_type size, const Allocator &alloc = Allocator()) :
                allocator_(alloc), capacity_(size), size_(0), data_(allocator_.allocate(capacity_)) {
        };

        explicit Vector(size_type size, const T &value, const Allocator &alloc = Allocator()) :
                allocator_(alloc), capacity_(size), size_(0), data_(allocator_.allocate(capacity_)) {
            fill_with_value(value);
        }

        reference at(size_type pos) {
            if (pos >= 0 && pos <= size_) {
                return data_[pos];
            } else {
                throw std::out_of_range("Index out of vector range");
            }
        }

        const_reference at(size_type pos) const {
            return data_[pos];
        }

        reference operator[](size_type pos) {
            return at(pos);
        }

        const_reference operator[](size_type pos) const {
            return at(pos);
        }

        reference front() {
            return at(0);
        }

        const_reference front() const {
            return at(0);
        }

        reference back() {
            return at(size_ - 1);
        }

        const_reference back() const {
            return at(size_ - 1);
        }

        T *data() {
            return data_;
        }

        const T *data() const {
            return data_;
        }

        iterator begin() {
            return iterator(data_, size_);
        }

        const_iterator begin() const {
            return const_iterator(data_, size_);
        }

        const_iterator cbegin() const {
            return const_iterator(data_, size_);
        }

        void debug_cout_data() {
            for (int i = 0; i < size_; ++i) {
                std::cout << data_[i];
            }
        }

    private:
        size_type capacity_, size_;
        Allocator allocator_;
        pointer data_;

        void fill_with_value(const T &value) {
            for (size_type i = 0; i < capacity_; ++i) {
                data_[i] = value;
            }
            size_ = capacity_;
        };
    };
}

int main() {
    pretty_vector::Vector<int> a(10, 2);
    a.debug_cout_data();
    std::cout << '\n';
    std::cout << a.front();
    std::cout << a.back();
    return 0;
}