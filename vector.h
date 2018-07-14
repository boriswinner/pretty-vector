#include <iostream>
#include "allocator.h"
#include <cmath>

namespace pretty_vector {


    template<class T, class Allocator = std::allocator<T>>
    class vector {
    public:
        using data_type = T;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using size_type = unsigned int;
        using reference = T &;
        using const_reference = const T &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
        float resize_coefficient = 1.5;

        template<typename TypeT>
        class MyIterator :
                public std::iterator<std::random_access_iterator_tag, TypeT> {
        public:

            template<typename T2>
            MyIterator(const MyIterator<T2> &other) : data_(other.data_), index_(other.index_) {}

            MyIterator(pointer data) : data_(data), index_(0) {}

            MyIterator(pointer data, size_type index) : data_(data), index_(index) {}

            MyIterator(){};

            template<typename T2>
            MyIterator &operator=(const MyIterator<T2> &other) {
                data_ = other.data_;
                index_ = other.index_;
                return *this;
            }

            size_type current_index() {
                return index_;
            }

            pointer iterator_data() {
                return data_;
            }

            MyIterator operator++(int) {
                return MyIterator(data_, index_++);
            }

            MyIterator operator--(int) {
                return MyIterator(data_, index_--);
            }

            MyIterator &operator++() {
                ++index_;
                return *this;
            }

            MyIterator &operator--() {
                --index_;
                return *this;
            }

            MyIterator operator+(const difference_type &n) const {
                return MyIterator(data_, (index_ + n));
            }

            MyIterator &operator+=(const difference_type &n) {
                index_ += n;
                return *this;
            }

            MyIterator operator-(const difference_type &n) const {
                return MyIterator(data_, (index_ - n));
            }

            MyIterator &operator-=(const difference_type &n) {
                index_ -= n;
                return *this;
            }

            reference operator[](const difference_type &n) const {
                return (data_)[index_ + n];
            }

            reference operator*() const {
                return data_[index_];
            }

            pointer operator->() const {
                return &data_[index_];
            }

            pointer operator&() const {
                return &data_[index_];
            }

            bool operator==(const MyIterator &other) const {
                return index_ == other.index_;
            }

            bool operator!=(const MyIterator &other) const {
                return index_ != other.index_;
            }

            bool operator<(const MyIterator &other) const {
                return index_ < other.index_;
            }

            bool operator>(const MyIterator &other) const {
                return index_ > other.index_;
            }

            bool operator>=(const MyIterator &other) const {
                return index_ >= other.index_;
            }

            bool operator<=(const MyIterator &other) const {
                return index_ <= other.index_;
            }

            difference_type operator+(const MyIterator &other) const {
                return index_ + other.index_;
            }

            difference_type operator-(const MyIterator &other) const {
                return index_ - other.index_;
            }

        public:
            pointer data_;
            size_type index_;
            typedef typename std::iterator<std::random_access_iterator_tag, TypeT, int>::difference_type difference_type;
        };

        typedef MyIterator<data_type> iterator;
        typedef MyIterator<const data_type> const_iterator;

