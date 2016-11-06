#pragma once

#include "iterator.hpp"

namespace mrsuyi
{
template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result)
{
    for (; first != last; ++result, ++first)
        new (static_cast<void*>(&*result))
            typename iterator_traits<ForwardIterator>::value_type(*first);
    return result;
}

template <class InputIterator, class Size, class ForwardIterator>
ForwardIterator
uninitialized_copy_n(InputIterator first, Size n, ForwardIterator result)
{
    for (; n > 0; ++result, ++first, --n)
        new (static_cast<void*>(&*result))
            typename iterator_traits<ForwardIterator>::value_type(*first);
    return result;
}

template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_move(InputIterator first, InputIterator last,
                   ForwardIterator result)
{
    uninitialized_copy(make_move_iterator(first), make_move_iterator(last),
                       result);
}

template <class InputIterator, class Size, class ForwardIterator>
ForwardIterator
uninitialized_move_n(InputIterator first, Size n, ForwardIterator result)
{
    uninitialized_copy_n(make_move_iterator(first), n, result);
}

template <class ForwardIterator, class T>
void
uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
    for (; first != last; ++first)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIterator>::value_type(x);
}

template <class ForwardIterator, class Size, class T>
ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
    for (; n--; ++first)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIterator>::value_type(x);
    return first;
}
}
