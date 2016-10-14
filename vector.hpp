#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
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

    // constructor & destructor
public:
    vector();
    vector(size_type n, const_reference val);

    ~vector();

    void push_back(const_reference val);
    void pop_back();

    // member functions
    void reserve(size_type size);
    size_type capacity() const;
    // void resize(size_type size, value_type val = value_type());
    size_type size() const;
    void clear();

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;

    // iterate
    iterator begin();
    iterator end();
    const_iterator end() const;
    const_iterator begin() const;

    // member-variable
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
vector<T, Alloc>::vector(size_type n, const_reference val)
{
    size_t alloc = 1U;
    for (; alloc < n; alloc *= 2);
    reserve(alloc);
    for (size_type i = 0; i < n; ++i)
    {
        new (ptr_ + i) T(val);
    }
    size_ = n;
}

template <class T, class Alloc>
vector<T, Alloc>::~vector()
{
    for (size_type i = 0; i < size_; ++i)
    {
        (ptr_ + i)->~T();
    }
    free(ptr_);
}

template <class T, class Alloc>
void
vector<T, Alloc>::push_back(const_reference val)
{
    if (capacity_ <= size_)
    {
        size_t alloc = 1U;
        for (; alloc <= size_; alloc *= 2);
        reserve(alloc);
    }
    new (ptr_ + size_) T(val);
    ++size_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::size() const
{
    return size_;
}

template <class T, class Alloc>
void
vector<T, Alloc>::clear()
{
    size_ = 0;
}

// template <class T, class Alloc>
// void
// vector<T, Alloc>::resize(size_type size, value_type val)
//{
/*}*/

template <class T, class Alloc>
void
vector<T, Alloc>::reserve(size_type size)
{
    // TODO call std::realloc may cause memory problem
    ptr_ = (pointer)std::realloc(ptr_, size * sizeof(T));
    capacity_ = size;
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::capacity() const
{
    return capacity_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n)
{
    return *(ptr_ + n);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](
    size_type n) const
{
    return *(ptr_ + n);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::begin()
{
    return iterator(ptr_, 0);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::end()
{
    return iterator(ptr_, size_);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::begin() const
{
    return const_iterator(ptr_, 0);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::end() const
{
    return const_iterator(ptr_, size_);
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

    reference operator*() const;
    pointer operator->() const;

    this_t& operator++();
    bool operator!=(const this_t&);

private:
    pointer ptr_;
    difference_type offset_;
};

template <class T, class Alloc>
template <class Type>
vector<T, Alloc>::iter_impl<Type>::iter_impl(pointer ptr,
                                             difference_type offset)
    : ptr_(ptr), offset_(offset)
{
}

template <class T, class Alloc>
template <class Type>
typename vector<T, Alloc>::template iter_impl<Type>::iter_impl::reference
    vector<T, Alloc>::iter_impl<Type>::iter_impl::operator*() const
{
    return *(ptr_ + offset_);
}

template <class T, class Alloc>
template <class Type>
typename vector<T, Alloc>::template iter_impl<Type>::iter_impl::pointer
    vector<T, Alloc>::iter_impl<Type>::iter_impl::operator->() const
{
    return ptr_ + offset_;
}

template <class T, class Alloc>
template <class Type>
typename vector<T, Alloc>::template iter_impl<Type>::iter_impl::this_t&
    vector<T, Alloc>::iter_impl<Type>::iter_impl::operator++()
{
    ++offset_;
}

template <class T, class Alloc>
template <class Type>
bool
vector<T, Alloc>::iter_impl<Type>::iter_impl::operator!=(const this_t& other)
{
    return ptr_ != other.ptr_ || offset_ != other.offset_;
}
}
