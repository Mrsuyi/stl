#pragma once

#include <cstddef>
#include <cstdlib>
#include <memory>

namespace mrsuyi
{
template <class T, class Alloc = std::allocator<T>>
class vector
{
    // typedefs
public:
    using value_type = T;
    using allocator_type = Alloc;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    // iterators
    template <class Type>
    class iter_impl;

    using iterator = iter_impl<value_type>;
    using const_iterator = iter_impl<const value_type>;

    // constructors
public:
    vector();
    vector(size_type n, const_reference val);

    // member functions
    size_type size() const;
    void clear();

    iterator begin();
    iterator end();
    const_iterator end() const;
    const_iterator begin() const;

private:
    pointer ptr_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
};

template <class T, class Alloc>
vector<T, Alloc>::vector()
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const_reference val) : size_(n)
{
    capacity_ = n;
    ptr_ = malloc(capacity_);
    for (size_type i = 0; i < n; ++i)
    {
        new (ptr_ + i) T(val);
    }
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::size() const
{
    return size_;
}

//================================= itertor ==================================//

template <class T, class Alloc>
template <class Type>
class vector<T, Alloc>::iter_impl
{
    using this_t = vector<T, Alloc>::iter_impl<Type>;
    using container = vector<Type, Alloc>;
    using value_type = typename container::value_type;
    using pointer = typename container::pointer;
    using reference = typename container::reference;
    using size_type = typename container::size_type;
    using difference_type = typename container::difference_type;

public:
    iter_impl(pointer, difference_type = 0);

    pointer operator * () const;
    reference operator -> () const;

    this_t& operator ++ ();
    bool operator != (const this_t&);

private:
    pointer ptr_;
    difference_type offset_;
};

template <class T, class Alloc>
template <class Type>
vector<T, Alloc>::iter_impl<Type>::iter_impl(pointer ptr, difference_type offset)
:
    ptr_(ptr),
    offset_(offset)
{}

template <class T, class Alloc>
template <class Type>
vector<T, Alloc>::iter_impl<Type>::iter_impl::pointer
vector<T, Alloc>::iter_impl<Type>::iter_impl::operator * () const
{
    return ptr_ + offset_;
}

template <class T, class Alloc>
template <class Type>
vector<T, Alloc>::iter_impl<Type>::iter_impl::reference
vector<T, Alloc>::iter_impl<Type>::iter_impl::operator -> () const
{
    return *(ptr_ + offset_);
}

template <class T, class Alloc>
template <class Type>
vector<T, Alloc>::iter_impl<Type>::iter_impl::this_t&
vector<T, Alloc>::iter_impl<Type>::iter_impl::operator ++ ()
{
    ++offset_;
}

template <class T, class Alloc>
template <class Type>
bool
vector<T, Alloc>::iter_impl<Type>::iter_impl::operator != (const this_t& other)
{
    return ptr_ != other.ptr_ || offset_ != other.offset_;
}

}
