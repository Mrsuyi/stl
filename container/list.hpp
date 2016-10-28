#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"

namespace mrsuyi
{
template <class T, class Alloc = allocator<T>>
class list
{
    struct node
    {
        node *pre, *nxt;
        T t;

        node() : pre(this), nxt(this) {}
        node(const T& t) : pre(this), nxt(this), t(t) {}
        node(T&& t) : pre(this), nxt(this), t(std::move(t)) {}
    };

    template <class E>
    class iter;

    // typedefs
public:
    using value_type = T;
    using allocator_type = typename Alloc::template other<node>;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    // iterators
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

    // member-function
public:
    // ctor & dtor
    // default
    list();
    explicit list(const allocator_type& alloc);
    // fill
    explicit list(size_type n, const allocator_type& alloc = allocator_type());
    list(size_type n, const_reference val,
         const allocator_type& alloc = allocator_type());
    // range
    template <class InputIterator>
    list(InputIterator first, InputIterator last,
         const allocator_type& alloc = allocator_type(),
         typename std::enable_if<
             !std::is_integral<InputIterator>::value>::type* = 0);
    // copy
    list(const list& x);
    list(const list& x, const allocator_type& alloc);
    // move
    list(list&& x);
    list(list&& x, const allocator_type& alloc);
    // list
    list(std::initializer_list<value_type> il,
         const allocator_type& alloc = allocator_type());
    // dtor
    ~list() noexcept;
    // =
    list& operator=(const list& x);
    list& operator=(list&& x);
    list& operator=(std::initializer_list<value_type> il);
    // assign
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void assign(size_type n, const_reference val);
    void assign(std::initializer_list<value_type> il);
    // alloc
    allocator_type get_allocator() const noexcept;

    // element access
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

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
    size_type size() const noexcept;
    bool empty() const noexcept;
    size_type max_size() const noexcept;

    // modifiers
    void clear();
    void resize(size_type size, value_type val = value_type());
    void swap(list& x);

    void push_front(const_reference val);
    void push_front(value_type&& val);
    void push_back(const_reference val);
    void push_back(value_type&& val);

    void pop_back();
    void pop_front();

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args);
    template <class... Args>
    void emplace_back(Args&&... args);
    template <class... Args>
    void emplace_front(Args&&... args);

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

private:
    allocator_type alloc_;
    node* ptr_;
};

template <class T, class Alloc>
template <class E>
class list<T, Alloc>::iter
{
public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = std::bidirectional_iterator_tag;

    iter(node* ptr) : ptr_(ptr) {}
    iter(const iter& it) : ptr_(it.ptr_) {}
    iter& operator=(const iter& it) { ptr_ = it.ptr_; }
    E& operator*() const { return ptr_->t; }
    E* operator->() const { return &(ptr_->t); }
    bool operator==(const iter& it) { return ptr_ == it.ptr_; }
    bool operator!=(const iter& it) { return ptr_ != it.ptr_; }
    iter& operator++()
    {
        ptr_ = ptr_->nxt;
        return *this;
    }
    iter operator++(int)
    {
        auto res = iter(ptr_);
        ptr_ = ptr_->nxt;
        return res;
    }
    iter& operator--()
    {
        ptr_ = ptr_->pre;
        return *this;
    }
    iter operator--(int)
    {
        auto res = iter(ptr_);
        ptr_ = ptr_->pre;
        return res;
    }

private:
    node* ptr_;
};

