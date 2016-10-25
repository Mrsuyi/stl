#pragma once

#include <type_traits>

namespace mrsuyi
{
template <class T>
typename std::remove_reference<T>::type&&
move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<decltype(arg)>::type&&>(arg);
}

template <class T>
constexpr T&& forward(typename std::remove_reference<T>&& t)
{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(typename std::remove_reference<T>& t)
{
    return static_cast<T&&>(t);
}

}
