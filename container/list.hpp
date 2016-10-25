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
template <class T, class Alloc = allocator<T>>
class list
{
    class node;

    template <class E>
    class iter;

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
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

    // member-function
public:
    // cons & des
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
    allocator_type alloc_;
    node* bgn_;
    node* end_;
};

//============ cons & des ============//
template <class T, class Alloc>
list<T, Alloc>::list() : list(allocator_type())
{
}

template <class T, class Alloc>
list<T, Alloc>::list(const allocator_type& alloc) : alloc_(alloc)
{
}

template <class T, class Alloc>
list<T, Alloc>::~list() noexcept
{
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::begin() noexcept
{
    return iterator(bgn_);
}

//==================================== Node ==================================//
template <class T, class Alloc>
class list<T, Alloc>::node
{
    node *pre, *nxt;
    T element;
};

//================================== Iterator ================================//
template <class T, class Alloc>
template <class E>
class list<T, Alloc>::iter
{
    friend class list<T, Alloc>;

public:
    iter(node*);
    iter(const iter&);

    iter& operator=(const iter&);

    E& operator*() const;
    E* operator->() const;

    bool operator==(const iter&);
    bool operator!=(const iter&);

    iter& operator++();
    iter operator++(int);
    iter& operator--();
    iter operator--(int);

private:
    node* ptr_;
};

template <class T, class Alloc>
template <class E>
list<T, Alloc>::iter<E>::iter(node* ptr) : ptr_(ptr)
{
}
}
