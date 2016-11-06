#pragma once

namespace mrsuyi
{
template <class T, T t>
struct integral_constant
{
    using type = integral_constant<T, t>;
    using value_type = T;

    static constexpr T value = t;

    constexpr operator T() const noexcept { return value; }
    constexpr T operator()() const noexcept { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
}
