#pragma once

#include <type_traits>
#include "mem_fn.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace mrsuyi {
namespace {
// Base type for type erasure.
template <class R, class... Args>
class callable {
 public:
  virtual callable* copy() = 0;
  virtual R operator()(Args&&... args) = 0;
  virtual ~callable(){};
};

// Impl type for type erasure.
template <class Signature, class Type>
class callable_impl;

// Specialization for function pointers.
template <class R, class... Args>
class callable_impl<R(Args...), R (*)(Args...)> : public callable<R, Args...> {
 public:
  using callable_t = R (*)(Args...);

  callable_impl(callable_t pointer) : pointer_(pointer) {}

  callable_impl* copy() override { return new callable_impl(pointer_); }
  R operator()(Args&&... args) override {
    return pointer_(forward<Args>(args)...);
  }

 protected:
  callable_t pointer_;
};

// Specialization for member function pointers.
template <class T, class M, class R, class... Args>
class callable_impl<R(Args...), M T::*> : public callable<R, Args...> {
 public:
  using callable_t = M T::*;

  callable_impl(callable_t pointer) : mem_pointer_(pointer) {}

  callable_impl* copy() override { return new callable_impl(mem_pointer_); }

  R operator()(Args&&... args) override { return Call(forward<Args>(args)...); }

  template <class Class, class... CallArgs>
  R Call(Class c, CallArgs&&... args) {
    return mem_fn(mem_pointer_)(c, forward<CallArgs>(args)...);
  }

 protected:
  callable_t mem_pointer_;
};

// Specialization for functors.
template <class F, class R, class... Args>
class callable_impl<R(Args...), F> : public callable<R, Args...> {
 public:
  using callable_t = F;

  callable_impl(callable_t pointer) : functor_(pointer) {}

  callable_impl* copy() override { return new callable_impl(functor_); }
  R operator()(Args&&... args) override {
    return functor_(forward<Args>(args)...);
  }

 protected:
  callable_t functor_;
};

}  // namespace
}  // namespace mrsuyi

namespace mrsuyi {

template <class>
class function;

template <class R, class... Args>
class function<R(Args...)> {
 public:
  typedef R result_type;
  typedef function<R(Args...)> this_type;

  // constructors
  function() noexcept;
  function(std::nullptr_t) noexcept;
  function(const function&) noexcept;
  function(function&&) noexcept;
  template <class F>
  function(F f);

  this_type& operator=(const this_type& other);
  this_type& operator=(this_type&& other);
  this_type& operator=(std::nullptr_t);
  template <class F>
  this_type& operator=(F&& f);

  void swap(function& other) noexcept;

  // invoke
  R operator()(Args&&... args) const;
  // check valid
  explicit operator bool() const noexcept;

 protected:
  unique_ptr<callable<R, Args...>> callable_;
};

// Constructor
template <class R, class... Args>
function<R(Args...)>::function() noexcept {}

template <class R, class... Args>
function<R(Args...)>::function(std::nullptr_t) noexcept {}

template <class R, class... Args>
function<R(Args...)>::function(const function& other) noexcept {
  if (other.callable_)
    callable_.reset(other.callable_->copy());
}

template <class R, class... Args>
function<R(Args...)>::function(function&& other) noexcept
    : callable_(other.callable_.release()) {}

template <class R, class... Args>
template <class F>
function<R(Args...)>::function(F f)
    : callable_(new callable_impl<R(Args...), F>(move(f))) {}

// operator=
template <class R, class... Args>
function<R(Args...)>& function<R(Args...)>::operator=(const this_type& other) {
  this_type(other).swap(*this);
  return *this;
}
template <class R, class... Args>
function<R(Args...)>& function<R(Args...)>::operator=(this_type&& other) {
  this_type(move(other)).swap(*this);
  return *this;
}
template <class R, class... Args>
function<R(Args...)>& function<R(Args...)>::operator=(std::nullptr_t) {
  this_type(nullptr).swap(*this);
  return *this;
}
template <class R, class... Args>
template <class F>
function<R(Args...)>& function<R(Args...)>::operator=(F&& f) {
  this_type(move(f)).swap(*this);
  return *this;
}

// swap
template <class R, class... Args>
void function<R(Args...)>::swap(this_type& other) noexcept {
  callable_.swap(other.callable_);
}

// invoke
template <class R, class... Args>
R function<R(Args...)>::operator()(Args&&... args) const {
  return (*callable_)(forward<Args>(args)...);
}

// check valid
template <class R, class... Args>
function<R(Args...)>::operator bool() const noexcept {
  return bool(callable_);
}

}  // namespace mrsuyi
