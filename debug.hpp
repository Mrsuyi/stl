#pragma once

#include <cstddef>
#include <iostream>

namespace mrsuyi
{
using std::cout;
using std::endl;

template <class Container, class T, size_t N>
bool
equal(const Container& cont, const T (&x)[N])
{
    if (cont.size() != N) return false;

    auto it = cont.begin();
    for (size_t i = 0; i < N; ++i, ++it)
        if (*it != x[i]) return false;

    return true;
}

template <class Container, class T, size_t N>
bool
requal(const Container& cont, const T (&x)[N])
{
    if (cont.size() != N) return false;

    auto it = cont.rbegin();
    for (size_t i = 0; i < N; ++i, ++it)
        if (*it != x[i]) return false;

    return true;
}

template <class T>
void
print(const T& cont)
{
    for (auto& ele : cont) std::cout << ele << " ";
    std::cout << "\n";
}
}
