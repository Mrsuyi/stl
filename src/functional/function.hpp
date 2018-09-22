#pragma once

#include <type_traits>
#include "memory.hpp"
#include "utility.hpp"

namespace mrsuyi {

// callable
template <class R, class... Args>
class callable {
 public:
  virtual callable* copy() = 0;
  virtual R operator()(Args... args) const = 0;
  virtual ~callable(){};
};

template <class F, class R, class... Args>
class callable_impl : public callable<R, Args...> {
 public:
  typedef callable_impl<F, R, Args...> this_type;
  typedef callable<R, Args...> base_type;

  callable_impl(F f) : f_(move(f)) {}
  R operator()(Args... args) const override {
    return f_(forward<Args>(args)...);
  }
  base_type* copy() override { return new this_type(f_); }

 protected:
  F f_;
};

// function declaration
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
  R operator()(Args... args) const;
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
    : callable_(new callable_impl<F, R, Args...>(move(f))) {}

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
R function<R(Args...)>::operator()(Args... args) const {
  return callable_->operator()(forward<Args...>(args)...);
}

// check valid
template <class R, class... Args>
function<R(Args...)>::operator bool() const noexcept {
  return bool(callable_);
}

}  // namespace mrsuyi
