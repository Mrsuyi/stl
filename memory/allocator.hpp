#pragma once

#include <cstddef>
#include <cstdlib>
#include <functional>
#include <limits>

namespace mrsuyi
{
template <class T>
class allocator;

template <>
class allocator<void>
{
public:
    using pointer = void*;
    using const_pointer = const void*;
    using value_type = void;

    template <class U>
    struct rebind
    {
        using other = allocator<U>;
    };
};

template <class T>
class allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    // cons & des
    allocator() noexcept;
    template <class U>
    allocator(const allocator<U>& alloc) noexcept;
    ~allocator() noexcept;

    // mem
    pointer allocate(size_type n, allocator<void>::const_pointer hint = 0);
    void deallocate(pointer p, size_type n);

    // cons & des
    template <class U, class... Args>
    void construct(U* p, Args&&... args);
    template <class U>
    void destroy(U* p);

    // extra
    pointer address(reference x) noexcept;
    const_pointer address(const_reference x) const noexcept;
    size_type max_size() const noexcept;
};

template <class T>
allocator<T>::allocator() noexcept
{
}

template <class T>
template <class U>
allocator<T>::allocator(const allocator<U>&) noexcept
{
}

template <class T>
allocator<T>::~allocator() noexcept
{
}

template <class T>
typename allocator<T>::pointer
allocator<T>::allocate(size_type n, allocator<void>::const_pointer hint)
{
    return (pointer)realloc((void*)hint, n * sizeof(value_type));
}

template <class T>
void
allocator<T>::deallocate(pointer p, size_type)
{
    free(p);
}

template <class T>
template <class U, class... Args>
void
allocator<T>::construct(U* p, Args&&... args)
{
    ::new ((void*)p) U(std::forward<Args>(args)...);
}

template <class T>
template <class U>
void
allocator<T>::destroy(U* p)
{
    p->~U();
}

template <class T>
typename allocator<T>::pointer
allocator<T>::address(reference x) noexcept
{
    return &x;
}

template <class T>
typename allocator<T>::const_pointer
allocator<T>::address(const_reference x) const noexcept
{
    return &x;
}

template <class T>
typename allocator<T>::size_type
allocator<T>::max_size() const noexcept
{
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
}
};
