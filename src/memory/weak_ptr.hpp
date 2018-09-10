#pragma once

#include "shared_ptr.hpp"

namespace mrsuyi
{
template <class T>
class ctrl;

template <class T>
class shared_ptr;

template <class T>
class weak_ptr
{
    template <class U>
    friend class shared_ptr;

public:
    using element_type = T;

    // ctor & dtor
    // default
    constexpr weak_ptr() noexcept;
    // copy
    weak_ptr(const weak_ptr&) noexcept;
    template <class U>
    weak_ptr(const weak_ptr<U>&) noexcept;
    // from shared_ptr
    template <class U>
    weak_ptr(const shared_ptr<U>&) noexcept;
    // move
    weak_ptr(weak_ptr&&) noexcept;
    template <class U>
    weak_ptr(weak_ptr<U>&&) noexcept;

    ~weak_ptr();

    // assignment
    // copy
    weak_ptr& operator=(const weak_ptr&) noexcept;
    template <class U>
    weak_ptr& operator=(const weak_ptr<U>&) noexcept;
    // from shared_ptr
    template <class U>
    weak_ptr& operator=(const shared_ptr<U>&) noexcept;
    // move
    weak_ptr& operator=(weak_ptr&&) noexcept;
    template <class U>
    weak_ptr& operator=(weak_ptr<U>&&) noexcept;

    void swap(weak_ptr&) noexcept;

    // get
    long int use_count() const noexcept;
    bool expired() const noexcept;

    // set
    void reset() noexcept;
    shared_ptr<element_type> lock() const noexcept;

    // cmp
    template <class U>
    bool owner_before(const weak_ptr<U>&) const;
    template <class U>
    bool owner_before(const shared_ptr<U>&) const;

private:
    ctrl<T>* ctrl_;
};

// ctor & dtor
// defeault
template <class T>
constexpr weak_ptr<T>::weak_ptr() noexcept : ctrl_(new ctrl<T>(nullptr, 0, 1))
{
}
// copy
template <class T>
weak_ptr<T>::weak_ptr(const weak_ptr& x) noexcept : ctrl_(x.ctrl_)
{
    ++(ctrl_->weak);
}
template <class T>
template <class U>
weak_ptr<T>::weak_ptr(const weak_ptr<U>& x) noexcept : ctrl_(x.ctrl_)
{
    ++(ctrl_->weak);
}
// from shared_ptr
template <class T>
template <class U>
weak_ptr<T>::weak_ptr(const shared_ptr<U>& x) noexcept : ctrl_(x.ctrl_)
{
    ++(ctrl_->weak);
}
// move
template <class T>
weak_ptr<T>::weak_ptr(weak_ptr&& x) noexcept : ctrl_(x.ctrl_)
{
    x.ctrl_ = new ctrl<T>(nullptr, 0, 1);
}
template <class T>
template <class U>
weak_ptr<T>::weak_ptr(weak_ptr<U>&& x) noexcept : ctrl_(x.ctrl_)
{
    x.ctrl_ = new ctrl<U>(nullptr, 0, 1);
}
// dtor
template <class T>
weak_ptr<T>::~weak_ptr()
{
    --(ctrl_->weak);
    if (ctrl_->shared == 0 && ctrl_->weak == 0) delete ctrl_;
}

// assignment
// copy
template <class T>
weak_ptr<T>&
weak_ptr<T>::operator=(const weak_ptr& x) noexcept
{
    weak_ptr(x).swap(*this);
}
template <class T>
template <class U>
weak_ptr<T>&
weak_ptr<T>::operator=(const weak_ptr<U>& x) noexcept
{
    weak_ptr(x).swap(*this);
}
// from shared_ptr
template <class T>
template <class U>
weak_ptr<T>&
weak_ptr<T>::operator=(const shared_ptr<U>& x) noexcept
{
    weak_ptr(x).swap(*this);
}
// move
template <class T>
weak_ptr<T>&
weak_ptr<T>::operator=(weak_ptr&& x) noexcept
{
    weak_ptr(x).swap(*this);
}
template <class T>
template <class U>
weak_ptr<T>&
weak_ptr<T>::operator=(weak_ptr<U>&& x) noexcept
{
    weak_ptr(x).swap(*this);
}

// get
template <class T>
long int
weak_ptr<T>::use_count() const noexcept
{
    return ctrl_->shared;
}

template <class T>
bool
weak_ptr<T>::expired() const noexcept
{
    return ctrl_->shared == 0;
}

// set
template <class T>
void
weak_ptr<T>::reset() noexcept
{
    --(ctrl_->weak);
    if (ctrl_->shared == 0 && ctrl_->weak == 0) delete ctrl_;
}
template <class T>
shared_ptr<typename weak_ptr<T>::element_type>
weak_ptr<T>::lock() const noexcept
{
    return shared_ptr<element_type>(*this);
}

// cmp
template <class T>
template <class U>
bool
weak_ptr<T>::owner_before(const weak_ptr<U>& x) const
{
    return ctrl_->managed < x.ctrl_->managed;
}

template <class T>
template <class U>
bool
weak_ptr<T>::owner_before(const shared_ptr<U>& x) const
{
    return ctrl_->managed < x.ctrl_->managed;
}

}
