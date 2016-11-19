#pragma once

#include "functional.hpp"
#include "iterator.hpp"

namespace mrsuyi
{
//================================== find ====================================//
template <class InputIt, class T>
InputIt
find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first)
        if (*first == value) return first;
    return last;
}
template <class InputIt, class UnaryPredicate>
InputIt
find_if(InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first)
        if (p(*first)) return first;
    return last;
}
template <class InputIt, class UnaryPredicate>
InputIt
find_if_not(InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first)
        if (!p(*first)) return first;
    return last;
}

//================================== count ===================================//
template <class InputIt, class T>
typename iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value)
{
    typename iterator_traits<InputIt>::difference_type res = 0;
    for (; first != last; ++first)
        if (*first == value) ++res;
    return res;
}
template <class InputIt, class UnaryPredicate>
typename iterator_traits<InputIt>::difference_type
count_if(InputIt first, InputIt last, UnaryPredicate p)
{
    typename iterator_traits<InputIt>::difference_type res = 0;
    for (; first != last; ++first)
        if (p(*first)) ++res;
    return res;
}

//================================= mismatch =================================//
template <class InputIt1, class InputIt2, class BinaryPredicate>
mrsuyi::pair<InputIt1, InputIt2>
mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
    while (first1 != last1 && p(*first1, *first2))
    {
        ++first1;
        ++first2;
    }
    return {first1, first2};
}
template <class InputIt1, class InputIt2>
mrsuyi::pair<InputIt1, InputIt2>
mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    return mismatch(
        first1, last1, first2,
        mrsuyi::equal_to<
            typename mrsuyi::iterator_traits<InputIt1>::value_type>());
}
}
