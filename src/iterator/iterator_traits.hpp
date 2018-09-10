#pragma once

#include "cstddef"
#include "iterator_category.hpp"

namespace mrsuyi {
template <class T>
class iterator_traits {
 public:
  using value_type = typename T::value_type;
  using difference_type = typename T::difference_type;
  using reference = typename T::reference;
  using pointer = typename T::pointer;
  using iterator_category = typename T::iterator_category;
};

template <class T>
class iterator_traits<T*> {
 public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using pointer = T*;
  using iterator_category = random_access_iterator_tag;
};

template <class T>
class iterator_traits<const T*> {
 public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = const T&;
  using pointer = const T*;
  using iterator_category = random_access_iterator_tag;
};

// this function is used for it-tag-dispatch
// i.e. template-function overload according to it-type-traits
template <class Iterator>
typename iterator_traits<Iterator>::iterator_category __iterator_category(
    const Iterator&) {
  return typename iterator_traits<Iterator>::iterator_category();
}
}  // namespace mrsuyi
