#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <initializer_list>
#include <stdexcept>

#include "memory/allocator.hpp"

namespace mrsuyi
{
template <class T, class Alloc = allocator<T>>
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
    using iterator = pointer;
    using const_iterator = const_pointer;

    // member functions
private:
    // ensure capcity_ >= size (size > 0)
    void realloc(size_type size);

public:
    // cons & des
    // default
    vector();
    explicit vector(const allocator_type& alloc);
    // fill
    explicit vector(size_type n,
                    const allocator_type& alloc = allocator_type());
    vector(size_type n, const_reference val,
           const allocator_type& alloc = allocator_type());
    // range
    template <class InputIterator>
    vector(InputIterator first, InputIterator last,
           const allocator_type& alloc = allocator_type(),
           typename std::enable_if<
               !std::is_integral<InputIterator>::value>::type* = 0);
    // copy
    explicit vector(const vector& x);
    vector(const vector& x, const allocator_type& alloc);
    // move
    vector(vector&& x);
    vector(vector&& x, const allocator_type& alloc);
    // list
    vector(std::initializer_list<value_type> il,
           const allocator_type& alloc = allocator_type());
    // des
    ~vector() noexcept;

    // mem
    void reserve(size_type size);
    void resize(size_type size, value_type val = value_type());
    void clear();
    void shrink_to_fit();
    size_type capacity() const;
    size_type size() const;

    // set
    void push_back(const_reference val);
    void pop_back();

    iterator insert(iterator position, const_reference val);
    void insert(iterator position, size_type n, const_reference val);
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args);
    template <class... Args>
    void emplace_back(Args&&... args);

    // get
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;

    // iterate
    iterator begin();
    iterator end();
    const_iterator end() const;
    const_iterator begin() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    // reverse_iterator rbegin();
    // reverse_iterator rend();
    // const_reverse_iterator rbegin() const;
    // const_reverse_iterator rend() const;
    // const_reverse_iterator crbegin() const;
    // const_reverse_iterator crend() const;

    // member-variable
private:
    pointer ptr_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
    allocator_type alloc_;
};

// cons & des
template <class T, class Alloc>
vector<T, Alloc>::vector() : vector(allocator_type())
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const allocator_type& alloc) : alloc_(alloc)
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const allocator_type& alloc)
    : vector(n, value_type(), alloc)
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(size_type n, const_reference val,
                         const allocator_type& alloc)
    : alloc_(alloc)
{
    reserve(n);
    for (size_type i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    size_ = n;
}

template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(
    InputIterator first, InputIterator last, const allocator_type& alloc,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
    : alloc_(alloc)
{
    for (; first != last; ++first) push_back(*first);
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& x) : vector(x, allocator_type())
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& x, const allocator_type& alloc)
    : alloc_(alloc)
{
    reserve(x.size_);
    for (size_type i = 0; i < x.size_; ++i)
        alloc_.construct(ptr_ + i, *(x.ptr_ + i));
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x) : vector(std::move(x), allocator_type())
{
}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x, const allocator_type& alloc)
    : alloc_(alloc)
{
    ptr_ = x.ptr_;
    size_ = x.size_;
    capacity_ = x.capacity_;
}

template <class T, class Alloc>
vector<T, Alloc>::~vector() noexcept
{
    for (size_type i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    alloc_.deallocate(ptr_, capacity_);
}

//================ mem ===============//
template <class T, class Alloc>
void
vector<T, Alloc>::realloc(size_type size)
{
    capacity_ = 1U;
    for (; capacity_ < size; capacity_ *= 2)
        ;
    ptr_ = alloc_.allocate(capacity_, ptr_);
}

template <class T, class Alloc>
void
vector<T, Alloc>::resize(size_type size, value_type val)
{
    if (size > size_)
    {
        if (size > capacity_) realloc(size);
        for (size_type i = size_; i < size; ++i)
            alloc_.construct(ptr_ + i, val);
    }
    else
    {
        for (size_type i = size; i < size_; ++i) alloc_.destroy(ptr_ + i);
    }
    size_ = size;
}

template <class T, class Alloc>
void
vector<T, Alloc>::reserve(size_type size)
{
    if (size > capacity_) realloc(size);
}

template <class T, class Alloc>
void
vector<T, Alloc>::clear()
{
    for (size_type i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    size_ = 0;
}

template <class T, class Alloc>
void
vector<T, Alloc>::shrink_to_fit()
{
    if (size_ == 0)
        alloc_.deallocate(ptr_, capacity_);
    else
        realloc(size_);
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

//================ set ===============//
template <class T, class Alloc>
void
vector<T, Alloc>::push_back(const_reference val)
{
    if (capacity_ <= size_) realloc(size_ + 1);
    alloc_.construct(ptr_ + size_, val);
    ++size_;
}

template <class T, class Alloc>
void
vector<T, Alloc>::pop_back()
{
    --size_;
    alloc_.destroy(ptr_ + size_);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator position, const_reference val)
{
    difference_type diff = position - ptr_;
    ++size_;
    reserve(size_);
    pointer pos = ptr_ + diff;
    memmove((void*)(pos + 1), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(value_type));
    alloc_.construct(pos, val);
    return pos;
}

template <class T, class Alloc>
void
vector<T, Alloc>::insert(iterator position, size_type n, const_reference val)
{
    difference_type diff = position - ptr_;
    size_ += n;
    reserve(size_);
    pointer pos = ptr_ + diff;
    memmove((void*)(pos + n), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(value_type));
    for (size_type i = 0; i < n; ++i) alloc_.construct(pos + i, val);
}

template <class T, class Alloc>
template <class InputIterator>
void
vector<T, Alloc>::insert(
    iterator position, InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    for (; first != last; ++first) position = insert(position, *first) + 1;
}

template <class T, class Alloc>
template <class... Args>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::emplace(const_iterator position, Args&&... args)
{
    difference_type diff = position - ptr_;
    ++size_;
    reserve(size_);
    pointer pos = ptr_ + diff;
    memmove((void*)(pos + 1), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(value_type));
    alloc_.construct(pos, std::forward<Args>(args)...);
    return pos;
}

template <class T, class Alloc>
template <class... Args>
void
vector<T, Alloc>::emplace_back(Args&&... args)
{
    ++size_;
    reserve(size_);
    alloc_.construct(ptr_ + size_ - 1, std::forward<Args>(args)...);
}

//================ get ===============//
template <class T, class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::front()
{
    return *ptr_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::front() const
{
    return *ptr_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::back()
{
    return *(ptr_ + size_ - 1);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::back() const
{
    return *(ptr_ + size_ - 1);
}

template <class T, class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::at(size_type n)
{
    if (n >= size_) throw std::out_of_range("out of range");
    return *(ptr_ + n);
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::at(size_type n) const
{
    if (n >= size_) throw std::out_of_range("out of range");
    return *(ptr_ + n);
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

//============== iterate =============//
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::begin()
{
    return ptr_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::end()
{
    return ptr_ + size_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::begin() const
{
    return ptr_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::end() const
{
    return ptr_ + size_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cbegin() const
{
    return ptr_;
}

template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cend() const
{
    return ptr_ + size_;
}
}
