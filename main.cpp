#include <iostream>

class pointer;

class pointer;
namespace pretty_vector {

    enum {constant = true, nonconstant = false};

    template <typename T, bool is_const_iterator, class Allocator = std::allocator<T>>
    class VectorIterator
    {
        using reference = T &;
        using const_reference = const T &;
        using pointer = typename std::allocator_traits<Allocator>::pointer;

    public:
        using size_type = int;

        VectorIterator() = delete;

        VectorIterator(const VectorIterator& other) : ptr_data_(other.ptr_data_) {}
        VectorIterator(VectorIterator&& other) noexcept : ptr_data_(std::move(other.ptr_data_)) {}

        VectorIterator(const typename
                       std::iterator_traits<VectorIterator<T, is_const_iterator>>:: pointer& ptr_data)
                : ptr_data_(ptr_data) {}

        ~VectorIterator() = default;


        VectorIterator& operator=(const VectorIterator& other) {
            ptr_data_ = other.ptr_data_;
            return *this;
        }

        VectorIterator& operator=(VectorIterator&& other) {
            std::swap(ptr_data_, other.ptr_data_);
            return *this;
        }

        typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::reference operator*() const {
            return *ptr_data_;
        }

        typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer operator->() const {
            return ptr_data_;
        }

        typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::reference operator[](size_type n) const {
            return *(ptr_data_ + n);
        }

        typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer data() const {
            return ptr_data_;
        }

        VectorIterator<T, is_const_iterator>& operator++() {
            ptr_data_++;
            return *this;
        }

        VectorIterator& operator--() {
            ptr_data_--;
            return *this;
        }

        VectorIterator operator++(int) {
            VectorIterator tmp(ptr_data_);
            ptr_data_++;
            return tmp;
        }

        VectorIterator operator--(int) {
            VectorIterator tmp(ptr_data_);
            ptr_data_--;
            return tmp;
        }

        VectorIterator& operator+=(size_type n) {
            ptr_data_ += n;
            return *this;
        }

        VectorIterator& operator-=(size_type n)  {
            ptr_data_ -= n;
            return *this;
        }

        void swap(VectorIterator& b) {
            std::swap(this->ptr_data_, b.ptr_data_);
        }

        typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::difference_type
        friend operator-(const VectorIterator& lv, const VectorIterator& rv)
        {
            return lv.ptr_data_ - rv.ptr_data_;
        }


        friend VectorIterator<T, is_const_iterator> operator+(const VectorIterator<T, is_const_iterator>& iter, int n)
        {
            return n + iter;
        }

        friend VectorIterator<T, is_const_iterator> operator+(int n, const VectorIterator<T, is_const_iterator>& iter)
        {
            return VectorIterator<T, is_const_iterator>(iter.ptr_data_ + n);
        }

        friend VectorIterator<T, is_const_iterator> operator-(const VectorIterator<T, is_const_iterator> & iter, int n)
        {
            return n - iter;
        }

        friend VectorIterator<T, is_const_iterator> operator-(int n, const VectorIterator<T, is_const_iterator>& iter)
        {
            return VectorIterator<T, is_const_iterator>(iter.ptr_data_ - n);
        }


        // const_iterator cast
        operator VectorIterator<T, true>() const {
            return VectorIterator<T, true>(this->ptr_data_);
        };

    private:
        typename std::iterator_traits<VectorIterator<T, is_const_iterator>>::pointer ptr_data_;

    };

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
        using iterator = VectorIterator<T, nonconstant>;
        using const_iterator = VectorIterator<T, constant>;

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

        reference front(){
            return at(0);
        }

        const_reference front() const{
            return at(0);
        }

        reference back(){
            return at(size_ - 1);
        }

        const_reference back() const{
            return at(size_ - 1);
        }

        T* data(){
            return data_;
        }

        const T* data() const{
            return data_;
        }

        iterator begin(){
            return iterator(data_, size_);
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