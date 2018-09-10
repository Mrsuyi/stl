#pragma once

#include "algorithm.hpp"
#include "functional.hpp"
#include "vector.hpp"

namespace mrsuyi
{
template <class T, class Container = mrsuyi::vector<T>,
          class Compare = mrsuyi::less<typename Container::value_type>>
class priority_queue
{
public:
    using container_type = Container;
    using value_compare = Compare;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    // ctor & dtor
    // default
    priority_queue(const Compare&, const Container&);
    explicit priority_queue(const Compare& = Compare(),
                            Container&& = Container());
    // copy
    priority_queue(const priority_queue&);
    // mrsuyi::move
    priority_queue(priority_queue&&);
    // from alloc
    // template <class Alloc>
    // priority_queue(const Alloc&);
    // template <class Alloc>
    // priority_queue(const Compare&, const Alloc&);
    /*    template <class Alloc>*/
    // priority_queue(const Compare&, const Container&, const Alloc&);
    // template <class Alloc>
    // priority_queue(const Compare&, Container&&, const Alloc&);
    // template <class Alloc>
    // priority_queue(const priority_queue&, const Alloc&);
    // template <class Alloc>
    /*priority_queue(priority_queue&&, const Alloc&);*/
    // range
    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare&,
                   const Container&);
    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare& = Compare(),
                   Container&& = Container());
    // ~
    ~priority_queue();
    // =
    priority_queue& operator=(const priority_queue&);
    priority_queue& operator=(priority_queue&&);

    // element access
    const_reference top() const;

    // capacity
    bool empty() const;
    size_type size() const;

    // modifiers
    void push(const value_type&);
    void push(value_type&&);

    template <class... Args>
    void emplace(Args&&... args);

    void pop();

    void swap(priority_queue& other);

protected:
    Container cont_;
    Compare cmp_;
};

//===================================== basic ================================//
// ctor & dtor
// default
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(const Compare& cmp,
                                                      const Container& cont)
    : cmp_(cmp), cont_(cont)
{
    make_heap(cont_.begin(), cont_.end(), cmp);
}
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(const Compare& cmp,
                                                      Container&& cont)
    : cmp_(cmp), cont_(mrsuyi::move(cont))
{
    make_heap(cont_.begin(), cont_.end(), cmp);
}
// copy
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(const priority_queue& q)
    : cmp_(mrsuyi::move(q.cmp_)), cont_(q.cont_)
{
}
// mrsuyi::move
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(priority_queue&& q)
    : cmp_(mrsuyi::move(q.cmp_)), cont_(mrsuyi::move(q.cont_))
{
}
// from alloc
/*template <class T, class Container, class Compare>*/
// template <class Alloc>
// priority_queue<T, Container, Compare>::priority_queue(const Alloc& alloc)
//: cmp_(Compare()), cont_(alloc)
//{
//}
// template <class T, class Container, class Compare>
// template <class Alloc>
// priority_queue<T, Container, Compare>::priority_queue(const Compare& cmp,
// const Alloc& alloc)
//: cmp_(cmp), cont_(alloc)
//{
//}
// template <class T, class Container, class Compare>
// template <class Alloc>
// priority_queue<T, Container, Compare>::priority_queue(const Compare& cmp,
// const Container& cont,
// const Alloc& alloc)
//: cmp_(cmp), cont_(cont, alloc)
//{
// make_heap(cont_.begin(), cont_.end(), cmp);
//}
// template <class T, class Container, class Compare>
// template <class Alloc>
// priority_queue<T, Container, Compare>::priority_queue(const Compare& cmp,
// Container&& cont,
// const Alloc& alloc)
//: cmp_(cmp), cont_(mrsuyi::move(cont), alloc)
//{
// make_heap(cont_.begin(), cont_.end(), cmp);
//}
// template <class T, class Container, class Compare>
// template <class Alloc>
// priority_queue<T, Container, Compare>::priority_queue(
// const priority_queue& other, const Alloc& alloc)
//: cmp_(other.cmp_), cont_(other.cont_, alloc)
//{
//}
// template <class T, class Container, class Compare>
// template <class Alloc>
// priority_queue<T, Container, Compare>::priority_queue(priority_queue&& other,
// const Alloc& alloc)
//: cmp_(other.cmp_), cont_(mrsuyi::move(other.cont_), alloc)
//{
/*}*/
// range
template <class T, class Container, class Compare>
template <class InputIt>
priority_queue<T, Container, Compare>::priority_queue(InputIt first,
                                                      InputIt last,
                                                      const Compare& cmp,
                                                      const Container& cont)
    : cmp_(cmp), cont_(cont)
{
    cont_.insert(cont_.end(), first, last);
    make_heap(cont_.begin(), cont_.end(), cmp);
}
template <class T, class Container, class Compare>
template <class InputIt>
priority_queue<T, Container, Compare>::priority_queue(InputIt first,
                                                      InputIt last,
                                                      const Compare& cmp,
                                                      Container&& cont)
    : cmp_(cmp), cont_(mrsuyi::move(cont))
{
    cont_.insert(cont_.end(), first, last);
    make_heap(cont_.begin(), cont_.end(), cmp);
}
// dtor
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::~priority_queue()
{
}
// =
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>&
priority_queue<T, Container, Compare>::operator=(const priority_queue& other)
{
    priority_queue(other).swap(*this);
}
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>&
priority_queue<T, Container, Compare>::operator=(priority_queue&& other)
{
    priority_queue(mrsuyi::move(other)).swap(*this);
}

//================================ element access ============================//
template <class T, class Container, class Compare>
typename priority_queue<T, Container, Compare>::const_reference
priority_queue<T, Container, Compare>::top() const
{
    return cont_.front();
}

//================================ element access ============================//
template <class T, class Container, class Compare>
bool
priority_queue<T, Container, Compare>::empty() const
{
    return size() == 0;
}
template <class T, class Container, class Compare>
typename priority_queue<T, Container, Compare>::size_type
priority_queue<T, Container, Compare>::size() const
{
    return cont_.size();
}

//================================== modifiers ===============================//
// push
template <class T, class Container, class Compare>
void
priority_queue<T, Container, Compare>::push(const value_type& val)
{
    cont_.push_back(val);
    push_heap(cont_.begin(), cont_.end(), cmp_);
}
template <class T, class Container, class Compare>
void
priority_queue<T, Container, Compare>::push(value_type&& val)
{
    cont_.push_back(mrsuyi::move(val));
    push_heap(cont_.begin(), cont_.end(), cmp_);
}
// emplace
template <class T, class Container, class Compare>
template <class... Args>
void
priority_queue<T, Container, Compare>::emplace(Args&&... args)
{
    cont_.emplace_back(mrsuyi::forward<Args>(args)...);
    push_heap(cont_.begin(), cont_.end(), cmp_);
}
// pop
template <class T, class Container, class Compare>
void
priority_queue<T, Container, Compare>::pop()
{
    pop_heap(cont_.begin(), cont_.end(), cmp_);
    cont_.pop_back();
}
// swap
template <class T, class Container, class Compare>
void
priority_queue<T, Container, Compare>::swap(priority_queue& other)
{
    mrsuyi::swap(cont_, other.cont_);
    mrsuyi::swap(cmp_, other.cmp_);
}

//============================ non-member functions ==========================//
template <class T, class Container, class Compare>
void
swap(priority_queue<T, Container, Compare>& lhs,
     priority_queue<T, Container, Compare>& rhs)
{
    lhs.swap(rhs);
}
}
