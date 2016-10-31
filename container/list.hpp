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

    void splice(const_iterator pos, list& other);
    void splice(const_iterator pos, list&& other);
    void splice(const_iterator pos, list& other, const_iterator it);
    void splice(const_iterator pos, list&& other, const_iterator it);
    void splice(const_iterator pos, list& other, const_iterator first,
                const_iterator last);
    void splice(const_iterator pos, list&& other, const_iterator first,
                const_iterator last);

    void remove(const T& val);
    template <class UnaryPredicate>
    void remove_if(UnaryPredicate p);

    void reverse();

    void unique();
    template <class BinaryPredicate>
    void unique(BinaryPredicate p);

    void sort();
    template <class Compare>
    void sort(Compare cmp);

protected:
    // new/delete
    template <class... Args>
    node* new_node(Args... args);
    void del_node(node*);
    // insert [one | head-tail] into list and increase size_ by [cnt]
    node* insert(node* pos, node* one);
    node* insert(node* pos, node* head, node* tail, size_t cnt);
    // extract [one | head-tail] from list and decrease size_ by [cnt]
    node* yield(node* one);
    node* yield(node* head, node* tail, size_t cnt);
    // erase [one] from list, delete it, and return its' following node
    node* erase(node* one);

protected:
    allocator_type alloc_;
    node* joint_;
    size_t size_;
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

    iter(node* ptr) : node_(ptr) {}
    iter(const iter& it) : node_(it.node_) {}
    iter&
    operator=(const iter& it)
    {
        node_ = it.node_;
    }
    E& operator*() const { return node_->t; }
    E* operator->() const { return &(node_->t); }
    bool
    operator==(const iter& it)
    {
        return node_ == it.node_;
    }
    bool
    operator!=(const iter& it)
    {
        return node_ != it.node_;
    }
    iter& operator++()
    {
        node_ = node_->nxt;
        return *this;
    }
    iter operator++(int)
    {
        auto res = iter(node_);
        node_ = node_->nxt;
        return res;
    }
    iter& operator--()
    {
        node_ = node_->pre;
        return *this;
    }
    iter operator--(int)
    {
        auto res = iter(node_);
        node_ = node_->pre;
        return res;
    }
    operator iter<const E>() const { return iter<const E>(node_); }
private:
    node* node_;
};

