#pragma once

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "utility.hpp"

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
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

private:
    // ensure capcity_ >= size (size > 0)
    void realloc(size_t size);

public:
    // ctor & dtor
    // default
    vector();
    explicit vector(const Alloc& alloc);
    // fill
    explicit vector(size_t n, const Alloc& alloc = Alloc());
    explicit vector(size_t n, const T& val, const Alloc& alloc = Alloc());
    // range
    template <class InputIterator>
    vector(InputIterator first, InputIterator last,
           const Alloc& alloc = Alloc(),
           typename std::enable_if<
               !std::is_integral<InputIterator>::value>::type* = 0);
    // copy
    vector(const vector& x);
    vector(const vector& x, const Alloc& alloc);
    // move
    vector(vector&& x);
    vector(vector&& x, const Alloc& alloc);
    // list
    vector(std::initializer_list<T> il, const Alloc& alloc = Alloc());
    // des
    ~vector() noexcept;

    // assignment
    vector& operator=(const vector& x);
    vector& operator=(vector&& x);
    vector& operator=(std::initializer_list<T> il);

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void assign(size_t n, const T& val);
    void assign(std::initializer_list<T> il);

    Alloc get_allocator() const noexcept;

    // element access
    T& at(size_t n);
    const T& at(size_t n) const;
    T& operator[](size_t n);
    const T& operator[](size_t n) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data() noexcept;
    const T* data() const noexcept;

    // iterators
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

    // capacity
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t max_size() const;
    void reserve(size_t size);
    size_t capacity() const noexcept;
    void shrink_to_fit();

    // modifiers
    void clear();

    iterator insert(const_iterator pos, const T& val);
    iterator insert(const_iterator pos, T&& val);
    void insert(const_iterator pos, size_t n, const T& val);
    template <class InputIterator>
    void insert(const_iterator pos, InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void insert(const_iterator pos, std::initializer_list<T> il);

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void push_back(const T& val);
    void push_back(T&& val);
    template <class... Args>
    void emplace_back(Args&&... args);
    void pop_back();

    void resize(size_t size, T val = T());

    void swap(vector& x);

protected:
    T* ptr_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    Alloc alloc_;
};

//================================ private ===================================//
template <class T, class Alloc>
void
vector<T, Alloc>::realloc(size_t size)
{
    capacity_ = 1U;
    for (; capacity_ < size; capacity_ *= 2)
        ;
    ptr_ = alloc_.allocate(capacity_, ptr_);
}

