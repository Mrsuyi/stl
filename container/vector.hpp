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
    // estimate a proper capacity >= [size]
    size_t overmeasure(size_t);
    // migrate to a new block of mem
    void migrate(size_t);
    // embed a block of mem inside current mem. new-alloc may happen
    void embed(T*, size_t);

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
           typename mrsuyi::enable_if<
               !mrsuyi::is_integral<InputIterator>::value>::type* = 0);
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
                typename mrsuyi::enable_if<
                    !mrsuyi::is_integral<InputIterator>::value>::type* = 0);
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
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
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
                typename mrsuyi::enable_if<
                    !mrsuyi::is_integral<InputIterator>::value>::type* = 0);
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
    T *bgn_ = nullptr, *end_ = nullptr, *cap_ = nullptr;
    Alloc alloc_;
};

//================================ private ===================================//
//
template <class T, class Alloc>
size_t
vector<T, Alloc>::overmeasure(size_t size)
{
    size_t cap = 1U;
    for (; cap < size; cap *= 2)
        ;
    return cap;
}
//
template <class T, class Alloc>
void
vector<T, Alloc>::migrate(size_t cap)
{
    auto new_mem = alloc_.allocate(cap);

    mrsuyi::uninitialized_move(bgn_, end_, new_mem);
    mrsuyi::destroy(bgn_, end_);

    alloc_.deallocate(bgn_, capacity());
    end_ = new_mem + size();
    cap_ = new_mem + cap;
    bgn_ = new_mem;
}
//
template <class T, class Alloc>
void
vector<T, Alloc>::embed(T* pos, size_t n)
{
    if (size() + n > capacity())
    {
        auto cap = overmeasure(size() + n);
        auto new_mem = alloc_.allocate(cap);

        mrsuyi::uninitialized_move(bgn_, pos, new_mem);
        mrsuyi::uninitialized_move(pos, end_, new_mem + (pos - bgn_) + n);
        mrsuyi::destroy(bgn_, end_);

        alloc_.deallocate(bgn_, capacity());
        end_ = new_mem + size() + n;
        cap_ = new_mem + cap;
        bgn_ = new_mem;
    }
    else
    {
        for (auto p = (end_ - 1); p >= pos; --p)
        {
            mrsuyi::construct(p + n, mrsuyi::move(*p));
            mrsuyi::destroy_at(p);
        }
        end_ += n;
    }
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
    auto cap = overmeasure(n);
    bgn_ = alloc_.allocate(cap);
    end_ = bgn_ + n;
    cap_ = bgn_ + cap;
    mrsuyi::uninitialized_fill(bgn_, end_, val);
}
// range
template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last,
                         const Alloc& alloc,
                         typename mrsuyi::enable_if<
                             !mrsuyi::is_integral<InputIterator>::value>::type*)
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
    bgn_ = alloc_.allocate(x.capacity());
    end_ = bgn_ + x.size();
    cap_ = bgn_ + x.capacity();
    mrsuyi::uninitialized_copy(x.begin(), x.end(), bgn_);
}
// move
template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x) : vector(mrsuyi::move(x), Alloc())
{
}
template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& x, const Alloc& alloc) : alloc_(alloc)
{
    bgn_ = x.bgn_;
    end_ = x.end_;
    cap_ = x.cap_;
    x.bgn_ = nullptr;
    x.end_ = nullptr;
    x.cap_ = nullptr;
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
    mrsuyi::destroy(bgn_, end_);
    alloc_.deallocate(bgn_, capacity());
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
vector<T, Alloc>::assign(InputIterator first, InputIterator last,
                         typename mrsuyi::enable_if<
                             !mrsuyi::is_integral<InputIterator>::value>::type*)
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
    end_ = bgn_ + n;
    mrsuyi::uninitialized_fill(bgn_, end_, val);
}
template <class T, class Alloc>
void
vector<T, Alloc>::assign(std::initializer_list<T> il)
{
    clear();
    reserve(il.size());
    end_ = bgn_ + il.size();
    mrsuyi::uninitialized_copy(il.begin(), il.end(), bgn_);
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
    if (n >= size()) throw std::out_of_range("out of range");
    return bgn_[n];
}
template <class T, class Alloc>
const T&
vector<T, Alloc>::at(size_t n) const
{
    if (n >= size()) throw std::out_of_range("out of range");
    return bgn_[n];
}
// []
template <class T, class Alloc>
T& vector<T, Alloc>::operator[](size_t n)
{
    return bgn_[n];
}
template <class T, class Alloc>
const T& vector<T, Alloc>::operator[](size_t n) const
{
    return bgn_[n];
}
// front
template <class T, class Alloc>
T&
vector<T, Alloc>::front()
{
    return bgn_[0];
}
template <class T, class Alloc>
const T&
vector<T, Alloc>::front() const
{
    return bgn_[0];
}
// back
template <class T, class Alloc>
T&
vector<T, Alloc>::back()
{
    return end_[-1];
}
template <class T, class Alloc>
const T&
vector<T, Alloc>::back() const
{
    return end_[-1];
}
// data
template <class T, class Alloc>
T*
vector<T, Alloc>::data() noexcept
{
    return bgn_;
}
template <class T, class Alloc>
const T*
vector<T, Alloc>::data() const noexcept
{
    return bgn_;
}

