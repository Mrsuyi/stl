#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <limits>
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
        node(T&& t) : pre(this), nxt(this), t(move(t)) {}
        template <class... Args>
        node(Args&&... args) : pre(this), nxt(this), t(forward<Args>(args)...)
        {
        }
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
    explicit list(size_t n, const allocator_type& alloc = allocator_type());
    list(size_t n, const T& val,
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
    list(std::initializer_list<T> il,
         const allocator_type& alloc = allocator_type());
    // dtor
    ~list() noexcept;
    // =
    list& operator=(const list& x);
    list& operator=(list&& x);
    list& operator=(std::initializer_list<T> il);
    // assign
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void assign(size_t n, const T& val);
    void assign(std::initializer_list<T> il);
    // alloc
    allocator_type get_allocator() const noexcept;

    // element access
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

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
    size_t size() const noexcept;
    bool empty() const noexcept;
    size_t max_size() const noexcept;

    // modifiers
    void clear();
    void resize(size_t size);
    void resize(size_t size, const T& val);
    void swap(list& x);

    void push_front(const T& val);
    void push_front(T&& val);
    void push_back(const T& val);
    void push_back(T&& val);

    void pop_back();
    void pop_front();

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    template <class... Args>
    void emplace_back(Args&&... args);
    template <class... Args>
    void emplace_front(Args&&... args);

    iterator insert(const_iterator pos, const T& val);
    iterator insert(const_iterator pos, T&& val);
    void insert(const_iterator pos, size_t n, const T& val);
    template <class InputIterator>
    void insert(const_iterator pos, InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    // operations
    void merge(list&&);
    template <class Compare>
    void merge(list&&, Compare);

    void splice(const_iterator pos, list&& other);
    void splice(const_iterator pos, list&& other, const_iterator it);
    void splice(const_iterator pos, list&& other, const_iterator first,
                const_iterator last);

private:
    template <class... Args>
    node* new_node(Args... args);
    void del_node(node*);

    node* insert(node* pos, node* ins);
    node* erase(node*);

private:
    allocator_type alloc_;
    node* ptr_;
};

template <class T, class Alloc>
template <class E>
class list<T, Alloc>::iter
{
    friend class list<T, Alloc>;

public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = bidirectional_iterator_tag;

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
    operator iter<const E>() const { return iter<const E>(ptr_); }
private:
    node* ptr_;
};

// list-private-functions
template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::node* list<T, Alloc>::new_node(Args... args)
{
    node* ptr = alloc_.allocate(1);
    alloc_.construct(ptr, forward<Args>(args)...);
    return ptr;
}
template <class T, class Alloc>
void
list<T, Alloc>::del_node(node* ptr)
{
    alloc_.destroy(ptr);
    alloc_.deallocate(ptr, 1);
}

template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::insert(node* pos, node* ins)
{
    ins->nxt = pos;
    ins->pre = pos->pre;
    pos->pre->nxt = ins;
    pos->pre = ins;
    return ins;
}
template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::erase(node* era)
{
    auto res = era->nxt;
    era->pre->nxt = era->nxt;
    era->nxt->pre = era->pre;
    del_node(era);
    return res;
}

// ctor & dtor
// default
template <class T, class Alloc>
list<T, Alloc>::list() : list(allocator_type())
{
}
template <class T, class Alloc>
list<T, Alloc>::list(const allocator_type& alloc)
    : ptr_(new_node()), alloc_(alloc)
{
}
// fill
template <class T, class Alloc>
list<T, Alloc>::list(size_t n, const allocator_type& alloc)
    : list(n, T(), alloc)
{
}
template <class T, class Alloc>
list<T, Alloc>::list(size_t n, const T& val, const allocator_type& alloc)
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
    : ptr_(x.ptr_), alloc_(alloc)
{
    x.ptr_ = new_node();
}
// list
template <class T, class Alloc>
list<T, Alloc>::list(std::initializer_list<T> il, const allocator_type& alloc)
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
        del_node(tmp);
    } while (ptr != ptr_);
}
// =
template <class T, class Alloc>
list<T, Alloc>&
list<T, Alloc>::operator=(const list& x)
{
    list(x).swap(*this);
}
template <class T, class Alloc>
list<T, Alloc>&
list<T, Alloc>::operator=(list&& x)
{
    list(move(x)).swap(*this);
}
template <class T, class Alloc>
list<T, Alloc>&
list<T, Alloc>::operator=(std::initializer_list<T> il)
{
    list(il).swap(*this);
}
// assign
template <class T, class Alloc>
template <class InputIterator>
void
list<T, Alloc>::assign(
    InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    list(first, last).swap(*this);
}
template <class T, class Alloc>
void
list<T, Alloc>::assign(size_t n, const T& val)
{
    list(n, val).swap(*this);
}
template <class T, class Alloc>
void
list<T, Alloc>::assign(std::initializer_list<T> il)
{
    list(il).swap(*this);
}
// alloc
template <class T, class Alloc>
typename list<T, Alloc>::allocator_type
list<T, Alloc>::get_allocator() const noexcept
{
    return alloc_;
}

// element access
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
    return *(--this->end());
}
template <class T, class Alloc>
const T&
list<T, Alloc>::back() const
{
    return *(--this->end());
}

// iterators
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