// new/delete function
template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::node*
list<T, Alloc>::new_node(Args... args)
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
// node/size manipulations
template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::insert(node* pos, node* one)
{
    return insert(pos, one, one, 1);
}
template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::insert(node* pos, node* head, node* tail, size_t cnt)
{
    size_ += cnt;
    head->pre = pos->pre;
    pos->pre->nxt = head;
    tail->nxt = pos;
    pos->pre = tail;
    return head;
}
template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::yield(node* one)
{
    return yield(one, one, 1);
}
template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::yield(node* head, node* tail, size_t cnt)
{
    size_ -= cnt;
    head->pre->nxt = tail->nxt;
    tail->nxt->pre = head->pre;
    return head;
}
template <class T, class Alloc>
typename list<T, Alloc>::node*
list<T, Alloc>::erase(node* one)
{
    auto res = one->nxt;
    del_node(yield(one));
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
    : joint_(new_node()), alloc_(alloc), size_(0)
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
    size_ = n;
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
    : joint_(x.joint_), alloc_(alloc), size_(x.size_)
{
    x.joint_ = new_node();
    x.size_ = 0;
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
    node* it = joint_;
    do
    {
        node* tmp = it;
        it = it->nxt;
        del_node(tmp);
    } while (it != joint_);
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
    return iterator(joint_->nxt);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::end() noexcept
{
    return iterator(joint_);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::begin() const noexcept
{
    return const_iterator(joint_->nxt);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::end() const noexcept
{
    return const_iterator(joint_);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::cbegin() const noexcept
{
    return const_iterator(joint_->nxt);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator
list<T, Alloc>::cend() const noexcept
{
    return const_iterator(joint_);
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
    return size_;
}
template <class T, class Alloc>
bool
list<T, Alloc>::empty() const noexcept
{
    return size_ == 0;
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
    if (size_ < size)
        for (size_t i = size_; i < size; ++i) push_back(val);
    else
        for (size_t i = size; i < size_; ++i) pop_back();
}
template <class T, class Alloc>
void
list<T, Alloc>::swap(list& x)
{
    mrsuyi::swap(joint_, x.joint_);
    mrsuyi::swap(alloc_, x.alloc_);
    mrsuyi::swap(size_, x.size_);
}

// push/pop -- front/back
template <class T, class Alloc>
void
list<T, Alloc>::push_front(const T& val)
{
    insert(joint_->nxt, new_node(val));
}
template <class T, class Alloc>
void
list<T, Alloc>::push_front(T&& val)
{
    insert(joint_->nxt, new_node(move(val)));
}
template <class T, class Alloc>
void
list<T, Alloc>::push_back(const T& val)
{
    insert(joint_, new_node(val));
}
template <class T, class Alloc>
void
list<T, Alloc>::push_back(T&& val)
{
    insert(joint_, new_node(move(val)));
}
template <class T, class Alloc>
void
list<T, Alloc>::pop_front()
{
    erase(joint_->nxt);
}
template <class T, class Alloc>
void
list<T, Alloc>::pop_back()
{
    erase(joint_->pre);
}

// emplace front/back
template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::iterator
list<T, Alloc>::emplace(const_iterator pos, Args&&... args)
{
    return iterator(insert(pos.node_, new_node(forward<Args>(args)...)));
}
template <class T, class Alloc>
template <class... Args>
void
list<T, Alloc>::emplace_back(Args&&... args)
{
    insert(joint_, new_node(forward<Args>(args)...));
}
template <class T, class Alloc>
template <class... Args>
void
list<T, Alloc>::emplace_front(Args&&... args)
{
    insert(joint_->nxt, new_node(forward<Args>(args)...));
}

// insert/erase
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, const T& val)
{
    return iterator(insert(pos.node_, new_node(val)));
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, T&& val)
{
    return iterator(insert(pos.node_, new_node(move(val))));
}
template <class T, class Alloc>
void
list<T, Alloc>::insert(const_iterator pos, size_t n, const T& val)
{
    for (size_t i = 0; i < n; ++i) insert(pos.node_, new_node(val));
}
template <class T, class Alloc>
template <class InputIterator>
void
list<T, Alloc>::insert(
    const_iterator pos, InputIterator first, InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
{
    for (; first != last; ++first) insert(pos.node_, new_node(*first));
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator pos)
{
    return iterator(erase(pos.node_));
}
template <class T, class Alloc>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    for (; first != last; first = erase(first))
        ;
}
// operations
// merge
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
    size_ += other.size_;
    // use three pointer to merge
    node* tail = joint_;
    node* pthis = joint_->nxt;
    node* pthat = other.joint_->nxt;
    while (pthis != joint_ && pthat != other.joint_)
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
    // append rest of other to this
    if (pthat != other.joint_)
    {
        tail->nxt = pthat;
        pthat->pre = tail;
        tail = other.joint_->pre;
    }
    // close this's node-circle
    tail->nxt = pthis;
    pthis->pre = tail;
    // reset other
    other.joint_->pre = other.joint_;
    other.joint_->nxt = other.joint_;
    other.size_ = 0;
}
// splice
template <class T, class Alloc>
void
list<T, Alloc>::splice(const_iterator pos, list& other)
{
    splice(pos, move(other));
}
template <class T, class Alloc>
void
list<T, Alloc>::splice(const_iterator pos, list&& other)
{
    if (other.size_ == 0) return;
    insert(pos.node_, other.joint_->nxt, other.joint_->pre, other.size_);
    other.joint_->pre = other.joint_;
    other.joint_->nxt = other.joint_;
    other.size_ = 0;
}
template <class T, class Alloc>
void
list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator it)
{
    splice(pos, move(other), it);
}
template <class T, class Alloc>
void
list<T, Alloc>::splice(const_iterator pos, list&& other, const_iterator it)
{
    insert(pos.node_, other.yield(it.node_));
}
template <class T, class Alloc>
void
list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator first,
                       const_iterator last)
{
    splice(pos, move(other), first, last);
}
template <class T, class Alloc>
void
list<T, Alloc>::splice(const_iterator pos, list&& other, const_iterator first,
                       const_iterator last)
{
    size_t cnt = distance(first, last);
    node* head = first.node_;
    node* tail = last.node_->pre;
    other.yield(head, tail, cnt);
    insert(pos.node_, head, tail, cnt);
}
// remove
template <class T, class Alloc>
void
list<T, Alloc>::remove(const T& val)
{
    for (auto it = begin(); it != end();)
        if (*it == val)
            it = erase(it);
        else
            ++it;
}
template <class T, class Alloc>
template <class UnaryPredicate>
void
list<T, Alloc>::remove_if(UnaryPredicate p)
{
    for (auto it = begin(); it != end();)
        if (p(*it))
            it = erase(it);
        else
            ++it;
}
// unique
template <class T, class Alloc>
void
list<T, Alloc>::unique()
{
    unique([](const T& a, const T& b) { return a == b; });
}
template <class T, class Alloc>
template <class BinaryPredicate>
void
list<T, Alloc>::unique(BinaryPredicate p)
{
    for (auto it1 = begin(), it2 = ++begin(); it2 != end();)
        if (p(*it1, *it2))
        {
            it2 = erase(it2);
        }
        else
        {
            ++it1;
            ++it2;
        }
}

template <class T, class Alloc>
void
list<T, Alloc>::sort()
{
    sort([](const T& a, const T& b) { return a < b; });
}
template <class T, class Alloc>
template <class Compare>
void
list<T, Alloc>::sort(Compare p)
{
    if (size_ <= 1) return;
   
    auto it = begin();
    for (size_t i = 0; i < size_ / 2; ++i, ++it);
    list tmp;
    tmp.splice(tmp.begin(), *this, begin(), it);

    tmp.sort(p);
    sort();

    merge(move(tmp), p);
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
