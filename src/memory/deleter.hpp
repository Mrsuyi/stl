#pragma once

#include <type_traits>

namespace mrsuyi {
//============================== general deleter =============================//
template <class T>
class default_delete {
 public:
  // cons & des
  constexpr default_delete() noexcept = default;
  template <
      class U,
      class = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
  default_delete(const default_delete<U>&) noexcept;

  // delete a ptr
  void operator()(T* ptr) const;
};

template <class T>
template <class U, class V>
default_delete<T>::default_delete(const default_delete<U>&) noexcept {}

template <class T>
void default_delete<T>::operator()(T* ptr) const {
  delete ptr;
}

//=============================== array deleter ==============================//
template <class T>
class default_delete<T[]> {
 public:
  // cons & des
  constexpr default_delete() noexcept = default;
  template <class U,
            class = typename std::enable_if<
                std::is_convertible<U* [], T*>::value>::type>
  default_delete(const default_delete<U[]>&) noexcept;

  // delete a ptr
  void operator()(T* ptr) const;
};

template <class T>
template <class U, class V>
default_delete<T[]>::default_delete(const default_delete<U[]>&) noexcept {}

template <class T>
void default_delete<T[]>::operator()(T* ptr) const {
  delete[] ptr;
}
}  // namespace mrsuyi