// ctor & dtor
// default
template <class T, class Alloc>
list<T, Alloc>::list() : list(allocator_type())
{
}
template <class T, class Alloc>
list<T, Alloc>::list(const allocator_type& alloc)
    : ptr_(new node()), alloc_(alloc)
{
}
// fill
template <class T, class Alloc>
list<T, Alloc>::list(size_type n, const allocator_type& alloc)
    : list(n, T(), alloc)
{
}
template <class T, class Alloc>
list<T, Alloc>::list(size_type n, const_reference val,
                     const allocator_type& alloc)
    : list(alloc)
{
    for (size_t i = 0; i < n; ++i) push_back(val);
}
// range
template <class T, class Alloc>
template <class InputIterator>
list<T, Alloc>::list(
    InputIterator first, InputIterator last, const allocator_type& alloc,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
    : list(alloc)
{
    for (; first != last; ++first) push_back(*first);
}
// copy
template <class T, class Alloc>
list<T, Alloc>::list(const list& x) : list(x, allocator_type())
{
}
template <class T, class Alloc>
list<T, Alloc>::list(const list& x, const allocator_type& alloc)
    : list(x.begin(), x.end(), alloc)
{
}
// move
template <class T, class Alloc>
list<T, Alloc>::list(list&& x) : list(move(x), allocator_type())
{
}
template <class T, class Alloc>
list<T, Alloc>::list(list&& x, const allocator_type& alloc)
    : ptr_(x.ptr), alloc_(alloc)
{
    x.ptr = new node();
}
// list
template <class T, class Alloc>
list<T, Alloc>::list(std::initializer_list<value_type> il,
                     const allocator_type& alloc)
    : list(il.begin(), il.end(), alloc)
{
}
// dtor
template <class T, class Alloc>
list<T, Alloc>::~list() noexcept
{
    auto ptr = ptr_;
    do
    {
        auto tmp = ptr;
        ptr = ptr->nxt;
        delete tmp;
    } while (ptr != ptr_);
}
// =
template <class T, class Alloc>
list<T, Alloc>&
list<T, Alloc>::operator=(const list& x)
{
    list(x).swap(*this);
}

// set
template <class T, class Alloc>
void
list<T, Alloc>::push_back(const T& val)
{
    auto tail = new node(val);
    tail->pre = ptr_->pre;
    tail->nxt = ptr_;
    ptr_->pre->nxt = tail;
    ptr_->pre = tail;
}

template <class T, class Alloc>
void
list<T, Alloc>::push_back(T&& val)
{
    auto tail = new node(std::move(val));
    tail->pre = ptr_->pre;
    tail->nxt = ptr_;
    ptr_->pre->nxt = tail;
    ptr_->pre = tail;
}

template <class T, class Alloc>
void
list<T, Alloc>::pop_back()
{
    auto old_tail = ptr_->pre;
    auto new_tail = old_tail->pre;
    new_tail->nxt = ptr_;
    ptr_->pre = new_tail;
    delete old_tail;
}

// mem
template <class T, class Alloc>
typename list<T, Alloc>::allocator_type
list<T, Alloc>::get_allocator() const noexcept
{
    return alloc_;
}
template <class T, class Alloc>
size_t
list<T, Alloc>::size() const noexcept
{
    return std::distance(begin(), end());
}
template <class T, class Alloc>
bool
list<T, Alloc>::empty() const noexcept
{
    return size() == 0;
}

// get
template <class T, class Alloc>
T&
list<T, Alloc>::front()
{
    return *(this->begin());
}
template <class T, class Alloc>
const T&
list<T, Alloc>::front() const
{
    return *(this->begin());
}
template <class T, class Alloc>
T&
list<T, Alloc>::back()
{
    return *(this->end());
}
template <class T, class Alloc>
const T&
list<T, Alloc>::back() const
{
    return *(this->end());
}

// iterate
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::begin() noexcept
{
    return iterator(ptr_->nxt);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::end() noexcept
{
    return iterator(ptr_);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::begin() const noexcept
{
    return const_iterator(ptr_->nxt);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::end() const noexcept
{
    return const_iterator(ptr_);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::cbegin() const noexcept
{
    return const_iterator(ptr_->nxt);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::cend() const noexcept
{
    return const_iterator(ptr_);
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator
list<T, Alloc>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator
list<T, Alloc>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator
list<T, Alloc>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator
list<T, Alloc>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator
list<T, Alloc>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator
list<T, Alloc>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}
}