        template<typename TypeV>
        class ReverseIterator :
                public std::iterator<std::random_access_iterator_tag, TypeV> {
        public:
            template<typename T2>
            ReverseIterator(const MyIterator<T2> &other) :  current(other){}

            size_type current_index() {
                return current.index_;
            }

            pointer iterator_data() {
                return current.data_;
            }

            ReverseIterator operator++(int) {
                return MyIterator<data_type >(current.data_, current.index_--);
            }

            ReverseIterator operator--(int) {
                return MyIterator<data_type>(current.data_, current.index_++);
            }

            ReverseIterator &operator++() {
                --current.index_;
                return *this;
            }

            ReverseIterator &operator--() {
                ++current.index_;
                return *this;
            }

            ReverseIterator operator+(const difference_type &n) const {
                return MyIterator<data_type>(current.data_, (current.index_ + n));
            }

            ReverseIterator &operator+=(const difference_type &n) {
                current.index_ += n;
                return *this;
            }

            ReverseIterator operator-(const difference_type &n) const {
                return MyIterator<data_type>(current.data_, (current.index_ - n));
            }

            ReverseIterator &operator-=(const difference_type &n) {
                current.index_ -= n;
                return *this;
            }

            reference operator[](const difference_type &n) const {
                return (current.data_)[current.index_ + n];
            }

            reference operator*() const {
                return current.data_[current.index_];
            }

            pointer operator->() const {
                return &current.data_[current.index_];
            }

            pointer operator&() const {
                return &current.data_[current.index_];
            }

            bool operator==(const ReverseIterator &other) const {
                return current.index_ == other.current.index_;
            }

            bool operator!=(const ReverseIterator &other) const {
                return current.index_ != other.current.index_;
            }

            bool operator<(const ReverseIterator &other) const {
                return current.index_ < other.current.index_;
            }

            bool operator>(const ReverseIterator &other) const {
                return current.index_ > other.current.index_;
            }

            bool operator>=(const ReverseIterator &other) const {
                return current.index_ >= other.current.index_;
            }

            bool operator<=(const ReverseIterator &other) const {
                return current.index_ <= other.current.index_;
            }

            difference_type operator+(const ReverseIterator &other) const {
                return current.index_ + other.current.index_;
            }

            difference_type operator-(const ReverseIterator &other) const {
                return current.index_ - other.current.index_;
            }

        public:
            MyIterator<data_type > current;
        };

        typedef ReverseIterator<data_type> reverse_iterator;
        typedef ReverseIterator<const data_type> const_reverse_iterator;

    public:
        explicit vector(const Allocator &alloc = Allocator()) : allocator_(alloc), capacity_(0), size_(0) {};

        explicit vector(size_type count, const T &value, const Allocator &alloc = Allocator()) :
                allocator_(alloc), capacity_(count), size_(count), data_(allocator_.allocate(capacity_)) {
            fill_with_value(value);
        }

        explicit vector(size_type count) :
                allocator_(Allocator()), capacity_(count), size_(count),
                data_(std::allocator_traits<Allocator>::allocate(allocator_, count)) {
            for (size_type i = 0; i < count; ++i) {
                allocator_.construct(data_ + i);
            }
        };

        template<class InputIt>
        vector(InputIt first, InputIt last,
               const Allocator &alloc = Allocator()) {
            reserve(last - first);
            iterator it = begin();
            insert(it, first, last);
            size_ = last - first;
        }

        vector(const vector &other) : allocator_(other.allocator_), capacity_(other.capacity_), size_(other.size_),
                                      data_(allocator_.allocate(capacity_)) {
            int i = 0;
            for (iterator it = other.begin(); it != other.end(); ++it) {
                std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
                ++i;
            }
        }

        vector(vector &&other, const Allocator &alloc) : allocator_(other.allocator_), capacity_(other.capacity_),
                                                         size_(other.size_),
                                                         data_(allocator_.allocate(capacity_)) {
            int i = 0;
            for (iterator it = other.begin(); it != other.end(); ++it) {
                std::allocator_traits<Allocator>::construct(allocator_, data_ + i, std::move(*it));//copy
                ++i;
            }
        }

        vector(std::initializer_list<T> init, const Allocator &alloc = Allocator()) : allocator_(alloc),
                                                                                      capacity_(init.size()),
                                                                                      size_(init.size()),
                                                                                      data_(allocator_.allocate(
                                                                                              capacity_)) {
            size_type i = 0;
            for (auto it = init.begin(); it != init.end(); it++) {
                allocator_.construct(data_ + i, *it);
                i++;
            }
        }

