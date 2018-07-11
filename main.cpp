#include <iostream>
#include <limits>


namespace pretty_vector {


    template<class T, class Allocator = std::allocator<T>>
    class Vector {
    public:
        using data_type = T;
        using allocator_type = Allocator;
        using size_type = unsigned int;
        using difference_type = unsigned int;
        using reference = T &;
        using const_reference = const T &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

        template<typename V>
        class MyVectorIterator : public std::iterator<std::random_access_iterator_tag, V, int> {

        protected:
            pointer c;
            size_type i;
            typedef typename std::iterator<std::random_access_iterator_tag, V, int>::pointer pointer;
            typedef typename std::iterator<std::random_access_iterator_tag, V, int>::reference reference;
            typedef typename std::iterator<std::random_access_iterator_tag, V, int>::difference_type difference_type;

        public:
            MyVectorIterator(pointer c) : c(c), i(0) {}

            MyVectorIterator(pointer c, size_type i) : c(c), i(i) {}

            template<typename V2>
            MyVectorIterator(const MyVectorIterator<V2> &other) : c(other.c), i(other.i) {}

            template<typename V2>
            MyVectorIterator &operator=(const MyVectorIterator<V2> &other) {
                c = other.c;
                i = other.i;
                return *this;
            }

            reference operator*() const {
                return (c)[i];
            }

            pointer operator->() const {
                return &(c)[i];
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
                return (c)[i + n];
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

        typedef MyVectorIterator<data_type> iterator;
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
            if (pos >= 0 && pos < size_) {
                return data_[pos];
            } else {
                throw std::out_of_range("Index out of vector range");
            }
        }

        const_reference at(size_type pos) const {
            return at(pos);
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
            return iterator(data_);
        }

        const_iterator begin() const {
            return const_iterator(data_);
        }

        const_iterator cbegin() const {
            return const_iterator(data_);
        }

        iterator end() {
            return iterator(data_, size_ - 1);
        }

        const_iterator end() const {
            return const_iterator(data_, size_ - 1);
        }

        const_iterator cend() const {
            return const_iterator(data_, size_ - 1);
        }

        bool empty() const {
            return (size_ == 0);
        }

        size_type size() const {
            return size_;
        }

        size_type max_size() const {
            return std::numeric_limits<size_type>::max();
        }

        void reserve(size_type size) {
            //actually, size means capacity here, but the standart header is like that
            if (size <= capacity_) {
                return;
            } else if (size > max_size()) {
                throw std::length_error("Out of memory!");
            } else {
                Allocator tallocator;
                auto new_data = tallocator.allocate(size);
                move_data_to_pointer(new_data);
                allocator_.deallocate(data_, capacity_); //maybe I should do here like Setyozha - std::alllocator_traits
                capacity_ = size;
                data_ = new_data;
                allocator_ = tallocator;
            }
        }

        size_type capacity() const{
            return capacity_;
        }

        void shrink_to_fit(){
            allocator_.deallocate(data_+size_,capacity_);
            capacity_ = size_;
        }

        void clear(){
            for (size_type i = 0; i < size_; ++i){
                allocator_.destroy(data_+i);
            }
            size_ = 0;
        }

        iterator insert( iterator pos, const T& value ){
            size_type start_position = std::distance(this->begin(), pos);
            move_data_from_position_to_pointer(start_position, data_+start_position+1);
            data_[start_position] = value;
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

        void move_data_from_position_to_pointer(size_type pos, pointer data){
            for (size_type i = pos; i < size_; i++) {
                allocator_.construct(data + i, std::move(data_[i]));
                allocator_.destroy(data_ + i);
            }
        }

        void move_data_to_pointer(pointer data) {
            move_data_from_position_to_pointer(0, data);
        }
    };
}

int main() {
    pretty_vector::Vector<int> a(10, 2);
    a.debug_cout_data();
    std::cout << '\n';
    std::cout << a.front();
    std::cout << a.back();
    std::cout << '\n';
    a[3] = 1;
    a.reserve(11);
    a[9] = 3;
    //a.clear();
    pretty_vector::Vector<int>::iterator it(a.begin());
    for (it = a.begin(); it <= a.end(); ++it) {
        std::cout << *it << ' ';
    }
    return 0;
}