#pragma once

namespace mrsuyi
{
// enable_if
template <bool B, class T = void>
struct enable_if
{
};
template <class T>
struct enable_if<true, T>
{
    using type = T;
};
template <bool B, class T>
using enable_if_t = typename enable_if<B, T>::type;
}
