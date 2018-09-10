#pragma once

#include "iterator.hpp"

namespace mrsuyi {
//================================ miscellaneous =============================//
template <class T>
T* addressof(T& arg) {
  return reinterpret_cast<T*>(
      &const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
}

//================================ uninitialized =============================//
template <class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt result) {
  for (; first != last; ++result, ++first)
    new (static_cast<void*>(mrsuyi::addressof(*result)))
        typename iterator_traits<ForwardIt>::value_type(*first);
  return result;
}
template <class InputIt, class Size, class ForwardIt>
ForwardIt uninitialized_copy_n(InputIt first, Size n, ForwardIt result) {
  for (; n > 0; ++result, ++first, --n)
    new (static_cast<void*>(mrsuyi::addressof(*result)))
        typename iterator_traits<ForwardIt>::value_type(*first);
  return result;
}

//==================================== move ==================================//
template <class InputIt, class ForwardIt>
ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt result) {
  mrsuyi::uninitialized_copy(mrsuyi::make_move_iterator(first),
                             mrsuyi::make_move_iterator(last), result);
}
template <class InputIt, class Size, class ForwardIt>
ForwardIt uninitialized_move_n(InputIt first, Size n, ForwardIt result) {
  mrsuyi::uninitialized_copy_n(mrsuyi::make_move_iterator(first), n, result);
}

//==================================== fill ==================================//
template <class ForwardIt, class T>
void uninitialized_fill(ForwardIt first, ForwardIt last, const T& x) {
  for (; first != last; ++first)
    new (static_cast<void*>(mrsuyi::addressof(*first)))
        typename iterator_traits<ForwardIt>::value_type(x);
}

template <class ForwardIt, class Size, class T>
ForwardIt uninitialized_fill_n(ForwardIt first, Size n, const T& x) {
  for (; n--; ++first)
    new (static_cast<void*>(mrsuyi::addressof(*first)))
        typename iterator_traits<ForwardIt>::value_type(x);
  return first;
}

//================================ default-ctor ==============================//
template <class ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last) {
  for (; first != last; ++first)
    new (static_cast<void*>(mrsuyi::addressof(*first)))
        typename iterator_traits<ForwardIt>::value_type;
}

template <class ForwardIt, class Size>
void uninitialized_default_construct_n(ForwardIt first, Size n) {
  for (; n > 0; ++first, --n)
    new (static_cast<void*>(mrsuyi::addressof(*first)))
        typename iterator_traits<ForwardIt>::value_type;
}

//================================ value-ctor ==============================//
template <class ForwardIt>
void uninitialized_value_construct(ForwardIt first, ForwardIt last) {
  for (; first != last; ++first)
    new (static_cast<void*>(mrsuyi::addressof(*first)))
        typename iterator_traits<ForwardIt>::value_type();
}

template <class ForwardIt, class Size>
void uninitialized_value_construct_n(ForwardIt first, Size n) {
  for (; n > 0; ++first, --n)
    new (static_cast<void*>(mrsuyi::addressof(*first)))
        typename iterator_traits<ForwardIt>::value_type();
}

//==================================== ctor ==================================//
template <class T, class... Args>
void construct(T* t, Args... args) {
  new (static_cast<void*>(t)) T(mrsuyi::forward<Args>(args)...);
}

//==================================== dtor ==================================//
template <class T>
void destroy_at(T* t) {
  t->~T();
}
template <class ForwardIt>
void destroy(ForwardIt first, ForwardIt last) {
  for (; first != last; ++first)
    destroy_at(mrsuyi::addressof(*first));
}
template <class ForwardIt, class Size>
void destroy_n(ForwardIt first, Size n) {
  for (; n > 0; ++first, --n)
    destroy_at(mrsuyi::addressof(*first));
}
}  // namespace mrsuyi
