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
public:
    using element_type = T;

    // cons & des
    // default
    constexpr shared_ptr() noexcpet;
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

    element_type* get() const noexcept;
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
    T* ptr_;
};
}
