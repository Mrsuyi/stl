#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory/allocator.hpp"

namespace mrsuyi
{
template <class T, template <class> class Alloc = allocator>
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
    using allocator_type = Alloc<node>;
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
    // des
    ~list() noexcept;

    void swap(list& x);

    list& operator=(const list& x);
    list& operator=(list&& x);
    list& operator=(std::initializer_list<value_type> il);

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
    template <class... Args>
    void emplace_front(Args&&... args);

    // get
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

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
    allocator_type alloc_;
    node* ptr_;
};

template <class T, template <class> class Alloc>
template <class E>
class list<T, Alloc>::iter
{
public:
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
        auto res = ptr_;
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
        auto res = ptr_;
        ptr_ = ptr_->pre;
        return res;
    }

private:
    node* ptr_;
};

//============ cons & des ============//
template <class T, template <class> class Alloc>
list<T, Alloc>::list() : ptr_(new node()), alloc_(allocator_type())
{
}

template <class T, template <class> class Alloc>
list<T, Alloc>::list(const allocator_type& alloc) : alloc_(alloc)
{
}

template <class T, template <class> class Alloc>
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

// set
template <class T, template <class> class Alloc>
void
list<T, Alloc>::push_back(const T& val)
{
    auto tail = new node(val);
    tail->pre = ptr_->pre;
    tail->nxt = ptr_;
    ptr_->pre->nxt = tail;
    ptr_->pre = tail;
}

template <class T, template <class> class Alloc>
void
list<T, Alloc>::push_back(T&& val)
{
    auto tail = new node(std::move(val));
    tail->pre = ptr_->pre;
    tail->nxt = ptr_;
    ptr_->pre->nxt = tail;
    ptr_->pre = tail;
}

template <class T, template <class> class Alloc>
void
list<T, Alloc>::pop_back()
{
    auto old_tail = ptr_->pre;
    auto new_tail = old_tail->pre;
    new_tail->nxt = ptr_;
    ptr_->pre = new_tail;
    delete old_tail;
}

// iterate
template <class T, template <class> class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::begin() noexcept
{
    return iterator(ptr_->nxt);
}

template <class T, template <class> class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::end() noexcept
{
    return iterator(ptr_);
}

// get
template <class T, template <class> class Alloc>
T&
list<T, Alloc>::front()
{
    return *(this->begin());
}
template <class T, template <class> class Alloc>
const T&
list<T, Alloc>::front() const
{
    return *(this->begin());
}
template <class T, template <class> class Alloc>
T&
list<T, Alloc>::back()
{
    return *(this->end());
}
template <class T, template <class> class Alloc>
const T&
list<T, Alloc>::back() const
{
    return *(this->end());
}

}
