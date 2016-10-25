#pragma once

#include <type_traits>

namespace mrsuyi
{
//=================================== move ===================================//
template <class T>
typename std::remove_reference<T>::type&&
move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<decltype(arg)>::type&&>(
        arg);
}

//================================== forward =================================//
template <class T>
constexpr T&&
forward(typename std::remove_reference<T>::type&& t)
{
    return static_cast<T&&>(t);
}
template <class T>
constexpr T&&
forward(typename std::remove_reference<T>::type& t)
{
    return static_cast<T&&>(t);
}

//=================================== pair ===================================//
template <class T1, class T2>
struct pair
{
    using first_type = T1;
    using second_type = T2;

    T1 first;
    T2 second;

    // cons & des
    // default
    constexpr pair();
    // copy & move
    template <class U, class V>
    pair(const pair<U, V>&);
    template <class U, class V>
    pair(pair<U, V>&&);
    pair(const pair&) = default;
    pair(pair&&) = default;
    // initialization
    pair(const T1&, const T2&);
    template <class U, class V>
    pair(U&&, V&&);

    // = assign
    pair& operator=(const pair&);
    template <class U, class V>
    pair& operator=(const pair<U, V>&);
    pair& operator=(pair&&);
    template <class U, class V>
    pair& operator=(pair<U, V>&&);

    void swap(pair&);
};

template <class T1, class T2>
pair<T1, T2>&
pair<T1, T2>::operator=(const pair& pr)
{
    first = pr.first;
    second = pr.second;
    return *this;
}

template <class T1, class T2>
pair<T1, T2>&
pair<T1, T2>::operator=(pair&& pr)
{
    first = move(pr.first);
    second = move(pr.second);
    return *this;
}

template <class T1, class T2>
void
pair<T1, T2>::swap(pair& pr)
{
    swap(first, pr.first);
    swap(second, pr.second);
}

template <class T1, class T2>
void
swap(pair<T1, T2>& pr1, pair<T1, T2>& pr2)
{
    pr1.swap(pr2);
}
template <class T1, class T2>
bool
operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class T1, class T2>
bool
operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(lhs == rhs);
}

template <class T1, class T2>
bool
operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return lhs.first < rhs.first ||
           (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template <class T1, class T2>
bool
operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(rhs < lhs);
}

template <class T1, class T2>
bool
operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return rhs < lhs;
}

template <class T1, class T2>
bool
operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(lhs < rhs);
}
}