        ~vector(){
            for (size_type i = 0; i < size_; i++) {
                std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
            }
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

        reverse_iterator rbegin(){
            return reverse_iterator(end()-1);
        }

        const_reverse_iterator rbegin() const{
            return const_reverse_iterator(end()-1);

        }

        const_reverse_iterator crbegin() const{
            return const_reverse_iterator(end()-1);
        }

        reverse_iterator rend(){
            return reverse_iterator(begin()-1);
        }

        const_reverse_iterator rend() const{
            return const_reverse_iterator(begin()-1);
        }

        const_reverse_iterator crend() const{
            return const_reverse_iterator(begin()-1);
        }

        iterator end() {
            return iterator(data_, size_);
        }

        const_iterator end() const {
            return const_iterator(data_, size_);
        }

        const_iterator cend() const {
            return const_iterator(data_, size_);
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
                if (capacity() > 0){
                    allocator_.deallocate(data_, capacity_);
                }
                capacity_ = size;
                data_ = new_data;
                allocator_ = tallocator;
            }
        }

        size_type capacity() const {
            return capacity_;
        }

        void shrink_to_fit() {
            allocator_.deallocate(data_ + size_, capacity_);
            capacity_ = size_;
        }

        void clear() {
            for (size_type i = 0; i < size_; ++i) {
                allocator_.destroy(data_ + i);
            }
            size_ = 0;
        }

        bool requires_reallocation(size_type new_size) { return new_size >= capacity_; }

        void reallocation(size_type new_size) {
            if (this->requires_reallocation(new_size)) {
                size_type needed_capacity = capacity_ + new_size - size_;
                size_type new_capacity = static_cast<size_type>(ceil(needed_capacity * resize_coefficient));
                this->reserve(new_capacity);
            }
        }

        template<typename ...Args>
        void emplace_back(Args &&... args) {
            this->reallocation(size_ + 1);
            std::allocator_traits<Allocator>::construct(allocator_, data_ + size_, std::forward<Args>(args)...);
            ++size_;
        }

        template<typename ...Args>
        iterator emplace(const_iterator position, Args &&... args) {
            difference_type index_position = std::distance(this->cbegin(), position);
            reallocation(size_ + 1);

            if (index_position <= size_ - 1) {
                shift_right(1, iterator(data_ + index_position), end());
            }

            std::allocator_traits<Allocator>::construct(allocator_, data_ + index_position,
                                                        std::forward<Args>(args)...);
            ++size_;

            return iterator(data_, index_position);
        }

        iterator insert(const_iterator pos, const T &value) {
            return this->emplace(pos, std::forward<const T &>(value));
        }

        iterator insert(const_iterator pos, T &&value) {
            return this->emplace(pos, std::forward<const T &>(value));
        }

        iterator insert(const_iterator pos, size_type count, const T &value) {
            shift_right(count, pos, end());
            for (auto i = 0; i < count; ++i) {
                std::allocator_traits<Allocator>::construct(allocator_, data_ + i, value);
            }
            size_ += count;
        }

        template<class InputIt>
        iterator insert(iterator pos, InputIt first, InputIt last) {
            size_type size = last - first;
            shift_right(size, pos, end());
            int i = pos.current_index();
            for (auto it = first; it != last; it++) {
                std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
                ++i;
            }
            size_ += size;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
            size_type size = ilist.end() - ilist.begin();
            shift_right(size, pos, end());
            int i = pos.current_index();
            for (auto it = ilist.begin(); it != ilist.end(); it++) {
                std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
                ++i;
            }
            size_ += size;
        }

        iterator erase(iterator pos) {
            allocator_.destroy(data_ + pos.current_index());
            shift_left(1,pos+1,end());
            size_--;
            return ++pos;
        }

        iterator erase(iterator first, iterator last) {
            for (iterator t = first; t != last; ++t) {
                allocator_.destroy(data_ + t.current_index());
            }
            shift_left(static_cast<size_type>(last - first), last + 1, end());
            size_ -= static_cast<size_type>(last - first);
            return ++last;
        }

