#pragma once

#include "vector.hpp"

namespace mrsuyi
{
template <class T, class Container = mrsuyi::vector<T>>
class stack
{
    template <class U, class C>
    friend bool operator==(const stack<U, C>& lhs, const stack<U, C>& rhs);
    template <class U, class C>
    friend bool operator!=(const stack<U, C>& lhs, const stack<U, C>& rhs);
    template <class U, class C>
    friend bool operator<(const stack<U, C>& lhs, const stack<U, C>& rhs);
    template <class U, class C>
    friend bool operator<=(const stack<U, C>& lhs, const stack<U, C>& rhs);
    template <class U, class C>
    friend bool operator>(const stack<U, C>& lhs, const stack<U, C>& rhs);
    template <class U, class C>
    friend bool operator>=(const stack<U, C>& lhs, const stack<U, C>& rhs);

public:
    using value_type = T;
    using container_type = Container;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    // ctor & dtor
    // default
    explicit stack(const Container&);
    explicit stack(Container&& = Container());
    // copy
    stack(const stack&);
    // move
    stack(stack&&);
    // from ainer & allocator
    template <class Alloc>
    explicit stack(
        const Alloc&,
        typename std::enable_if<
            !std::uses_allocator<Container, Alloc>::value>::type* = 0);
    template <class Alloc>
    stack(const Container&, const Alloc&,
          typename std::enable_if<
              !std::uses_allocator<Container, Alloc>::value>::type* = 0);
    template <class Alloc>
    stack(Container&&, const Alloc&,
          typename std::enable_if<
              !std::uses_allocator<Container, Alloc>::value>::type* = 0);
    // from stack &
    template <class Alloc>
    stack(const stack&, const Alloc&,
          typename std::enable_if<
              !std::uses_allocator<Container, Alloc>::value>::type* = 0);
    template <class Alloc>
    stack(stack&&, const Alloc&,
          typename std::enable_if<
              !std::uses_allocator<Container, Alloc>::value>::type* = 0);
    // dtor
    ~stack();
    // assginment
    stack& operator=(const stack&);
    stack& operator=(stack&&);

    // element access
    T& top();
    const T& top() const;

    // capacity
    bool empty() const;
    size_t size() const;

    // modifiers
    void push(const T& val);
    void push(T&& val);

    template <class... Args>
    void emplace_back(Args&&... args);

    void pop();

    void swap(stack& other);

protected:
    Container cont_;
};

// ctor & dtor
// default
template <class T, class Container>
stack<T, Container>::stack(const Container& cont) : cont_(cont)
{
}
template <class T, class Container>
stack<T, Container>::stack(Container&& cont) : cont_(move(cont))
{
}
// copy
template <class T, class Container>
stack<T, Container>::stack(const stack& other) : cont_(other.cont_)
{
}
// move
template <class T, class Container>
stack<T, Container>::stack(stack&& other) : cont_(move(other.cont_))
{
}
// from container & alloc
template <class T, class Container>
template <class Alloc>
stack<T, Container>::stack(
    const Alloc& alloc,
    typename std::enable_if<
        !std::uses_allocator<Container, Alloc>::value>::type*)
    : cont_(alloc)
{
}
template <class T, class Container>
template <class Alloc>
stack<T, Container>::stack(
    const Container& cont, const Alloc& alloc,
    typename std::enable_if<
        !std::uses_allocator<Container, Alloc>::value>::type*)
    : cont_(cont, alloc)
{
}
template <class T, class Container>
template <class Alloc>
stack<T, Container>::stack(
    Container&& cont, const Alloc& alloc,
    typename std::enable_if<
        !std::uses_allocator<Container, Alloc>::value>::type*)
    : cont_(move(cont), alloc)
{
}
// from stack & alloc
template <class T, class Container>
template <class Alloc>
stack<T, Container>::stack(
    const stack& other, const Alloc& alloc,
    typename std::enable_if<
        !std::uses_allocator<Container, Alloc>::value>::type*)
    : cont_(cont(other.cont_), alloc)
{
}
template <class T, class Container>
template <class Alloc>
stack<T, Container>::stack(
    stack&& other, const Alloc& alloc,
    typename std::enable_if<
        !std::uses_allocator<Container, Alloc>::value>::type*)
    : cont_(cont(move(other.cont_)), alloc)
{
}
template <class T, class Container>
stack<T, Container>::~stack()
{
}
// assignment
template <class T, class Container>
stack<T, Container>&
stack<T, Container>::operator=(const stack& other)
{
    stack(other).swap(*this);
}
template <class T, class Container>
stack<T, Container>&
stack<T, Container>::operator=(stack&& other)
{
    stack(move(other)).swap(*this);
}

// element access
template <class T, class Container>
T&
stack<T, Container>::top()
{
    return cont_.back();
}
template <class T, class Container>
const T&
stack<T, Container>::top() const
{
    return cont_.back();
}
template <class T, class Container>
size_t
stack<T, Container>::size() const
{
    return cont_.size();
}

// modifiers
template <class T, class Container>
void
stack<T, Container>::push(const T& val)
{
    cont_.push_back(val);
}
template <class T, class Container>
void
stack<T, Container>::push(T&& val)
{
    cont_.push_back(move(val));
}
template <class T, class Container>
template <class... Args>
void
stack<T, Container>::emplace_back(Args&&... args)
{
    cont_.emplace_back(forward<Args>(args)...);
}
template <class T, class Container>
void
stack<T, Container>::pop()
{
    cont_.pop_back();
}
template <class T, class Container>
void
stack<T, Container>::swap(stack& other)
{
    swap(cont_, other.cont_);
}

//=========================== non-member functions ===========================//
template <class T, class Container>
void
swap(stack<T, Container>& x, stack<T, Container>& y)
{
    x.swap(y);
}
template <class T, class Container>
bool
operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.cont_ == rhs.cont_;
}
template <class T, class Container>
bool
operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.cont_ != rhs.cont_;
}
template <class T, class Container>
bool
operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.cont_ < rhs.cont_;
}
template <class T, class Container>
bool
operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.cont_ <= rhs.cont_;
}
template <class T, class Container>
bool
operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.cont_ > rhs.cont_;
}
template <class T, class Container>
bool
operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs.cont_ >= rhs.cont_;
}
};
