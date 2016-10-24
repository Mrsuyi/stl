#pragma once

#include <cstddef>

namespace mrsuyi
{
template <class T>
class weak_ptr;
template <class T, class D>
class unique_ptr;

template <class T>
class shared_ptr
{
    template <class U = T, class D = std::, class Alloc>
    struct control
    {
        U* managed_;
        int shared_cnt_;
        D deleter_;
        Alloc alloc_;
    };

public:
    using element_type = T;

    // cons & des
    // default
    constexpr shared_ptr() noexcept;
    // from null pointer
    constexpr shared_ptr(std::nullptr_t);
    // from pointer
    template <class U>
    explicit shared_ptr(U*);
    // from deleter
    template <class U, class D>
    shared_ptr(U* p, D del);
    template <class D>
    shared_ptr(std::nullptr_t, D del);
    // with allocator
    template <class U, class D, class Alloc>
    shared_ptr(U* p, D del, Alloc alloc);
    template <class D, class Alloc>
    shared_ptr(std::nullptr_t, D del, Alloc alloc);
    // copy
    shared_ptr(const shared_ptr& x) noexcept;
    template <class U>
    shared_ptr(const shared_ptr<U>& x) noexcept;
    // copy from wek
    template <class U>
    shared_ptr(const weak_ptr<U>& x);
    // move
    shared_ptr(shared_ptr&& x) noexcept;
    template <class U>
    shared_ptr(shared_ptr<U>&& x) noexcept;
    // move from unique
    template <class U, class D>
    shared_ptr(unique_ptr<U, D>&& x);

    ~shared_ptr();

    // copy =
    shared_ptr& operator=(const shared_ptr& x) noexcept;
    template <class U>
    shared_ptr& operator=(const shared_ptr<U>& x) noexcept;
    // move =
    shared_ptr& operator=(shared_ptr&& x) noexcept;
    template <class U>
    shared_ptr& operator=(shared_ptr<U>&& x) noexcept;
    template <class U, class D>
    shared_ptr& operator=(unique_ptr<U, D>&& x);

    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;

    long use_count() const noexcept;
    bool unique() const noexcept;
    void swap(shared_ptr& x) noexcept;

    void reset() noexcept;
    template <class U>
    void reset(U* p);
    template <class U, class D>
    void reset(U* p, D del);
    template <class U, class D, class Alloc>
    void reset(U* p, D del, Alloc alloc);

    template <class U>
    bool owner_before(const shared_ptr<U>& x) const;
    template <class U>
    bool owner_before(const weak_ptr<U>& x) const;

private:
    T* stored_;
    control* ctrl_;
};

template <class T>
constexpr shared_ptr<T>::shared_ptr() noexcept : ctrl_(new control())
{
}

template <class T>
constexpr shared_ptr<T>::shared_ptr(std::nullptr_t) : shared_ptr()
{
}

template <class T>
template <class U>
shared_ptr<T>::shared_ptr(U* p) : shared_ptr(), stored_(p)
{
    ctrl_->managed_ = p;
    ctrl_->shared_cnt_ = 1;
}

template <class T>
template <class U, class D>
shared_ptr<T>::shared_ptr(U* p, D del) : shared_ptr(p)
{
}

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
}
