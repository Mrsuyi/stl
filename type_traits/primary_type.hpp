#pragma once

#include "helper_class.hpp"

namespace mrsuyi
{
// is-integral
template <class T>
struct is_integral : public integral_constant<bool, false>
{
};
template <>
struct is_integral<bool> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<char> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<char16_t> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<char32_t> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<wchar_t> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<short> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<int> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<long> : public integral_constant<bool, true>
{
};
template <>
struct is_integral<long long> : public integral_constant<bool, true>
{
};
}
