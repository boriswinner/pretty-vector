#include <iostream>

namespace pretty_vector {


    template<class T, class Allocator = std::allocator<T>>
    class vector {
    public:
        using data_type = T;
        using allocator_type = Allocator;
        using size_type = unsigned int;
        using difference_type = unsigned int;
        using reference = T &;
        using const_reference = const T &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        float resize_coefficient = 1.5;

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

            size_type current_index(){
                return i;
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
        explicit vector(const Allocator &alloc = Allocator()) {

        };

        explicit vector(size_type size, const Allocator &alloc = Allocator()) :
                allocator_(alloc), capacity_(size), size_(size), data_(allocator_.allocate(capacity_)) {
            for (size_type i = 0; i < size; ++i){
                data_[i] = T();
            }
        };

        explicit vector(size_type size, const T &value, const Allocator &alloc = Allocator()) :
                allocator_(alloc), capacity_(size), size_(0), data_(allocator_.allocate(capacity_)) {
            fill_with_value(value);
        }

        template< class InputIt >
        vector( InputIt first, InputIt last,
                const Allocator& alloc = Allocator() ){
            reserve(last - first);
            iterator it = begin();
            insert(it, first, last);
        }

        reference at(size_type pos) {
            if (pos >= 0 && pos < size_) {
                return data_[pos];
            } else {
                throw std::out_of_range("Index out of vector range");
            }
        }

        const_reference at(size_type pos) const {
            if (pos >= 0 && pos < size_) {
                return data_[pos];
            } else {
                throw std::out_of_range("Index out of vector range");
            }
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

        template< class... Args >
        iterator emplace( iterator pos, Args&&... args ){
            //this may be not working
            insert(pos,T(&args...));
        }

        iterator insert( iterator pos, const T& value ){
            if (capacity_ - size_ < 1){
                reserve(capacity_+1);
            }
            size_type start_position = static_cast<size_type>(pos.current_index());
            shift_right_from_position(start_position);
            data_[start_position] = value;
            size_++;
        }

        void insert( iterator pos, size_type count, const T& value ){
            iterator titer = pos;
            for (size_type i = 0; i < count; ++i){
                insert(titer,value);
                titer++;
            }
        }

        template< class InputIt >
        void insert( iterator pos, InputIt first, InputIt last){
            iterator titer = pos;
            for (InputIt t = first; t != last; ++t){
                insert(titer, *t);
                titer++;
            }
        }

        iterator erase( iterator pos ){
            //this may be not working
            allocator_.destroy(data_+pos.current_index());
            return ++pos;
        }

        iterator erase( iterator first, iterator last ){
            for (iterator t = first; t != last; ++t){
                erase(t);
            }
        }

        void push_back( const T& value ){
            if (capacity_ < size_ * resize_coefficient){
                reserve (static_cast<size_type>(size_ * resize_coefficient));
            }
            iterator it = end();
            insert(++it, value);
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

        void shift_right_from_position(size_type pos){
            for (size_type i = size_; i > pos ; --i){
                data_[i] = data_[i-1];
            }
        }

        void move_data_to_pointer(pointer data) {
            for (size_type i = 0; i < size_; i++) {
                allocator_.construct(data + i, std::move(data_[i]));
                allocator_.destroy(data_ + i);
            }
        }
    };
}