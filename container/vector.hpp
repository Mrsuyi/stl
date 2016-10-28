#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace mrsuyi
{
template <class T, template <class> class Alloc = allocator>
class vector
{
    // typedefs
public:
    using value_type = T;
    using allocator_type = Alloc<T>;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    // iterators
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

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
    vector(const vector& x);
    vector(const vector& x, const allocator_type& alloc);
    // move
    vector(vector&& x);
    vector(vector&& x, const allocator_type& alloc);
    // list
    vector(std::initializer_list<value_type> il,
           const allocator_type& alloc = allocator_type());
    // des
    ~vector() noexcept;

    vector& operator=(const vector& x);
    vector& operator=(vector&& x);
    vector& operator=(std::initializer_list<value_type> il);

    // mem
    allocator_type get_allocator() const noexcept;
    void reserve(size_type size);
    void resize(size_type size, value_type val = value_type());
    void clear();
    void shrink_to_fit();
    size_type capacity() const noexcept;
    size_type size() const noexcept;
    bool empty() const noexcept;

    // set
    void swap(vector& x);
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void assign(size_type n, const_reference val);
    void assign(std::initializer_list<value_type> il);

    void push_back(const_reference val);
    void push_back(value_type&& val);
    void pop_back();

    iterator insert(const_iterator position, const_reference val);
    iterator insert(const_iterator position, value_type&& val);
    void insert(const_iterator position, size_type n, const_reference val);
    template <class InputIterator>
    void insert(const_iterator position, InputIterator first,
                InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

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
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // iterate
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // member-variable
private:
    pointer ptr_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
    allocator_type alloc_;
};

// ctor & dtor
// default
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector() : vector(allocator_type())
{
}
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(const allocator_type& alloc) : alloc_(alloc)
{
}
// fill
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(size_type n, const allocator_type& alloc)
    : vector(n, value_type(), alloc)
{
}
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(size_type n, const_reference val,
                         const allocator_type& alloc)
    : alloc_(alloc)
{
    reserve(n);
    for (size_type i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    size_ = n;
}
// range
template <class T, template <class> class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(
    InputIterator first, InputIterator last, const allocator_type& alloc,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
    : alloc_(alloc)
{
    for (; first != last; ++first) push_back(*first);
}
// copy
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(const vector& x) : vector(x, allocator_type())
{
}
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(const vector& x, const allocator_type& alloc)
    : alloc_(alloc)
{
    reserve(x.size_);
    size_ = x.size_;
    for (size_type i = 0; i < x.size_; ++i)
        alloc_.construct(ptr_ + i, *(x.ptr_ + i));
}
// move
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(vector&& x) : vector(move(x), allocator_type())
{
}
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(vector&& x, const allocator_type& alloc)
    : alloc_(alloc)
{
    ptr_ = x.ptr_;
    size_ = x.size_;
    capacity_ = x.capacity_;
}
// list
template <class T, template <class> class Alloc>
vector<T, Alloc>::vector(std::initializer_list<value_type> il,
                         const allocator_type& alloc)
    : vector(il.begin(), il.end(), alloc)
{
}

template <class T, template <class> class Alloc>
vector<T, Alloc>::~vector() noexcept
{
    for (size_type i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    alloc_.deallocate(ptr_, capacity_);
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::swap(vector& x)
{
    std::swap(ptr_, x.ptr_);
    std::swap(size_, x.size_);
    std::swap(capacity_, x.capacity_);
}

template <class T, template <class> class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(const vector& x)
{
    vector(x).swap(*this);
    return *this;
}

template <class T, template <class> class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(vector&& x)
{
    vector(move(x)).swap(*this);
    return *this;
}

template <class T, template <class> class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(std::initializer_list<value_type> il)
{
    vector(il).swap(*this);
    return *this;
}

//================ mem ===============//
template <class T, template <class> class Alloc>
typename vector<T, Alloc>::allocator_type
vector<T, Alloc>::get_allocator() const noexcept
{
    return alloc_;
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::realloc(size_type size)
{
    capacity_ = 1U;
    for (; capacity_ < size; capacity_ *= 2)
        ;
    ptr_ = alloc_.allocate(capacity_, ptr_);
}

template <class T, template <class> class Alloc>
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

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::reserve(size_type size)
{
    if (size > capacity_) realloc(size);
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::clear()
{
    for (size_type i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    size_ = 0;
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::shrink_to_fit()
{
    if (size_ == 0)
        alloc_.deallocate(ptr_, capacity_);
    else
        realloc(size_);
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::size() const noexcept
{
    return size_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::size_type
vector<T, Alloc>::capacity() const noexcept
{
    return capacity_;
}

template <class T, template <class> class Alloc>
bool
vector<T, Alloc>::empty() const noexcept
{
    return size_ == 0;
}

//================ set ===============//
template <class T, template <class> class Alloc>
template <class InputIterator>
void
vector<T, Alloc>::assign(
    InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    clear();
    for (; first != last; ++first) push_back(*first);
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::assign(size_type n, const_reference val)
{
    clear();
    for (size_type i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    size_ = n;
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::assign(std::initializer_list<value_type> il)
{
    clear();
    reserve(il.size());
    for (auto it = il.begin(); it != il.end(); ++it)
    {
        alloc_.construct(ptr_ + size_, *it);
        ++size_;
    }
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::push_back(const_reference val)
{
    if (capacity_ <= size_) realloc(size_ + 1);
    alloc_.construct(ptr_ + size_, val);
    ++size_;
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::push_back(value_type&& val)
{
    if (capacity_ <= size_) realloc(size_ + 1);
    alloc_.construct(ptr_ + size_, move(val));
    ++size_;
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::pop_back()
{
    --size_;
    alloc_.destroy(ptr_ + size_);
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator position, const_reference val)
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

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator position, value_type&& val)
{
    difference_type diff = position - ptr_;
    ++size_;
    reserve(size_);
    pointer pos = ptr_ + diff;
    memmove((void*)(pos + 1), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(value_type));
    alloc_.construct(pos, move(val));
    return pos;
}

template <class T, template <class> class Alloc>
void
vector<T, Alloc>::insert(const_iterator position, size_type n,
                         const_reference val)
{
    difference_type diff = position - ptr_;
    size_ += n;
    reserve(size_);
    pointer pos = ptr_ + diff;
    memmove((void*)(pos + n), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(value_type));
    for (size_type i = 0; i < n; ++i) alloc_.construct(pos + i, val);
}

template <class T, template <class> class Alloc>
template <class InputIterator>
void
vector<T, Alloc>::insert(
    const_iterator position, InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    iterator pos = ptr_ + (position - ptr_);
    for (; first != last; ++first) pos = insert(pos, *first) + 1;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator position)
{
    alloc_.destroy(position);
    memmove((void*)position, (const void*)(position + 1),
            (size_ - (position - ptr_ + 1)) * sizeof(value_type));
    --size_;
    return ptr_ + (position - ptr_);
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    for (auto iter = first; iter != last; ++iter) alloc_.destroy(iter);
    memmove((void*)first, (const void*)last,
            (size_ - (last - ptr_)) * sizeof(value_type));
    size_ -= last - first;
    return ptr_ + (first - ptr_);
}

template <class T, template <class> class Alloc>
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
    alloc_.construct(pos, forward<Args>(args)...);
    return pos;
}

template <class T, template <class> class Alloc>
template <class... Args>
void
vector<T, Alloc>::emplace_back(Args&&... args)
{
    ++size_;
    reserve(size_);
    alloc_.construct(ptr_ + size_ - 1, forward<Args>(args)...);
}

//================ get ===============//
template <class T, template <class> class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::front()
{
    return ptr_[0];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::front() const
{
    return ptr_[0];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::back()
{
    return ptr_[size_ - 1];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::back() const
{
    return ptr_[size_ - 1];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::reference
vector<T, Alloc>::at(size_type n)
{
    if (n >= size_) throw std::out_of_range("out of range");
    return ptr_[n];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reference
vector<T, Alloc>::at(size_type n) const
{
    if (n >= size_) throw std::out_of_range("out of range");
    return ptr_[n];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type n)
{
    return ptr_[n];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](
    size_type n) const
{
    return ptr_[n];
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::pointer
vector<T, Alloc>::data() noexcept
{
    return ptr_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_pointer
vector<T, Alloc>::data() const noexcept
{
    return ptr_;
}

//============== iterate =============//
template <class T, template <class> class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::begin() noexcept
{
    return ptr_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::end() noexcept
{
    return ptr_ + size_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::begin() const noexcept
{
    return ptr_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::end() const noexcept
{
    return ptr_ + size_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cbegin() const noexcept
{
    return ptr_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cend() const noexcept
{
    return ptr_ + size_;
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, template <class> class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

//=========================== non-member functions ===========================//
template <class T, template <class> class Alloc>
bool
operator==(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return lhs.size() == rhs.size() &&
           equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, template <class> class Alloc>
bool
operator!=(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T, template <class> class Alloc>
bool
operator<(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                   rhs.end());
}

template <class T, template <class> class Alloc>
bool
operator<=(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return !(rhs < lhs);
}

template <class T, template <class> class Alloc>
bool
operator>(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return rhs < lhs;
}

template <class T, template <class> class Alloc>
bool
operator>=(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return !(lhs < rhs);
}

template <class T, template <class> class Alloc>
void
swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
{
    x.swap(y);
}
}
