#pragma once

#include "functional.hpp"
#include "iterator.hpp"

namespace mrsuyi
{
// max
template <class T>
const T&
max(const T& a, const T& b)
{
    return max(a, b, less<T>());
}
template <class T, class Compare>
const T&
max(const T& a, const T& b, Compare cmp)
{
    return cmp(a, b) ? b : a;
}
// min
template <class T>
const T&
min(const T& a, const T& b)
{
    return min(a, b, less<T>());
}
template <class T, class Compare>
const T&
min(const T& a, const T& b, Compare cmp)
{
    return cmp(a, b) ? a : b;
}

// equal
template <class InputIterator1, class InputIterator2>
bool
equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (!(*first1 == *first2)) return false;
    }
    return true;
}

// lexicographical_compare (aa < aab,  a < b)
template <class InputIterator1, class InputIterator2>
bool
lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2)
{
    for (; first1 != last1; ++first1, ++first2)
    {
        if (first2 == last2 || *first2 < *first1) return false;
        if (*first1 < *first2) return true;
    }
    return first2 != last2;
}
}
