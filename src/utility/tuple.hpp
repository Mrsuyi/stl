#pragma once

#include <type_traits>
#include "functions.hpp"

namespace mrsuyi {

// tuple pre-declaration
template <class... Ts>
class tuple;

template <>
class tuple<>;

// tuple_element
template <std::size_t I, class T>
struct tuple_element;

template <std::size_t I, class Head, class... Tail>
struct tuple_element<I, tuple<Head, Tail...>>
    : tuple_element<I - 1, tuple<Tail...>> {};

template <class Head, class... Tail>
struct tuple_element<0, tuple<Head, Tail...>> {
  typedef Head type;
};

// tuple declaration
template <>
class tuple<> {
 public:
  void swap(const tuple<>&) noexcept {}
};

template <class T, class... Ts>
class tuple<T, Ts...> : public tuple<Ts...> {
  template <std::size_t I, class J, class... Js>
  friend struct __get_helper;

 public:
  typedef T value_type;
  typedef tuple<Ts...> base_type;
  typedef tuple<T, Ts...> this_type;

  tuple();
  tuple(const T&, Ts... args);
  tuple(T&&, Ts... args);
  tuple(const tuple<T, Ts...>&);
  tuple(tuple<T, Ts...>&&);

  this_type& operator=(const this_type& other);
  this_type& operator=(this_type&& other) noexcept;

  void swap(this_type& other) noexcept;

 protected:
  T value_;
};

// tuple definition
template <class T, class... Ts>
tuple<T, Ts...>::tuple() : base_type(), value_(T()) {}

template <class T, class... Ts>
tuple<T, Ts...>::tuple(const T& t, Ts... args)
    : base_type(forward<Ts>(args)...), value_(t) {}

template <class T, class... Ts>
tuple<T, Ts...>::tuple(T&& t, Ts... args)
    : base_type(forward<Ts>(args)...), value_(move(t)) {}

template <class T, class... Ts>
tuple<T, Ts...>::tuple(const this_type& another)
    : base_type(static_cast<const base_type&>(another)),
      value_(another.value_) {}

template <class T, class... Ts>
tuple<T, Ts...>::tuple(this_type&& another)
    : base_type(static_cast<base_type&&>(another)),
      value_(move(another.value_)) {}

// operator=
template <class T, class... Ts>
tuple<T, Ts...>& tuple<T, Ts...>::operator=(const this_type& other) {
  static_cast<base_type*>(this)->operator=(other);
  value_ = other.value_;
  return *this;
}
template <class T, class... Ts>
tuple<T, Ts...>& tuple<T, Ts...>::operator=(this_type&& other) noexcept {
  static_cast<base_type*>(this)->operator=(move(other));
  value_ = move(other.value_);
  return *this;
}
// swap
template <class T, class... Ts>
void tuple<T, Ts...>::swap(this_type& other) noexcept {
  static_cast<base_type*>(this)->swap(other);
  mrsuyi::swap(value_, other.value_);
}

// get
template <std::size_t I, class T, class... Ts>
struct __get_helper {
  typedef tuple<T, Ts...> tuple_type;
  static typename tuple_element<I, tuple_type>::type& get(
      tuple_type& t) noexcept {
    return __get_helper<I - 1, Ts...>::get(t);
  }
  static typename tuple_element<I, tuple_type>::type&& get(
      tuple_type&& t) noexcept {
    return __get_helper<I - 1, Ts...>::get(move(t));
  }
  static const typename tuple_element<I, tuple_type>::type& get(
      const tuple_type& t) noexcept {
    return __get_helper<I - 1, Ts...>::get(t);
  }
  static const typename tuple_element<I, tuple_type>::type&& get(
      const tuple_type&& t) noexcept {
    return __get_helper<I - 1, Ts...>::get(move(t));
  }
};
template <class T, class... Ts>
struct __get_helper<0, T, Ts...> {
  typedef tuple<T, Ts...> tuple_type;
  static typename tuple_element<0, tuple_type>::type& get(
      tuple_type& t) noexcept {
    return t.value_;
  }
  static typename tuple_element<0, tuple_type>::type&& get(
      tuple_type&& t) noexcept {
    return move(t.value_);
  }
  static const typename tuple_element<0, tuple_type>::type& get(
      const tuple_type& t) noexcept {
    return t.value_;
  }
  static const typename tuple_element<0, tuple_type>::type&& get(
      const tuple_type&& t) noexcept {
    return move(t.value_);
  }
};
template <std::size_t I, class T, class... Ts>
typename tuple_element<I, tuple<T, Ts...>>::type& get(
    tuple<T, Ts...>& t) noexcept {
  return __get_helper<I, T, Ts...>::get(t);
}
template <std::size_t I, class T, class... Ts>
typename tuple_element<I, tuple<T, Ts...>>::type&& get(
    tuple<T, Ts...>&& t) noexcept {
  return __get_helper<I, T, Ts...>::get(t);
}
template <std::size_t I, class T, class... Ts>
const typename tuple_element<I, tuple<T, Ts...>>::type& get(
    const tuple<T, Ts...>& t) noexcept {
  return __get_helper<I, T, Ts...>::get(t);
}
template <std::size_t I, class T, class... Ts>
const typename tuple_element<I, tuple<T, Ts...>>::type&& get(
    const tuple<T, Ts...>&& t) noexcept {
  return __get_helper<I, T, Ts...>::get(t);
}

}  // namespace mrsuyi
