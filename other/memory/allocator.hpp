#pragma once

#include <cstddef>

namespace mrsuyi
{

template <class T>
class allocator;

// specialized version of allocator<void>
template <>
class allocator<void>
{
public:
    using value_type = void;
    using pointer = void*;
    using const_pointer = const void*;

    template <class U>
    struct rebind { using other = allocator<U>; };
};

template <class T>
class allocator
{
// typedef
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    template <class Type>
    struct rebind { using other = allocator<Type>; };

// constructor & desctructor
public:
    allocator() throw();
    allocator(const allocator&) throw();
    template<class U>
    allocator(const allocator<U>&) throw();

    ~allocator() throw();

public:
    pointer address(reference x) const;
    const_pointer address(const_reference x) const;

    pointer allocate(size_type n, allocator<void>::const_pointer hint = 0);
    void deallocate(pointer p, size_type n);

    void construct(pointer p, const_reference val);
    void destory(pointer p);

    size_type max_size() const throw();
};


}
