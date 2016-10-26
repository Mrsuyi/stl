#pragma once

#include <cstddef>
#include "allocator.hpp"
#include "default_delete.hpp"
#include "unique_ptr.hpp"

namespace mrsuyi
{
template <class T>
class weak_ptr;
template <class T, class Del>
class unique_ptr;

template <class T>
class shared_ptr
{
    class ctrl;

public:
    using element_type = T;

    // ctor & dtor
    // default
    constexpr shared_ptr() noexcept;
    // from null pointer
    constexpr shared_ptr(std::nullptr_t);
    // from pointer
    template <class U>
    explicit shared_ptr(U*);
    // from deleter
    template <class U, class Del>
    shared_ptr(U*, Del);
    template <class Del>
    shared_ptr(std::nullptr_t, Del);
    // with deleter and allocator
    template <class U, class Del, class Alloc>
    shared_ptr(U* p, Del, Alloc);
    template <class Del, class Alloc>
    shared_ptr(std::nullptr_t, Del, Alloc);
    // copy
    shared_ptr(const shared_ptr&) noexcept;
    template <class U>
    shared_ptr(const shared_ptr<U>&) noexcept;
    // copy from wek
    template <class U>
    shared_ptr(const weak_ptr<U>&);
    // move
    shared_ptr(shared_ptr&&) noexcept;
    template <class U>
    shared_ptr(shared_ptr<U>&&) noexcept;
    // move from unique
    template <class U, class Del>
    shared_ptr(unique_ptr<U, Del>&&);
    // alias
    template <class U>
    shared_ptr(const shared_ptr<U>&, element_type*) noexcept;

    ~shared_ptr();

    // copy =
    shared_ptr& operator=(const shared_ptr&) noexcept;
    template <class U>
    shared_ptr& operator=(const shared_ptr<U>&) noexcept;
    // move =
    shared_ptr& operator=(shared_ptr&&) noexcept;
    template <class U>
    shared_ptr& operator=(shared_ptr<U>&&) noexcept;
    template <class U, class Del>
    shared_ptr& operator=(unique_ptr<U, Del>&&);

    void swap(shared_ptr&) noexcept;

    // get
    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;

    long use_count() const noexcept;
    bool unique() const noexcept;

    // set
    void reset() noexcept;
    template <class U>
    void reset(U* p);
    template <class U, class Del>
    void reset(U* p, Del del);
    template <class U, class Del, class Alloc>
    void reset(U* p, Del del, Alloc alloc);

    template <class U>
    bool owner_before(const shared_ptr<U>& x) const;
    template <class U>
    bool owner_before(const weak_ptr<U>& x) const;

private:
    T* stored_;
    ctrl* ctrl_;
};

template <class T>
class shared_ptr<T>::ctrl
{
    struct base
    {
        virtual ~base() {}
        virtual void del(T*) = 0;
    };

