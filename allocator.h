#include <limits>
#include <iostream>
#include <vector>

namespace pretty_allocator {
    template<class T>
    class allocator {
    public:
        typedef T value_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template<class U>
        struct rebind {
            typedef allocator<U> other;
        };

        pointer address(reference value) const {
            return &value;
        }

        const_pointer address(const_reference value) const {
            return &value;
        }

        allocator() noexcept {
        }

        allocator(const allocator &) noexcept {
        }

        template<class U>
        allocator(const allocator<U> &) noexcept {
        }

        ~allocator() noexcept {
        }

        size_type max_size() const noexcept {
            return std::numeric_limits<std::size_t>::max() / sizeof(T);
        }

        pointer allocate(size_type num, const void * = 0) {
            pointer p = (pointer) (::operator new(num * sizeof(T)));
            return p;
        }

        void construct(pointer p, const T &value = T()) {
            new((void *) p)T(value);
        }

        void destroy(pointer p) {
            p->~T();
        }

        void deallocate(pointer p, size_type num) {
            ::operator delete((void *) p);
        }
    };

    template<class T1, class T2>
    bool operator==(const allocator<T1> &,
                    const allocator<T2> &) throw() {
        return true;
    }

    template<class T1, class T2>
    bool operator!=(const allocator<T1> &,
                    const allocator<T2> &) throw() {
        return false;
    }
}


/*int main()
{
    std::vector<int,pretty_allocator::allocator<int> > v;

    v.push_back(42);
    v.push_back(56);
    v.push_back(11);
    v.push_back(22);
    v.push_back(33);
    v.push_back(44);
    std::cout << v[2];
}*/