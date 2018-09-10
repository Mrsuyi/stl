#pragma once

#include "iterator_traits.hpp"
#include "utility.hpp"

namespace mrsuyi
{
template <class Iterator>
class move_iterator
{
public:
    using iterator_type = Iterator;
    using difference_type =
        typename mrsuyi::iterator_traits<Iterator>::difference_type;
    using pointer = Iterator;
    using value_type = typename mrsuyi::iterator_traits<Iterator>::value_type;
    using reference = value_type&&;
    using iterator_category =
        typename mrsuyi::iterator_traits<Iterator>::iterator_category;

    // cons & des
    // default
    move_iterator() {}
    // initialization
    explicit move_iterator(Iterator it) : it_(it) {}
    // copy
    template <class Iter>
    move_iterator(const move_iterator<Iter>& move_it) : it_(move_it.base())
    {
    }

    // assignment
    template <class Iter>
    move_iterator& operator=(const move_iterator<Iter>& move_it)
    {
        it_ = (move_it.base());
    }

    // get
    iterator_type base() const { return it_; }
    reference operator*() const { return mrsuyi::move(*it_); }
    pointer operator->() const { return it_; }
    reference operator[](difference_type n) const
    {
        return mrsuyi::move(it_[n]);
    }
    // set
    move_iterator operator+(difference_type n)
    {
        return move_iterator(it_ + n);
    }
    move_iterator operator-(difference_type n)
    {
        return move_iterator(it_ - n);
    }
    move_iterator& operator++()
    {
        ++it_;
        return *this;
    }
    move_iterator operator++(int)
    {
        auto res = *this;
        ++*this;
        return res;
    }
    move_iterator& operator--()
    {
        --it_;
        return *this;
    }
    move_iterator operator--(int)
    {
        auto res = *this;
        --*this;
        return res;
    }
    move_iterator& operator+=(difference_type n)
    {
        it_ += n;
        return *this;
    }
    move_iterator& operator-=(difference_type n)
    {
        it_ -= n;
        return *this;
    }

    // member-variable
private:
    iterator_type it_;
};

// non-member functions
// cmp
template <class Iterator1, class Iterator2>
bool
operator==(const move_iterator<Iterator1>& lhs,
           const move_iterator<Iterator2>& rhs)
{
    return lhs.base() == rhs.base();
}
template <class Iterator1, class Iterator2>
bool
operator!=(const move_iterator<Iterator1>& lhs,
           const move_iterator<Iterator2>& rhs)
{
    return lhs.base() != rhs.base();
}
template <class Iterator1, class Iterator2>
bool
operator<(const move_iterator<Iterator1>& lhs,
          const move_iterator<Iterator2>& rhs)
{
    return lhs.base() < rhs.base();
}
template <class Iterator1, class Iterator2>
bool
operator<=(const move_iterator<Iterator1>& lhs,
           const move_iterator<Iterator2>& rhs)
{
    return lhs.base() <= rhs.base();
}
template <class Iterator1, class Iterator2>
bool
operator>(const move_iterator<Iterator1>& lhs,
          const move_iterator<Iterator2>& rhs)
{
    return lhs.base() > rhs.base();
}
template <class Iterator1, class Iterator2>
bool
operator>=(const move_iterator<Iterator1>& lhs,
           const move_iterator<Iterator2>& rhs)
{
    return lhs.base() >= rhs.base();
}
// inc & dec
template <class Iterator>
move_iterator<Iterator>
operator+(typename move_iterator<Iterator>::difference_type n,
          const move_iterator<Iterator>& it)
{
    return move_iterator<Iterator>(it.base() - n);
}
template <class Iterator1, class Iterator2>
auto
operator-(const move_iterator<Iterator1>& lhs,
          const move_iterator<Iterator2>& rhs)
    -> decltype(lhs.base() - rhs.base())
{
    return lhs.base() - rhs.base();
}

// make_reverse_iterator
template <class Iterator>
move_iterator<Iterator>
make_move_iterator(Iterator it)
{
    return move_iterator<Iterator>(it);
}
}
