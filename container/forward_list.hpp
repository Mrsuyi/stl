#pragma once

#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"

namespace mrsuyi
{
template <class T, class Alloc = allocator<T>>
class forward_list
{
    struct node
    {
        node* nxt;
        T t;

        template <class... Args>
        node(Args&&... args) : nxt(this), t(mrsuyi::forward<Args>(args)...)
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

    // member-function
public:
    // ctor & dtor
    // default
    forward_list();
    explicit forward_list(const allocator_type& alloc);
    // fill
    explicit forward_list(size_t n,
                          const allocator_type& alloc = allocator_type());
    forward_list(size_t n, const T& val,
                 const allocator_type& alloc = allocator_type());
    // range
    template <class InputIterator>
    forward_list(InputIterator first, InputIterator last,
                 const allocator_type& alloc = allocator_type(),
                 typename mrsuyi::enable_if<
                     !mrsuyi::is_integral<InputIterator>::value>::type* = 0);
    // copy
    forward_list(const forward_list& x);
    forward_list(const forward_list& x, const allocator_type& alloc);
    // move
    forward_list(forward_list&& x);
    forward_list(forward_list&& x, const allocator_type& alloc);
    // forward_list
    forward_list(std::initializer_list<T> il,
                 const allocator_type& alloc = allocator_type());
    // dtor
    ~forward_list() noexcept;
    // =
    forward_list& operator=(const forward_list& x);
    forward_list& operator=(forward_list&& x);
    forward_list& operator=(std::initializer_list<T> il);
    // assign
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename mrsuyi::enable_if<
                    !mrsuyi::is_integral<InputIterator>::value>::type* = 0);
    void assign(size_t n, const T& val);
    void assign(std::initializer_list<T> il);
    // alloc
    allocator_type get_allocator() const noexcept;

    // element access
    T& front();
    const T& front() const;

    // iterators
    iterator before_begin() noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator before_begin() const noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbefore_begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // capacity
    bool empty() const noexcept;
    size_t max_size() const noexcept;

    // modifiers
    void clear();
    void resize(size_t size);
    void resize(size_t size, const T& val);
    void swap(forward_list& x);

    void push_front(const T& val);
    void push_front(T&& val);

    void pop_front();

    template <class... Args>
    iterator emplace_after(const_iterator pos, Args&&... args);
    template <class... Args>
    void emplace_front(Args&&... args);

    iterator insert(const_iterator pos, const T& val);
    iterator insert(const_iterator pos, T&& val);
    void insert(const_iterator pos, size_t n, const T& val);
    template <class InputIterator>
    void insert(const_iterator pos, InputIterator first, InputIterator last,
                typename mrsuyi::enable_if<
                    !mrsuyi::is_integral<InputIterator>::value>::type* = 0);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    // operations
    void merge(forward_list&&);
    template <class Compare>
    void merge(forward_list&&, Compare);

    void splice(const_iterator pos, forward_list& other);
    void splice(const_iterator pos, forward_list&& other);
    void splice(const_iterator pos, forward_list& other, const_iterator it);
    void splice(const_iterator pos, forward_list&& other, const_iterator it);
    void splice(const_iterator pos, forward_list& other, const_iterator first,
                const_iterator last);
    void splice(const_iterator pos, forward_list&& other, const_iterator first,
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

protected:
    allocator_type alloc_;
    node* joint_;
};

template <class T, class Alloc>
template <class E>
class forward_list<T, Alloc>::iter
{
    friend class forward_list<T, Alloc>;

public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = bidirectional_iterator_tag;

