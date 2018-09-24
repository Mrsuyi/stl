#pragma once

#include <type_traits>

#include "utility.hpp"

namespace mrsuyi {
namespace {

template <bool... bs>
struct BoolPack;

template <bool... Args>
using AllTrue = std::is_same<BoolPack<true, Args...>, BoolPack<Args..., true>>;

template <class From, class To>
struct AllConvertible : std::is_convertible<From, To> {};

template <class... From, class... To>
struct AllConvertible<tuple<From...>, tuple<To...>>
    : AllTrue<std::is_convertible<From, To>::value...> {};

template <class T>
class MemFunc {};

template <class R, class T, class... Args>
class MemFunc<R (T::*)(Args...)> {
  using Func = R (T::*)(Args...);

 public:
  explicit MemFunc(Func& f) : func_(f) {}

  template <typename... CallArgs,
            typename = AllConvertible<tuple<Args...>, tuple<CallArgs...>>>
  R operator()(T& t, CallArgs&&... args) {
    return (t.*func_)(forward<Args...>(args)...);
  }

  template <typename... CallArgs,
            typename = AllConvertible<tuple<Args...>, tuple<CallArgs...>>>
  R operator()(T&& t, CallArgs&&... args) {
    return (move(t).*func_)(forward<Args...>(args)...);
  }

  template <typename... CallArgs,
            typename = AllConvertible<tuple<Args...>, tuple<CallArgs...>>>
  R operator()(T* t, CallArgs&&... args) {
    return (t->*func_)(forward<Args...>(args)...);
  }

  // TODO: more specialization.

 protected:
  Func func_;
};

}  // namespace

template <class M, class T>
MemFunc<M T::*> mem_fn(M T::*pm) {
  return MemFunc<M T::*>(pm);
}

}  // namespace mrsuyi
