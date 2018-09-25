#pragma once

#include <type_traits>

#include "utility.hpp"

namespace mrsuyi {
namespace {

template <bool... bs>
struct bool_pack;

template <bool... Args>
using all_true =
    std::is_same<bool_pack<true, Args...>, bool_pack<Args..., true>>;

template <class From, class To>
struct all_convertible : std::is_convertible<From, To> {};

template <class... From, class... To>
struct all_convertible<tuple<From...>, tuple<To...>>
    : all_true<std::is_convertible<From, To>::value...> {};

template <class T>
class mem_func {};

template <class R, class T, class... Args>
class mem_func<R (T::*)(Args...)> {
  using Func = R (T::*)(Args...);

 public:
  explicit mem_func(Func& f) : func_(f) {}

  template <typename... CallArgs,
            typename = all_convertible<tuple<Args...>, tuple<CallArgs...>>>
  R operator()(T& t, CallArgs&&... args) {
    return (t.*func_)(forward<Args...>(args)...);
  }

  template <typename... CallArgs,
            typename = all_convertible<tuple<Args...>, tuple<CallArgs...>>>
  R operator()(T&& t, CallArgs&&... args) {
    return (move(t).*func_)(forward<Args...>(args)...);
  }

  template <typename... CallArgs,
            typename = all_convertible<tuple<Args...>, tuple<CallArgs...>>>
  R operator()(T* t, CallArgs&&... args) {
    return (t->*func_)(forward<Args...>(args)...);
  }

  // TODO: more specialization.

 protected:
  Func func_;
};
}  // namespace
}  // namespace mrsuyi

namespace mrsuyi {

template <class M, class T>
mem_func<M T::*> mem_fn(M T::*pm) {
  return mem_func<M T::*>(pm);
}

}  // namespace mrsuyi
