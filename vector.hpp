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
    template <class Type>
    class iter_rev_impl;

    using iterator = iter_impl<value_type>;
    using const_iterator = iter_impl<const value_type>;
    using reverse_iterator = iter_rev_impl<value_type>;
    using const_reverse_iterator = iter_rev_impl<const value_type>;

    // constructor & destructor
public:
    vector();
    vector(size_type n, const_reference val);

    ~vector();

    void push_back(const_reference val);
    void pop_back();

    // member functions
    void reserve(size_type size);
    void resize(size_type size, value_type val = value_type());
    void clear();
    void shrink_to_fit();

    size_type capacity() const;
    size_type size() const;

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;

    // iterate
    iterator begin();
    iterator end();
    const_iterator end() const;
    const_iterator begin() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

private:
    void realloc(size_type size);
    static inline size_t proper_capacity(size_type size);

    // member-variable
private:
    pointer ptr_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
};

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::proper_capacity(size_type size)
{
    size_type ret = 1U;
    for (; ret < size; ret *= 2)
        ;
    return ret;
}

template <class T, class Alloc>
vector<T, Alloc>::vector()
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const_reference val)
{
    reserve(proper_capacity(n));
    for (size_type i = 0; i < n; ++i) new (ptr_ + i) T(val);
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
vector<T, Alloc>::realloc(size_type size)
{
    capacity_ = size;
    ptr_ = (pointer)std::realloc(ptr_, size * sizeof(T));
}

template <class T, class Alloc>
void
vector<T, Alloc>::resize(size_type size, value_type val)
{
    if (size > size_)
    {
        if (size > capacity_) realloc(proper_capacity(size));
        for (size_t i = size_; i < size; ++i)
        {
            new (ptr_ + i) T(val);
        }
    }
    else
    {
        for (size_t i = size; i < size_; ++i)
        {
            (ptr_ + i)->~T();
        }
    }
    size_ = size;
}

template <class T, class Alloc>
void
vector<T, Alloc>::reserve(size_type size)
{
    if (size == 0)
    {
        free(ptr_);
    }
    else if (size > capacity_)
    {
        realloc(proper_capacity(size));
    }
}

template <class T, class Alloc>
void
vector<T, Alloc>::clear()
{
    for (size_type i = 0; i < size_; ++i) (ptr_ + i)->~T();
    size_ = 0;
}

template <class T, class Alloc>
void
vector<T, Alloc>::shrink_to_fit()
{
    realloc(proper_capacity(size_));
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::size() const
{
    return size_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::capacity() const
{
    return capacity_;
}

template <class T, class Alloc>
void
vector<T, Alloc>::push_back(const_reference val)
{
    if (capacity_ <= size_) realloc(proper_capacity(size_ + 1));
    new (ptr_ + size_) T(val);
    ++size_;
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

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cbegin() const
{
    return const_iterator(ptr_, 0);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cend() const
{
    return const_iterator(ptr_, size_);
}

template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rbegin()
{
    return reverse_iterator(ptr_ + size_, 0);
}

template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rend()
{
    return reverse_iterator(ptr_ + size_, size_);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rbegin() const
{
    return reverse_iterator(ptr_ + size_, 0);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rend() const
{
    return reverse_iterator(ptr_ + size_, size_);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crbegin() const
{
    return const_reverse_iterator(ptr_ + size_, 0);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crend() const
{
    return const_reverse_iterator(ptr_ + size_, size_);
}

//================================= itertor ==================================//

template <class T, class Alloc>
template <class Type>
class vector<T, Alloc>::iter_impl
{
protected:
    using this_t = vector<T, Alloc>::iter_impl<Type>;
    using container = vector<Type, Alloc>;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using size_type = typename container::size_type;
    using difference_type = typename container::difference_type;

public:
    iter_impl(pointer, difference_type);

    reference operator*() const;
    pointer operator->() const;

    this_t& operator++();
    this_t& operator--();
    bool operator!=(const this_t&);
    bool operator==(const this_t&);

protected:
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
Type& vector<T, Alloc>::iter_impl<Type>::iter_impl::operator*() const
{
    return *(ptr_ + offset_);
}

template <class T, class Alloc>
template <class Type>
Type* vector<T, Alloc>::iter_impl<Type>::iter_impl::operator->() const
{
    return ptr_ + offset_;
}

template <class T, class Alloc>
template <class Type>
typename vector<T, Alloc>::template iter_impl<Type>::iter_impl&
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
