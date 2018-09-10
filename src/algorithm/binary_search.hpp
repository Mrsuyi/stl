#pragma once

#include "functional.hpp"
#include "iterator.hpp"

namespace mrsuyi
{
// lower-bound
template <class ForwardIt, class T, class Compare>
ForwardIt
lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare cmp)
{
    ForwardIt it;
    typename mrsuyi::iterator_traits<ForwardIt>::difference_type step, dist;
    dist = mrsuyi::distance(first, last);

    while (dist > 0)
    {
        it = first;
        step = dist / 2;
        mrsuyi::advance(it, step);

        if (cmp(*it, value))
        {
            first = ++it;
            dist -= step + 1;
        }
        else
            dist = step;
    }
    return first;
}
template <class ForwardIt, class T>
ForwardIt
lower_bound(ForwardIt first, ForwardIt last, const T& value)
{
    mrsuyi::lower_bound(first, last, value, mrsuyi::less<T>());
}
// upper-bound
template <class ForwardIt, class T, class Compare>
ForwardIt
upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare cmp)
{
    ForwardIt it;
    typename mrsuyi::iterator_traits<ForwardIt>::difference_type step, dist;
    dist = mrsuyi::distance(first, last);

    while (dist > 0)
    {
        it = first;
        step = dist / 2;
        mrsuyi::advance(it, step);

        if (!cmp(value, *it))
        {
            first = ++it;
            dist -= step + 1;
        }
        else
            dist = step;
    }
    return first;
}
template <class ForwardIt, class T>
ForwardIt
upper_bound(ForwardIt first, ForwardIt last, const T& value)
{
    mrsuyi::upper_bound(first, last, value, mrsuyi::less<T>());
}
// binary-search
template <class ForwardIt, class T, class Compare>
ForwardIt
binary_search(ForwardIt first, ForwardIt last, const T& value, Compare cmp)
{
    first = lower_bound(first, last, value, cmp);
    return (first != last) && (!cmp(value, *first));
}
template <class ForwardIt, class T>
ForwardIt
binary_search(ForwardIt first, ForwardIt last, const T& value)
{
    mrsuyi::binary_search(first, last, value, mrsuyi::less<T>());
}
// equal-range
template <class ForwardIt, class T, class Compare>
mrsuyi::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first, ForwardIt last, const T& value, Compare cmp)
{
}
template <class ForwardIt, class T>
mrsuyi::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first, ForwardIt last, const T& value)
{
    mrsuyi::equal_range(first, last, value, mrsuyi::less<T>());
}
}