// capacity
template <class T, class Alloc>
size_t
list<T, Alloc>::size() const noexcept
{
    return distance(begin(), end());
}
template <class T, class Alloc>
bool
list<T, Alloc>::empty() const noexcept
{
    return size() == 0;
}
template <class T, class Alloc>
size_t
list<T, Alloc>::max_size() const noexcept
{
    return std::numeric_limits<size_t>::max();
}

// modifiers
template <class T, class Alloc>
void
list<T, Alloc>::clear()
{
    resize(0);
}
template <class T, class Alloc>
void
list<T, Alloc>::resize(size_t size)
{
    resize(size, T());
}
template <class T, class Alloc>
void
list<T, Alloc>::resize(size_t size, const T& val)
{
    size_t len = this->size();
    if (len < size)
        for (size_t i = len; i < size; ++i) push_back(val);
    else
        for (size_t i = size; i < len; ++i) pop_back();
}
template <class T, class Alloc>
void
list<T, Alloc>::swap(list& x)
{
    mrsuyi::swap(ptr_, x.ptr_);
    mrsuyi::swap(alloc_, x.alloc_);
}

// push/pop -- front/back
template <class T, class Alloc>
void
list<T, Alloc>::push_front(const T& val)
{
    insert(ptr_->nxt, new_node(val));
}
template <class T, class Alloc>
void
list<T, Alloc>::push_front(T&& val)
{
    insert(ptr_->nxt, new_node(move(val)));
}
template <class T, class Alloc>
void
list<T, Alloc>::push_back(const T& val)
{
    insert(ptr_, new_node(val));
}
template <class T, class Alloc>
void
list<T, Alloc>::push_back(T&& val)
{
    insert(ptr_, new_node(move(val)));
}
template <class T, class Alloc>
void
list<T, Alloc>::pop_front()
{
    erase(ptr_->nxt);
}
template <class T, class Alloc>
void
list<T, Alloc>::pop_back()
{
    erase(ptr_->pre);
}

// emplace front/back
template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::iterator
list<T, Alloc>::emplace(const_iterator pos, Args&&... args)
{
    return iterator(insert(pos.ptr_, new_node(forward<Args>(args)...)));
}
template <class T, class Alloc>
template <class... Args>
void
list<T, Alloc>::emplace_back(Args&&... args)
{
    insert(ptr_, new_node(forward<Args>(args)...));
}
template <class T, class Alloc>
template <class... Args>
void
list<T, Alloc>::emplace_front(Args&&... args)
{
    insert(ptr_->nxt, new_node(forward<Args>(args)...));
}

// insert/erase
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, const T& val)
{
    return iterator(insert(pos.ptr_, new_node(val)));
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, T&& val)
{
    return iterator(insert(pos.ptr_, new_node(move(val))));
}
template <class T, class Alloc>
void
list<T, Alloc>::insert(const_iterator pos, size_t n, const T& val)
{
    for (size_t i = 0; i < n; ++i) insert(pos.ptr_, new_node(val));
}
template <class T, class Alloc>
template <class InputIterator>
void
list<T, Alloc>::insert(
    const_iterator pos, InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    for (; first != last; ++first) insert(pos.ptr_, new_node(*first));
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator pos)
{
    return iterator(erase(pos.ptr_));
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    for (; first != last; first = erase(first))
        ;
}
// operations
template <class T, class Alloc>
void
list<T, Alloc>::merge(list&& other)
{
    merge(move(other), [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class T, class Alloc>
template <class Compare>
void
list<T, Alloc>::merge(list&& other, Compare cmp)
{
    node* tail = ptr_;
    node* pthis = ptr_->nxt;
    node* pthat = other.ptr_->nxt;
    while (pthis != ptr_ && pthat != other.ptr_)
    {
        if (cmp(pthis->t, pthat->t))
        {
            tail->nxt = pthis;
            pthis->pre = tail;
            tail = pthis;
            pthis = pthis->nxt;
        }
        else
        {
            tail->nxt = pthat;
            pthat->pre = tail;
            tail = pthat;
            pthat = pthat->nxt;
        }
    }
    if (pthat != other.ptr_)
    {
        tail->nxt = pthat;
        pthat->pre = tail;
        tail = other.ptr_->pre;
    }
    tail->nxt = ptr_;
    ptr_->pre = tail;
}

// non-member functions
template <class T, class Alloc>
void
swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs)
{
    lhs.swap(rhs);
}
template <class T, class Alloc>
bool
operator==(const list<T, Alloc>& lhs, const list<T>& rhs)
{
    return lhs.size() == rhs.size() &&
           equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <class T, class Alloc>
bool
operator!=(const list<T, Alloc>& lhs, const list<T>& rhs)
{
    return !(lhs == rhs);
}
template <class T, class Alloc>
bool
operator<(const list<T, Alloc>& lhs, const list<T>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                   rhs.end());
}
template <class T, class Alloc>
bool
operator<=(const list<T, Alloc>& lhs, const list<T>& rhs)
{
    return !(rhs < lhs);
}
template <class T, class Alloc>
bool
operator>(const list<T, Alloc>& lhs, const list<T>& rhs)
{
    return rhs < lhs;
}
template <class T, class Alloc>
bool
operator>=(const list<T, Alloc>& lhs, const list<T>& rhs)
{
    return !(lhs < rhs);
}
}
