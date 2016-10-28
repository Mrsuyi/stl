#pragma once

#include <cstddef>
#include <iterator>

namespace mrsuyi
{
template <class Iterator>
class reverse_iterator
{
    // typedefs
public:
    using iterator_type = Iterator;
    using traits = std::iterator_traits<Iterator>;

    using iterator_category = typename traits::iterator_category;
    using value_type = typename traits::value_type;
    using difference_type = typename traits::difference_type;
    using pointer = typename traits::pointer;
    using reference = typename traits::reference;

    // member-function
public:
    // cons & des
    // default
    reverse_iterator();
    // initialization
    explicit reverse_iterator(iterator_type it);
    // copy
    template <class Iter>
    reverse_iterator(const reverse_iterator<Iter>& rev_it);

    iterator_type base() const;

    // operators
    reference operator*() const;
    pointer operator->() const;
    reference operator[](difference_type) const;

    reverse_iterator operator+(difference_type);
    reverse_iterator operator-(difference_type);
    reverse_iterator& operator++();
    reverse_iterator operator++(int);
    reverse_iterator& operator--();
    reverse_iterator operator--(int);
    reverse_iterator& operator-=(difference_type);
    reverse_iterator& operator+=(difference_type);

    // member-variable
private:
    iterator_type it_;
};

template <class Iterator>
reverse_iterator<Iterator>::reverse_iterator()
{
}

template <class Iterator>
reverse_iterator<Iterator>::reverse_iterator(iterator_type it) : it_(it)
{
}

template <class Iterator>
template <class Iter>
reverse_iterator<Iterator>::reverse_iterator(
    const reverse_iterator<Iter>& rev_it)
    : it_(rev_it.base())
{
}

template <class Iterator>
typename reverse_iterator<Iterator>::reference reverse_iterator<Iterator>::
operator*() const
{
    return *(it_ - 1);
}

template <class Iterator>
typename reverse_iterator<Iterator>::pointer reverse_iterator<Iterator>::
operator->() const
{
    return &(operator*());
}

template <class Iterator>
typename reverse_iterator<Iterator>::reference reverse_iterator<Iterator>::
operator[](difference_type diff) const
{
    return it_[-diff - 1];
}

template <class Iterator>
reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator++()
{
    --it_;
    return *this;
}

template <class Iterator>
reverse_iterator<Iterator> reverse_iterator<Iterator>::operator++(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

template <class Iterator>
reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator--()
{
    ++it_;
    return *this;
}

template <class Iterator>
reverse_iterator<Iterator> reverse_iterator<Iterator>::operator--(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

template <class Iterator>
reverse_iterator<Iterator>
reverse_iterator<Iterator>::operator+(difference_type diff)
{
    return reverse_iterator(it_ - diff);
}

template <class Iterator>
reverse_iterator<Iterator>
reverse_iterator<Iterator>::operator-(difference_type diff)
{
    return reverse_iterator(it_ + diff);
}

template <class Iterator>
reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator+=(difference_type diff)
{
    it_ -= diff;
    return *this;
}

template <class Iterator>
reverse_iterator<Iterator>&
reverse_iterator<Iterator>::operator-=(difference_type diff)
{
    it_ += diff;
    return *this;
}
}
