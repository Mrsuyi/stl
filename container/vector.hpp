#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "algorithm.hpp"
#include "debug.hpp"
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
    // allocate a mem whose size >= capacity
    T* alloc_new(size_t size);
    // call dtor on old mem, set ptr_ to ptr, and free old mem
    void reside_new(T* ptr);

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
// alloc new
template <class T, class Alloc>
T*
vector<T, Alloc>::alloc_new(size_t size)
{
    if (size == 0) return nullptr;
    capacity_ = 1U;
    for (; capacity_ < size; capacity_ *= 2)
        ;
    return alloc_.allocate(capacity_);
}
// reside new
template <class T, class Alloc>
void
vector<T, Alloc>::reside_new(T* ptr_new)
{
    destroy_n(ptr_, size_);
    alloc_.deallocate(ptr_, size_);
    ptr_ = ptr_new;
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
    mrsuyi::uninitialized_fill_n(ptr_, n, val);
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
    mrsuyi::uninitialized_copy(x.begin(), x.end(), ptr_);
    size_ = x.size_;
}
// move
template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x) : vector(mrsuyi::move(x), Alloc())
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
    reside_new(nullptr);
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
    vector(mrsuyi::move(x)).swap(*this);
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
    mrsuyi::uninitialized_fill_n(ptr_, n, val);
    size_ = n;
}
template <class T, class Alloc>
void
vector<T, Alloc>::assign(std::initializer_list<T> il)
{
    clear();
    reserve(il.size());
    mrsuyi::uninitialized_copy(il.begin(), il.end(), ptr_);
    size_ = il.size();
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
    if (size > capacity_)
    {
        auto new_ptr = alloc_new(size);
        mrsuyi::uninitialized_move_n(ptr_, size_, new_ptr);
        reside_new(new_ptr);
    }
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
    auto new_ptr = alloc_new(size_);
    mrsuyi::uninitialized_move_n(ptr_, size_, new_ptr);
    reside_new(new_ptr);
}

//================================ modifiers =================================//
// clear
template <class T, class Alloc>
void
vector<T, Alloc>::clear()
{
    destroy_n(ptr_, size_);
    size_ = 0;
}
// insert
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, const T& val)
{
    return emplace(pos, val);
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator pos, T&& val)
{
    return emplace(pos, mrsuyi::move(val));
}
template <class T, class Alloc>
void
vector<T, Alloc>::insert(const_iterator pos, size_t n, const T& val)
{
    auto dist = pos - ptr_;

    if (size_ + n > capacity_)
    {
        auto new_ptr = alloc_new(size_ + n);

        uninitialized_move(ptr_, ptr_ + dist, new_ptr);
        destroy(ptr_, ptr_ + dist);

        uninitialized_move(ptr_ + dist, ptr_ + size_, new_ptr + dist + n);
        destory(ptr_ + dist, ptr_ + size_);

        uninitialized_fill_n(new_ptr, n, val);

        reside_new(new_ptr);
        ++size_;
        return const_cast<iterator>(pos);
    }
    else
    {
        // uninitialized_move(ptr_ + dist, ptr_ + size_, ptr_ + dist + 1);
        // construct(ptr_ + dist, mrsuyi::forward<Args>(args)...);
        //++size_;
    }
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
    auto dist = pos - ptr_;

    if (size_ == capacity_)
    {
        auto new_ptr = alloc_new(size_ + 1);

        uninitialized_move(ptr_, ptr_ + dist, new_ptr);
        destory(ptr_, ptr_ + dist);

        uninitialized_move(ptr_ + dist, ptr_ + size_, new_ptr + dist + 1);
        destroy(ptr_ + dist, ptr_ + size_);

        construct(new_ptr + dist, mrsuyi::forward<Args>(args)...);

        reside_new(new_ptr);
        ++size_;
        return const_cast<iterator>(pos);
    }
    else
    {
        //uninitialized_move(ptr_ + dist, ptr_ + size_, ptr_ + dist + 1);
        //construct(ptr_ + dist, mrsuyi::forward<Args>(args)...);
        //++size_;
    }
}
// erase
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator pos)
{
    destroy_at(pos);
    memmove((void*)pos, (const void*)(pos + 1),
            (size_ - (pos - ptr_ + 1)) * sizeof(T));
    --size_;
    return ptr_ + (pos - ptr_);
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    destroy(first, last);
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
    emplace_back(mrsuyi::move(val));
}
// emplace_back
template <class T, class Alloc>
template <class... Args>
void
vector<T, Alloc>::emplace_back(Args&&... args)
{
    emplace(end(), mrsuyi::forward<Args>(args)...);
}
// pop_back
template <class T, class Alloc>
void
vector<T, Alloc>::pop_back()
{
    --size_;
    destroy_at(ptr_ + size_);
}
// resize
template <class T, class Alloc>
void
vector<T, Alloc>::resize(size_t size, T val)
{
    if (size > size_)
    {
        reserve(size);
        mrsuyi::uninitialized_fill_n(ptr_ + size_, size - size_, val);
    }
    else
    {
        destroy_n(ptr_ + size, size_ - size);
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