//================================ iterators =================================//
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::begin() noexcept
{
    return bgn_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::end() noexcept
{
    return end_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::begin() const noexcept
{
    return bgn_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::end() const noexcept
{
    return end_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cbegin() const noexcept
{
    return bgn_;
}
template <class T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::cend() const noexcept
{
    return end_;
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
    return bgn_ == end_;
}
template <class T, class Alloc>
size_t
vector<T, Alloc>::size() const noexcept
{
    return end_ - bgn_;
}
template <class T, class Alloc>
size_t
vector<T, Alloc>::max_size() const
{
    return std::numeric_limits<size_t>::max();
}
template <class T, class Alloc>
void
vector<T, Alloc>::reserve(size_t cap)
{
    if (cap > capacity()) migrate(overmeasure(cap));
}
template <class T, class Alloc>
size_t
vector<T, Alloc>::capacity() const noexcept
{
    return cap_ - bgn_;
}
template <class T, class Alloc>
void
vector<T, Alloc>::shrink_to_fit()
{
    auto cap = overmeasure(size());
    if (cap < capacity()) migrate(cap);
}

//================================ modifiers =================================//
// clear
template <class T, class Alloc>
void
vector<T, Alloc>::clear()
{
    destroy(bgn_, end_);
    end_ = bgn_;
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
    auto dist = pos - bgn_;
    embed(const_cast<iterator>(pos), n);
    mrsuyi::uninitialized_fill_n(bgn_ + dist, n, val);
}
template <class T, class Alloc>
template <class InputIterator>
void
vector<T, Alloc>::insert(const_iterator pos, InputIterator first,
                         InputIterator last,
                         typename mrsuyi::enable_if<
                             !mrsuyi::is_integral<InputIterator>::value>::type*)
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
    auto dist = pos - bgn_;
    embed(const_cast<iterator>(pos), 1);
    construct(bgn_ + dist, mrsuyi::forward<Args>(args)...);
    return bgn_ + dist;
}
// erase
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator pos)
{
    return erase(pos, pos + 1);
}
template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    destroy(first, last);
    auto res = first;
    for (; last != cend(); ++first, ++last)
    {
        construct(const_cast<iterator>(first), mrsuyi::move(*last));
        destroy_at(last);
    }
    end_ -= last - first;
    return const_cast<iterator>(res);
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
    --end_;
    destroy_at(end_);
}
// resize
template <class T, class Alloc>
void
vector<T, Alloc>::resize(size_t new_size, T val)
{
    if (new_size > size())
    {
        reserve(new_size);
        mrsuyi::uninitialized_fill_n(end_, new_size - size(), val);
    }
    else
    {
        destroy(bgn_ + new_size, end_);
    }
    end_ = bgn_ + new_size;
}
// swap
template <class T, class Alloc>
void
vector<T, Alloc>::swap(vector& x)
{
    mrsuyi::swap(bgn_, x.bgn_);
    mrsuyi::swap(end_, x.end_);
    mrsuyi::swap(cap_, x.cap_);
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