//================================== basic ===================================//
// default
template <class T, class Alloc>
vector<T, Alloc>::vector() : vector(Alloc())
{
}
template <class T, class Alloc>
vector<T, Alloc>::vector(const Alloc& alloc) : alloc_(alloc)
{
}
// fill
template <class T, class Alloc>
vector<T, Alloc>::vector(size_t n, const Alloc& alloc) : vector(n, T(), alloc)
{
}
template <class T, class Alloc>
vector<T, Alloc>::vector(size_t n, const T& val, const Alloc& alloc)
    : alloc_(alloc)
{
    reserve(n);
    for (size_t i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    size_ = n;
}
// range
template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(
    InputIterator first, InputIterator last, const Alloc& alloc,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
    : alloc_(alloc)
{
    for (; first != last; ++first) push_back(*first);
}
// copy
template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& x) : vector(x, Alloc())
{
}
template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& x, const Alloc& alloc) : alloc_(alloc)
{
    reserve(x.size_);
    size_ = x.size_;
    for (size_t i = 0; i < x.size_; ++i) alloc_.construct(ptr_ + i, x.ptr_[i]);
}
// move
template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x) : vector(std::move(x), Alloc())
{
}
template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x, const Alloc& alloc) : alloc_(alloc)
{
    ptr_ = x.ptr_;
    size_ = x.size_;
    capacity_ = x.capacity_;
    x.ptr_ = nullptr;
    x.size_ = 0;
    x.capacity_ = 0;
}
// list
template <class T, class Alloc>
vector<T, Alloc>::vector(std::initializer_list<T> il, const Alloc& alloc)
    : vector(il.begin(), il.end(), alloc)
{
}
// dtor
template <class T, class Alloc>
vector<T, Alloc>::~vector() noexcept
{
    for (size_t i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    alloc_.deallocate(ptr_, capacity_);
}
// ==
template <class T, class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(const vector& x)
{
    vector(x).swap(*this);
    return *this;
}

template <class T, class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(vector&& x)
{
    vector(std::move(x)).swap(*this);
    return *this;
}

template <class T, class Alloc>
vector<T, Alloc>&
vector<T, Alloc>::operator=(std::initializer_list<T> il)
{
    vector(il).swap(*this);
    return *this;
}
// assign
template <class T, class Alloc>
template <class InputIterator>
void
vector<T, Alloc>::assign(
    InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    clear();
    for (; first != last; ++first) push_back(*first);
}
template <class T, class Alloc>
void
vector<T, Alloc>::assign(size_t n, const T& val)
{
    clear();
    reserve(n);
    for (size_t i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    size_ = n;
}
template <class T, class Alloc>
void
vector<T, Alloc>::assign(std::initializer_list<T> il)
{
    clear();
    reserve(il.size());
    for (auto it = il.begin(); it != il.end(); ++it)
    {
        alloc_.construct(ptr_ + size_, *it);
        ++size_;
    }
}
// get_allocator
template <class T, class Alloc>
Alloc
vector<T, Alloc>::get_allocator() const noexcept
{
    return alloc_;
}

//============================= element access ===============================//
// at
template <class T, class Alloc>
T&
vector<T, Alloc>::at(size_t n)
{
    if (n >= size_) throw std::out_of_range("out of range");
    return ptr_[n];
}
template <class T, class Alloc>
const T&
vector<T, Alloc>::at(size_t n) const
{
    if (n >= size_) throw std::out_of_range("out of range");
    return ptr_[n];
}
// []
template <class T, class Alloc>
T& vector<T, Alloc>::operator[](size_t n)
{
    return ptr_[n];
}
template <class T, class Alloc>
const T& vector<T, Alloc>::operator[](size_t n) const
{
    return ptr_[n];
}
// front
template <class T, class Alloc>
T&
vector<T, Alloc>::front()
{
    return ptr_[0];
}
template <class T, class Alloc>
const T&
vector<T, Alloc>::front() const
{
    return ptr_[0];
}
// back
template <class T, class Alloc>
T&
vector<T, Alloc>::back()
{
    return ptr_[size_ - 1];
}
template <class T, class Alloc>
const T&
vector<T, Alloc>::back() const
{
    return ptr_[size_ - 1];
}
// data
template <class T, class Alloc>
T*
vector<T, Alloc>::data() noexcept
{
    return ptr_;
}
template <class T, class Alloc>
const T*
vector<T, Alloc>::data() const noexcept
{
    return ptr_;
}

//================================ iterators =================================//
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::begin() noexcept
{
    return ptr_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::end() noexcept
{
    return ptr_ + size_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::begin() const noexcept
{
    return ptr_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::end() const noexcept
{
    return ptr_ + size_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cbegin() const noexcept
{
    return ptr_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cend() const noexcept
{
    return ptr_ + size_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rbegin() noexcept
{
    return reverse_iterator(end());
}
template <class T, class Alloc>
typename vector<T, Alloc>::reverse_iterator
vector<T, Alloc>::rend() noexcept
{
    return reverse_iterator(begin());
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_reverse_iterator
vector<T, Alloc>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

//================================ capacity ==================================//
template <class T, class Alloc>
bool
vector<T, Alloc>::empty() const noexcept
{
    return size_ == 0;
}
template <class T, class Alloc>
size_t
vector<T, Alloc>::size() const noexcept
{
    return size_;
}
template <class T, class Alloc>
size_t
vector<T, Alloc>::max_size() const
{
    return std::numeric_limits<size_t>::max();
}
template <class T, class Alloc>
void
vector<T, Alloc>::reserve(size_t size)
{
    if (size > capacity_) realloc(size);
}
template <class T, class Alloc>
size_t
vector<T, Alloc>::capacity() const noexcept
{
    return capacity_;
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

//================================ modifiers =================================//
// clear
template <class T, class Alloc>
void
vector<T, Alloc>::clear()
{
    for (size_t i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    size_ = 0;
}
// insert
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, const T& val)
{
    difference_type diff = pos - ptr_;
    ++size_;
    reserve(size_);
    pos = ptr_ + diff;
    memmove((void*)(pos + 1), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(T));
    alloc_.construct(pos, val);
    return const_cast<iterator>(pos);
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, T&& val)
{
    difference_type diff = pos - ptr_;
    ++size_;
    reserve(size_);
    pos = ptr_ + diff;
    memmove((void*)(pos + 1), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(T));
    alloc_.construct(pos, std::move(val));
    return const_cast<iterator>(pos);
}
template <class T, class Alloc>
void
vector<T, Alloc>::insert(const_iterator pos, size_t n, const T& val)
{
    difference_type diff = pos - ptr_;
    size_ += n;
    reserve(size_);
    pos = ptr_ + diff;
    memmove((void*)(pos + n), (const void*)(pos),
            (size_ - (pos - ptr_ + n)) * sizeof(T));
    for (size_t i = 0; i < n; ++i) alloc_.construct(pos + i, val);
}
template <class T, class Alloc>
template <class InputIterator>
void
vector<T, Alloc>::insert(
    const_iterator pos, InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    for (; first != last; ++first) pos = insert(pos, *first) + 1;
}
template <class T, class Alloc>
void
vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> il)
{
    insert(pos, il.begin(), il.end());
}
// emplace
template <class T, class Alloc>
template <class... Args>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::emplace(const_iterator pos, Args&&... args)
{
    difference_type diff = pos - ptr_;
    ++size_;
    reserve(size_);
    pos = ptr_ + diff;
    memmove((void*)(pos + 1), (const void*)(pos),
            (size_ - (pos - ptr_ + 1)) * sizeof(T));
    alloc_.construct(pos, std::forward<Args>(args)...);
    return const_cast<iterator>(pos);
}
// erase
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator pos)
{
    alloc_.destroy(pos);
    memmove((void*)pos, (const void*)(pos + 1),
            (size_ - (pos - ptr_ + 1)) * sizeof(T));
    --size_;
    return ptr_ + (pos - ptr_);
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    for (auto iter = first; iter != last; ++iter) alloc_.destroy(iter);
    memmove((void*)first, (const void*)last,
            (size_ - (last - ptr_)) * sizeof(T));
    size_ -= last - first;
    return ptr_ + (first - ptr_);
}
// push_back
template <class T, class Alloc>
void
vector<T, Alloc>::push_back(const T& val)
{
    emplace_back(val);
}
template <class T, class Alloc>
void
vector<T, Alloc>::push_back(T&& val)
{
    emplace_back(std::move(val));
}
// emplace_back
template <class T, class Alloc>
template <class... Args>
void
vector<T, Alloc>::emplace_back(Args&&... args)
{
    ++size_;
    reserve(size_);
    alloc_.construct(ptr_ + size_ - 1, std::forward<Args>(args)...);
}
// pop_back
template <class T, class Alloc>
void
vector<T, Alloc>::pop_back()
{
    --size_;
    alloc_.destroy(ptr_ + size_);
}
// resize
template <class T, class Alloc>
void
vector<T, Alloc>::resize(size_t size, T val)
{
    if (size > size_)
    {
        if (size > capacity_) realloc(size);
        for (size_t i = size_; i < size; ++i) alloc_.construct(ptr_ + i, val);
    }
    else
    {
        for (size_t i = size; i < size_; ++i) alloc_.destroy(ptr_ + i);
    }
    size_ = size;
}
// swap
template <class T, class Alloc>
void
vector<T, Alloc>::swap(vector& x)
{
    mrsuyi::swap(ptr_, x.ptr_);
    mrsuyi::swap(size_, x.size_);
    mrsuyi::swap(capacity_, x.capacity_);
}

//=========================== non-member functions ===========================//
template <class T, class Alloc>
void
swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
{
    x.swap(y);
}
template <class T, class Alloc>
bool
operator==(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return lhs.size() == rhs.size() &&
           equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <class T, class Alloc>
bool
operator!=(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return !(lhs == rhs);
}
template <class T, class Alloc>
bool
operator<(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                   rhs.end());
}
template <class T, class Alloc>
bool
operator<=(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return !(rhs < lhs);
}
template <class T, class Alloc>
bool
operator>(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return rhs < lhs;
}
template <class T, class Alloc>
bool
operator>=(const vector<T, Alloc>& lhs, const vector<T>& rhs)
{
    return !(lhs < rhs);
}
}
