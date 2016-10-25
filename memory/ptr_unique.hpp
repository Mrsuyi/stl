#pragma once

#include "../utility.hpp"
#include <type_traits>
#include "default_delete.hpp"

namespace mrsuyi
{
//========================== general unique-ptr ==============================//
template <class T, class D = default_delete<T>>
class unique_ptr
{
public:
    using element_type = T;
    using deleter_type = D;
    // using pointer = typename std::remove_reference<D>::type::pointer;
    using pointer = T*;

    // cons & des
    // default
    constexpr unique_ptr() noexcept;
    // from null pointer
    constexpr unique_ptr(std::nullptr_t) noexcept;
    // from pointer
    explicit unique_ptr(pointer p) noexcept;
    // from pointer & lvalue-deleter
    unique_ptr(
        pointer p,
        typename std::conditional<std::is_reference<D>::value, D, const D&>::type
            del) noexcept;
    // from pointer & rvalue-deleter
    unique_ptr(pointer p,
               typename std::remove_reference<D>::type&& del) noexcept;
    // move
    unique_ptr(unique_ptr&& x) noexcept;
    // move cast
    template <class U, class E>
    unique_ptr(unique_ptr<U, E>&& x) noexcept;
    // copy(deleted)
    unique_ptr(const unique_ptr&) = delete;

    ~unique_ptr();

    // = assignment
    // move
    unique_ptr& operator=(unique_ptr&& x) noexcept;
    // null
    unique_ptr& operator=(std::nullptr_t) noexcept;
    // type case move
    template <class U, class E>
    unique_ptr& operator=(unique_ptr<U, E>&& x) noexcept;
    // copy(deleted)
    unique_ptr& operator=(const unique_ptr&) = delete;

    // get
    pointer get() const noexcept;
    deleter_type& get_deleter() noexcept;
    const deleter_type& get_deleter() const noexcept;

    explicit operator bool() const noexcept;
    typename std::add_lvalue_reference<element_type>::type operator*() const;
    pointer operator->() const;

    // set
    pointer release() noexcept;
    void reset(pointer p = pointer()) noexcept;
    void swap(unique_ptr& x) noexcept;

private:
    T* ptr_;
    D del_;
};

template <class T, class D>
constexpr unique_ptr<T, D>::unique_ptr() noexcept : ptr_(nullptr), del_(D())
{
}

template <class T, class D>
constexpr unique_ptr<T, D>::unique_ptr(std::nullptr_t) noexcept : unique_ptr()
{
}

template <class T, class D>
unique_ptr<T, D>::unique_ptr(pointer p) noexcept : ptr_(p), del_(D())
{
}

template <class T, class D>
unique_ptr<T, D>::unique_ptr(unique_ptr&& x) noexcept : ptr_(x.release()),
                                                        del_(move(x.del_))
{
}

template <class T, class D>
unique_ptr<T, D>::unique_ptr(
    pointer p,
    typename std::conditional<std::is_reference<D>::value, D, const D&>::type
        del) noexcept : ptr_(p),
                        del_(del)
{
}

template <class T, class D>
unique_ptr<T, D>::unique_ptr(
    pointer p, typename std::remove_reference<D>::type&& del) noexcept
    : ptr_(p),
      del_(move(del))
{
}

template <class T, class D>
unique_ptr<T, D>::~unique_ptr()
{
    del_(ptr_);
}

template <class T, class D>
unique_ptr<T, D>&
unique_ptr<T, D>::operator=(unique_ptr&& x) noexcept
{
    reset(x.release());
    del_ = move(x.del_);
    return *this;
}

template <class T, class D>
unique_ptr<T, D>& unique_ptr<T, D>::operator=(std::nullptr_t) noexcept
{
    reset();
    return *this;
}

template <class T, class D>
typename unique_ptr<T, D>::pointer
unique_ptr<T, D>::get() const noexcept
{
    return ptr_;
}

template <class T, class D>
typename unique_ptr<T, D>::deleter_type&
unique_ptr<T, D>::get_deleter() noexcept
{
    return del_;
}

template <class T, class D>
const typename unique_ptr<T, D>::deleter_type&
unique_ptr<T, D>::get_deleter() const noexcept
{
    return del_;
}

template <class T, class D>
unique_ptr<T, D>::operator bool() const noexcept
{
    return ptr_ != nullptr;
}

template <class T, class D>
typename std::add_lvalue_reference<
    typename unique_ptr<T, D>::element_type>::type unique_ptr<T, D>::operator*()
    const
{
    return *get();
}

template <class T, class D>
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::operator->() const
{
    return get();
}

template <class T, class D>
typename unique_ptr<T, D>::pointer
unique_ptr<T, D>::release() noexcept
{
    auto tmp = ptr_;
    ptr_ = nullptr;
    return tmp;
}

template <class T, class D>
void
unique_ptr<T, D>::reset(pointer p) noexcept
{
    del_(ptr_);
    ptr_ = p;
}

template <class T, class D>
void
unique_ptr<T, D>::swap(unique_ptr& x) noexcept
{
    std::swap(ptr_, x.ptr_);
    std::swap(del_, x.del_);
}

//============================== array unique-ptr ============================//
}