        void push_back(const T &value) {
            reallocation(size_ + 1);
            iterator it = end();
            insert(it, value);
        }

        void pop_back() {
            if (size_ > 0) {
                size_--;
                std::allocator_traits<Allocator>::destroy(allocator_, data_ + size_);
            }
        }

        void assign( size_type count, const T& value ){
            for (size_type i = 0; i < size_; i++) {
                allocator_.destroy(data_ + i);
            }
            reallocation(count);
            fill_with_value(value);
            size_ = count;
        }

        template< class InputIt >
        void assign( InputIt first, InputIt last ){
            for (size_type i = 0; i < size_; i++) {
                allocator_.destroy(data_ + i);
            }
            auto count = last - first;
            reallocation(count);
            size_ = count;
            size_type i = 0;
            for (auto it = first; it != last; it++) {
                allocator_.construct(data_ + i, *it);
                i++;
            }
        }

        void assign( std::initializer_list<T> ilist ){
            assign(ilist.begin(), ilist.end());
        }

        void resize(size_type count) {
            if (count <= size_) {
                for (int i = 0; i < abs(count - size_); ++i) {
                    pop_back();
                }
            } else {
                for (auto i = 0; i < abs(count - size_); ++i) {
                    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_ + i, T());
                }
                size_ += abs(count - size_);
            }
        }

        void resize(size_type count, const value_type &value) {
            if (count <= size_) {
                for (int i = 0; i < abs(count - size_); ++i) {
                    pop_back();
                }
            } else {
                for (auto i = 0; i < abs(count - size_); ++i) {
                    std::allocator_traits<Allocator>::construct(allocator_, data_ + size_ + i, value);
                }
                size_ += abs(count - size_);
            }
        }

        void swap(vector &other) {
            std::swap(allocator_, other.allocator_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            std::swap(data_, other.data_);
        }

        friend bool operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
        {
            if (lhs.size_ != rhs.size_) {
                return false;
            }

            auto itL = lhs.begin();
            auto itR = rhs.begin();

            for (; itL != lhs.end(); itL++, itR++) {
                if (*itL != *itR) {
                    return false;
                }
            }

            return true;
        }

        friend bool operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
        {
            auto size = std::min(lhs.size(), rhs.size());

            for (typename vector<T, Allocator>::size_type i = 0; i < size; i++) {
                if (lhs[i] < rhs[i]) {
                    return true;
                }
            }
            return lhs.size() < rhs.size();
        }

        friend bool operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator> (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
        {
//            auto size = std::min(lhs.size(), rhs.size());
//
//            bool flag = true;
//            for (typename vector<T, Allocator>::size_type i = 0; i < size; i++) {
//                if (lhs[i] < rhs[i]) {
//                    return true;
//                }
//            }
//            return lhs.size() == rhs.size();
        }

        friend bool operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs)
        {
            return rhs <= lhs;
        }


    private:
        size_type capacity_, size_;
        Allocator allocator_;
        pointer data_;

        void fill_with_value(const T &value) {
            for (iterator it = begin(); it != end(); ++it) {
                *it = value;
            }
            size_ = capacity_;
        };

        void move_data_to_pointer(pointer data) {
            for (size_type i = 0; i < size_; i++) {
                allocator_.construct(data + i, std::move(data_[i]));
                allocator_.destroy(data_ + i);
            }
        }

        void shift_right(size_type n, iterator from, iterator after_last) { // [begin, last)
            if (from >= after_last || n == 0) {
                return;
            }

            for (iterator it = after_last - 1; true; --it) {
                allocator_.construct(&*(it + n), std::move(*it));
                if (it == from) { break; }
            }
        }

        void shift_left(size_type n, iterator from, iterator after_last) { // [begin, last)
            if (from >= after_last || n == 0) {
                return;
            }

            for (iterator it = from; it < after_last ; ++it) {
                allocator_.construct(&*(it - n), std::move(*it));
            }
        }

    };
}