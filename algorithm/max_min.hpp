#pragma once

#include "functional.hpp"
#include "iterator.hpp"

namespace mrsuyi
{
//================================== max-min =================================//
// max
template <class T, class Compare>
const T&
max(const T& a, const T& b, Compare cmp)
{
    return cmp(a, b) ? b : a;
}
template <class T>
const T&
max(const T& a, const T& b)
{
    return max(a, b, less<T>());
}
// min
template <class T, class Compare>
const T&
min(const T& a, const T& b, Compare cmp)
{
    return cmp(a, b) ? a : b;
}
template <class T>
const T&
min(const T& a, const T& b)
{
    return min(a, b, less<T>());
}
}