    template <class Deleter, class Alloc>
    struct inherit : base
    {
        inherit(Deleter del, Alloc alloc) : del_(del), alloc_(alloc) {}
        virtual ~inherit() {}
        virtual void del(T* t) override { del_(t); }
        Deleter del_;
        Alloc alloc_;
    };

private:
    base* base_;

public:
    template <class Deleter = default_delete<T>, class Alloc = allocator<T>>
    ctrl(T* ptr = nullptr, Deleter del = Deleter(), Alloc alloc = Alloc())
        : managed(ptr),
          shared(1),
          weak(0),
          base_(new inherit<Deleter, Alloc>(del, alloc))
    {
    }
    ~ctrl()
    {
        base_->del(managed);
        delete base_;
    }

public:
    T* managed;
    int shared;
    int weak;
};

//================================= ctor & dtor ==============================//
// default
template <class T>
constexpr shared_ptr<T>::shared_ptr() noexcept : stored_(nullptr),
                                                 ctrl_(new ctrl())
{
}
template <class T>
constexpr shared_ptr<T>::shared_ptr(std::nullptr_t) : shared_ptr()
{
}
// from pointer
template <class T>
template <class U>
shared_ptr<T>::shared_ptr(U* p) : stored_(p), ctrl_(new ctrl(p))
{
}
// from deleter
template <class T>
template <class U, class Del>
shared_ptr<T>::shared_ptr(U* p, Del del) : stored_(p), ctrl_(new ctrl(p, del))
{
}
template <class T>
template <class Del>
shared_ptr<T>::shared_ptr(std::nullptr_t, Del del)
    : stored_(nullptr), ctrl(new ctrl(nullptr, del))
{
}
// with deleter and allocator
template <class T>
template <class U, class Del, class Alloc>
shared_ptr<T>::shared_ptr(U* p, Del del, Alloc alloc)
    : stored_(p), ctrl_(new ctrl(p, del, alloc))
{
    ctrl_->shared = 1;
}
template <class T>
template <class Del, class Alloc>
shared_ptr<T>::shared_ptr(std::nullptr_t, Del del, Alloc alloc)
    : stored_(del), ctrl_(new ctrl(nullptr, del, alloc))
{
}
// copy
template <class T>
shared_ptr<T>::shared_ptr(const shared_ptr& x) noexcept : stored_(x.stored_),
                                                          ctrl_(x.ctrl_)
{
    ++(ctrl_->shared);
}
template <class T>
template <class U>
shared_ptr<T>::shared_ptr(const shared_ptr<U>& x) noexcept : stored_(x.ptr_),
                                                             ctrl_(x.ctrl_)
{
    ++(ctrl_->shared);
}
// copy from weak

// move
template <class T>
shared_ptr<T>::shared_ptr(shared_ptr&& x) noexcept : stored_(x.stored_),
                                                     ctrl_(x.ctrl_)
{
    x.ctrl_ = new ctrl();
}
template <class T>
template <class U>
shared_ptr<T>::shared_ptr(shared_ptr<U>&& x) noexcept : stored_(x.stored_),
                                                        ctrl_(x.ctrl_)
{
    x.ctrl_ = new ctrl();
}
// move from unique
template <class T>
template <class U, class Del>
shared_ptr<T>::shared_ptr(unique_ptr<U, Del>&& x)
    : stored_(x.get()), ctrl_(new ctrl(x.get(), move(x.get_deleter())))
{
    x.release();
}
// alias
template <class T>
template <class U>
shared_ptr<T>::shared_ptr(const shared_ptr<U>& x, element_type* p) noexcept
    : stored_(p),
      ctrl_(x.ctrl_)
{
    ++(ctrl_->shared);
}

template <class T>
shared_ptr<T>::~shared_ptr()
{
    --(ctrl_->shared);
    if (ctrl_->shared == 0) delete ctrl_;
}

//================================= assignment ===============================//
// copy
template <class T>
shared_ptr<T>&
shared_ptr<T>::operator=(const shared_ptr& x) noexcept
{
    shared_ptr<T>(x).swap(*this);
    return *this;
}

template <class T>
template <class U>
shared_ptr<T>&
shared_ptr<T>::operator=(const shared_ptr<U>& x) noexcept
{
    shared_ptr<T>(x) > swap(*this);
    return *this;
}
template <class T>
template <class U, class Del>
shared_ptr<T>&
shared_ptr<T>::operator=(unique_ptr<U, Del>&& x)
{
    shared_ptr<T>(x).swap(*this);
    return *this;
}

template <class T>
void
shared_ptr<T>::swap(shared_ptr& x) noexcept
{
    std::swap(stored_, x.stored_);
    std::swap(ctrl_, x.ctrl_);
}

//===================================== get ==================================//
template <class T>
T*
shared_ptr<T>::get() const noexcept
{
    return stored_;
}

template <class T>
T& shared_ptr<T>::operator*() const noexcept
{
    return *stored_;
}

template <class T>
T* shared_ptr<T>::operator->() const noexcept
{
    return stored_;
}

template <class T>
long
shared_ptr<T>::use_count() const noexcept
{
    return ctrl_->shared;
}

template <class T>
bool
shared_ptr<T>::unique() const noexcept
{
    return ctrl_->shared == 1;
}

//===================================== set ==================================//
template <class T>
void
shared_ptr<T>::reset() noexcept
{
    shared_ptr<T>().swap(*this);
}
template <class T>
template <class U>
void
shared_ptr<T>::reset(U* p)
{
    shared_ptr<T>(p).swap(*this);
}

template <class T>
template <class U, class Del>
void
shared_ptr<T>::reset(U* p, Del del)
{
    shared_ptr<T>(p, del).swap(*this);
}

template <class T>
template <class U, class Del, class Alloc>
void
shared_ptr<T>::reset(U* p, Del del, Alloc alloc)
{
    shared_ptr<T>(p, del, alloc).swap(*this);
}

template <class T>
template <class U>
bool
shared_ptr<T>::owner_before(const shared_ptr<U>& x) const
{
    return ctrl_->managed < x.ctrl_->managed;
}

//============================ non-member functions ==========================//
template <class T>
void
swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <class T>
bool
operator==(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept
{
    return lhs.get() == rhs.get();
}

template <class T>
bool
operator!=(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept

{
    return lhs.get() != rhs.get();
}

template <class T>
bool
operator<(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept

{
    return lhs.get() < rhs.get();
}

template <class T>
bool
operator<=(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept
{
    return lhs.get() <= rhs.get();
}

template <class T>
bool
operator>(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept
{
    return lhs.get() > rhs.get();
}

template <class T>
bool
operator>=(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) noexcept
{
    return lhs.get() >= rhs.get();
}

template <class T>
bool
operator==(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
{
    return nullptr == rhs.get();
}

template <class T>
bool
operator!=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
{
    return nullptr != rhs.get();
}

template <class T>
bool
operator<(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
{
    return nullptr < rhs.get();
}

template <class T>
bool
operator<=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
{
    return nullptr <= rhs.get();
}

template <class T>
bool
operator>(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
{
    return nullptr > rhs.get();
}

template <class T>
bool
operator>=(std::nullptr_t, const shared_ptr<T>& rhs) noexcept
{
    return nullptr >= rhs.get();
}

template <class T>
bool
operator==(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
{
    return lhs.get() == nullptr;
}

template <class T>
bool
operator!=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
{
    return lhs.get() != nullptr;
}

template <class T>
bool
operator<(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
{
    return lhs.get() < nullptr;
}

template <class T>
bool
operator<=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
{
    return lhs.get() <= nullptr;
}

template <class T>
bool
operator>(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
{
    return lhs.get() > nullptr;
}

template <class T>
bool
operator>=(const shared_ptr<T>& lhs, std::nullptr_t) noexcept
{
    return lhs.get() >= nullptr;
}
}
