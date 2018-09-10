#pragma once

namespace mrsuyi
{
// ==
template <class T = void>
struct equal_to
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs == rhs;
    }
};
// !=
template <class T = void>
struct not_equal_to
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs != rhs;
    }
};
// <
template <class T = void>
struct less
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs < rhs;
    }
};
// <=
template <class T = void>
struct less_equal
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs <= rhs;
    }
};
// >
template <class T = void>
struct greater
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs > rhs;
    }
};
// >=
template <class T = void>
struct greater_equal
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs >= rhs;
    }
};
}
