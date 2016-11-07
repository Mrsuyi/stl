#pragma once

#include <cstddef>
#include "type_traits.hpp"

namespace mrsuyi
{
//=================================== move ===================================//
template <class T>
typename remove_reference<T>::type&&
move(T&& arg) noexcept
{
    return static_cast<typename remove_reference<decltype(arg)>::type&&>(arg);
}

//================================== forward =================================//
template <class T>
constexpr T&&
forward(typename remove_reference<T>::type&& t)
{
    return static_cast<T&&>(t);
}
template <class T>
constexpr T&&
forward(typename remove_reference<T>::type& t)
{
    return static_cast<T&&>(t);
}

//=================================== swap ===================================//
template <class T>
void
swap(T& a, T& b)
{
    T c(move(a));
    a = move(b);
    b = move(c);
}

template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N])
{
    for (size_t i = 0; i < N; ++i) swap(a[i], b[i]);
}
}
