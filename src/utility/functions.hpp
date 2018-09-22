#pragma once

#include <cstddef>
#include <type_traits>

namespace mrsuyi {
template <class T>
typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
  return static_cast<typename std::remove_reference<decltype(arg)>::type&&>(
      arg);
}

template <class T>
constexpr T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
  return static_cast<T&&>(t);
}
template <class T>
constexpr T&& forward(typename std::remove_reference<T>::type& t) noexcept {
  return static_cast<T&&>(t);
}

template <class T>
void swap(T& a, T& b) {
  T c(move(a));
  a = move(b);
  b = move(c);
}

template <class T, size_t N>
void swap(T (&a)[N], T (&b)[N]) {
  for (size_t i = 0; i < N; ++i)
    swap(a[i], b[i]);
}
}  // namespace mrsuyi