    iter() : node_(nullptr) {}
    iter(node* ptr) : node_(ptr) {}
    iter(const iter& it) : node_(it.node_) {}
    iter& operator=(const iter& it) { node_ = it.node_; }
    E& operator*() const { return node_->t; }
    E* operator->() const { return &(node_->t); }
    bool operator==(const iter& it) { return node_ == it.node_; }
    bool operator!=(const iter& it) { return node_ != it.node_; }
    iter& operator++()
    {
        node_ = node_->nxt;
        return *this;
    }
    iter operator++(int)
    {
        auto res = *this;
        ++*this;
        return res;
    }
    iter& operator--()
    {
        node_ = node_->pre;
        return *this;
    }
    iter operator--(int)
    {
        auto res = *this;
        --*this;
        return res;
    }
    operator iter<const E>() const { return iter<const E>(node_); }
private:
    node* node_;
};

// new/delete function
template <class T, class Alloc>
template <class... Args>
typename forward_list<T, Alloc>::node*
forward_list<T, Alloc>::new_node(Args... args)
{
    node* ptr = alloc_.allocate(1);
    mrsuyi::construct(ptr, mrsuyi::forward<Args>(args)...);
    return ptr;
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::del_node(node* ptr)
{
    mrsuyi::destroy_at(ptr);
    alloc_.deallocate(ptr, 1);
}

// ctor & dtor
// default
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list() : forward_list(allocator_type())
{
}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(const allocator_type& alloc)
    : joint_(new_node()), alloc_(alloc)
{
}
// fill
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(size_t n, const allocator_type& alloc)
    : forward_list(n, T(), alloc)
{
}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(size_t n, const T& val,
                                     const allocator_type& alloc)
    : forward_list(alloc)
{
    for (size_t i = 0; i < n; ++i) push_back(val);
}
// range
template <class T, class Alloc>
template <class InputIterator>
forward_list<T, Alloc>::forward_list(
    InputIterator first, InputIterator last, const allocator_type& alloc,
    typename mrsuyi::enable_if<
        !mrsuyi::is_integral<InputIterator>::value>::type*)
    : forward_list(alloc)
{
    for (; first != last; ++first) push_back(*first);
}
// copy
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(const forward_list& x)
    : forward_list(x, allocator_type())
{
}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(const forward_list& x,
                                     const allocator_type& alloc)
    : forward_list(x.begin(), x.end(), alloc)
{
}
// move
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(forward_list&& x)
    : forward_list(mrsuyi::move(x), allocator_type())
{
}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(forward_list&& x,
                                     const allocator_type& alloc)
    : joint_(x.joint_), alloc_(alloc)
{
    x.joint_ = new_node();
    x.size_ = 0;
}
// forward_list
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(std::initializer_list<T> il,
                                     const allocator_type& alloc)
    : forward_list(il.begin(), il.end(), alloc)
{
}
// dtor
template <class T, class Alloc>
forward_list<T, Alloc>::~forward_list() noexcept
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
forward_list<T, Alloc>&
forward_list<T, Alloc>::operator=(const forward_list& x)
{
    forward_list(x).swap(*this);
}
template <class T, class Alloc>
forward_list<T, Alloc>&
forward_list<T, Alloc>::operator=(forward_list&& x)
{
    forward_list(mrsuyi::move(x)).swap(*this);
}
template <class T, class Alloc>
forward_list<T, Alloc>&
forward_list<T, Alloc>::operator=(std::initializer_list<T> il)
{
    forward_list(il).swap(*this);
}
// assign
template <class T, class Alloc>
template <class InputIterator>
void
forward_list<T, Alloc>::assign(
    InputIterator first, InputIterator last,
    typename mrsuyi::enable_if<
        !mrsuyi::is_integral<InputIterator>::value>::type*)
{
    forward_list(first, last).swap(*this);
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::assign(size_t n, const T& val)
{
    forward_list(n, val).swap(*this);
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::assign(std::initializer_list<T> il)
{
    forward_list(il).swap(*this);
}
// alloc
template <class T, class Alloc>
typename forward_list<T, Alloc>::allocator_type
forward_list<T, Alloc>::get_allocator() const noexcept
{
    return alloc_;
}

// element access
template <class T, class Alloc>
T&
forward_list<T, Alloc>::front()
{
    return *(this->begin());
}
template <class T, class Alloc>
const T&
forward_list<T, Alloc>::front() const
{
    return *(this->begin());
}

// iterators
// non-const before_begin/begin/end
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::before_begin() noexcept
{
    return iterator(joint_);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::begin() noexcept
{
    return iterator(joint_->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::end() noexcept
{
    return iterator(joint_);
}
// const before_begin/begin/end
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::before_begin() const noexcept
{
    return const_iterator(joint_);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::begin() const noexcept
{
    return const_iterator(joint_->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::end() const noexcept
{
    return const_iterator(joint_);
}
// cbefore_begin/cbegin/cend
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::cbefore_begin() const noexcept
{
    return const_iterator(joint_);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::cbegin() const noexcept
{
    return const_iterator(joint_->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::cend() const noexcept
{
    return const_iterator(joint_);
}

// capacity
template <class T, class Alloc>
bool
forward_list<T, Alloc>::empty() const noexcept
{
    return joint_->nxt == joint_;
}
template <class T, class Alloc>
size_t
forward_list<T, Alloc>::max_size() const noexcept
{
    return std::numeric_limits<size_t>::max();
}

// modifiers
template <class T, class Alloc>
void
forward_list<T, Alloc>::clear()
{
    resize(0);
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::resize(size_t size)
{
    resize(size, T());
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::resize(size_t size, const T& val)
{
/*    auto sz = mrsuyi::distance(begin(), end());*/
    //if (sz < size)
        //for (size_t i = sz; i < size; ++i) push_front(val);
    //else
        /*for (size_t i = size; i < sz; ++i) pop_front();*/
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::swap(forward_list& x)
{
    mrsuyi::swap(joint_, x.joint_);
    mrsuyi::swap(alloc_, x.alloc_);
}

// push/pop -- front/back
template <class T, class Alloc>
void
forward_list<T, Alloc>::push_front(const T& val)
{
    insert(joint_->nxt, new_node(val));
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::push_front(T&& val)
{
    insert(joint_->nxt, new_node(mrsuyi::move(val)));
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::pop_front()
{
    erase(joint_->nxt);
}

// emplace front/back
template <class T, class Alloc>
template <class... Args>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::emplace_after(const_iterator pos, Args&&... args)
{
    return iterator(
        insert(pos.node_, new_node(mrsuyi::forward<Args>(args)...)));
}
template <class T, class Alloc>
template <class... Args>
void
forward_list<T, Alloc>::emplace_front(Args&&... args)
{
    insert(joint_->nxt, new_node(mrsuyi::forward<Args>(args)...));
}

// insert/erase
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert(const_iterator pos, const T& val)
{
    return iterator(insert(pos.node_, new_node(val)));
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert(const_iterator pos, T&& val)
{
    return iterator(insert(pos.node_, new_node(mrsuyi::move(val))));
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::insert(const_iterator pos, size_t n, const T& val)
{
    for (size_t i = 0; i < n; ++i) insert(pos.node_, new_node(val));
}
template <class T, class Alloc>
template <class InputIterator>
void
forward_list<T, Alloc>::insert(
    const_iterator pos, InputIterator first, InputIterator last,
    typename mrsuyi::enable_if<
        !mrsuyi::is_integral<InputIterator>::value>::type*)
{
    for (; first != last; ++first) insert(pos.node_, new_node(*first));
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::erase(const_iterator pos)
{
    return iterator(erase(pos.node_));
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::erase(const_iterator first, const_iterator last)
{
    for (; first != last; first = erase(first))
        ;
}
// operations
// merge
template <class T, class Alloc>
void
forward_list<T, Alloc>::merge(forward_list&& other)
{
    merge(mrsuyi::move(other),
          [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class T, class Alloc>
template <class Compare>
void
forward_list<T, Alloc>::merge(forward_list&& other, Compare cmp)
{
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
forward_list<T, Alloc>::splice(const_iterator pos, forward_list& other)
{
    splice(pos, mrsuyi::move(other));
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::splice(const_iterator pos, forward_list&& other)
{
    if (other.size_ == 0) return;
    insert(pos.node_, other.joint_->nxt, other.joint_->pre, other.size_);
    other.joint_->pre = other.joint_;
    other.joint_->nxt = other.joint_;
    other.size_ = 0;
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::splice(const_iterator pos, forward_list& other,
                               const_iterator it)
{
    splice(pos, mrsuyi::move(other), it);
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::splice(const_iterator pos, forward_list&& other,
                               const_iterator it)
{
    insert(pos.node_, other.yield(it.node_));
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::splice(const_iterator pos, forward_list& other,
                               const_iterator first, const_iterator last)
{
    splice(pos, mrsuyi::move(other), first, last);
}
template <class T, class Alloc>
void
forward_list<T, Alloc>::splice(const_iterator pos, forward_list&& other,
                               const_iterator first, const_iterator last)
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
forward_list<T, Alloc>::remove(const T& val)
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
forward_list<T, Alloc>::remove_if(UnaryPredicate p)
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
forward_list<T, Alloc>::unique()
{
    unique([](const T& a, const T& b) { return a == b; });
}
template <class T, class Alloc>
template <class BinaryPredicate>
void
forward_list<T, Alloc>::unique(BinaryPredicate p)
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
forward_list<T, Alloc>::sort()
{
    sort([](const T& a, const T& b) { return a < b; });
}
template <class T, class Alloc>
template <class Compare>
void
forward_list<T, Alloc>::sort(Compare p)
{
/*    if (size_ <= 1) return;*/

    //auto it = begin();
    //for (size_t i = 0; i < size_ / 2; ++i, ++it)
        //;
    //forward_list tmp;
    //tmp.splice(tmp.begin(), *this, begin(), it);

    //tmp.sort(p);
    //sort();

    //merge(mrsuyi::move(tmp), p);
}

// non-member functions
template <class T, class Alloc>
void
swap(forward_list<T, Alloc>& lhs, forward_list<T, Alloc>& rhs)
{
    lhs.swap(rhs);
}
template <class T, class Alloc>
bool
operator==(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs)
{
    return lhs.size() == rhs.size() &&
           equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <class T, class Alloc>
bool
operator!=(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs)
{
    return !(lhs == rhs);
}
template <class T, class Alloc>
bool
operator<(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                   rhs.end());
}
template <class T, class Alloc>
bool
operator<=(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs)
{
    return !(rhs < lhs);
}
template <class T, class Alloc>
bool
operator>(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs)
{
    return rhs < lhs;
}
template <class T, class Alloc>
bool
operator>=(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs)
{
    return !(lhs < rhs);
}
}
