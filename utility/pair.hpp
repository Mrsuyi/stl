#pragma once

#include "functions.hpp"

namespace mrsuyi
{
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
    pair(const pair&) = default;
    pair(pair&&) = default;

    template <class U, class V>
    pair(const pair<U, V>& p) : first(p.first), second(p.second)
    {
    }
    template <class U, class V>
    pair(pair<U, V>&& p) : first(move(p.first)), second(move(p.second))
    {
    }

    // initialization
    pair(const T1&, const T2&);
    template <class U, class V>
    pair(U&& a, V&& b) : first(move(a)), second(move(b))
    {
    }

    // = assign
    pair& operator=(const pair&) = default;
    pair& operator=(pair&&) = default;

    template <class U, class V>
    pair& operator=(const pair<U, V>& p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }
    template <class U, class V>
    pair& operator=(pair<U, V>&& p)
    {
        first = p.first;
        second = p.second;
    }

    void swap(pair& p)
    {
        swap(first, p.first);
        swap(second, p.second);
    }
};

//=========================== non-member-functions ===========================//
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
