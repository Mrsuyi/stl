#pragma once

#include "functional.hpp"
#include "memory.hpp"
#include "vector.hpp"

namespace mrsuyi
{
template <class T, class Hash = hash<T>, class KeyEqual = equal_to<T>,
          class Allocator = allocator<T>>
class hash_table
{
    template <class E>
    class iter;

public:
    using iterator = iter<T>;
    using const_iterator = iter<T>;

    // ctor & dtor
    hash_table();
    ~hash_table();

    // iterators

protected:
    Hash hash_;
    KeyEqual equal_;
    Allocator alloc_;
};
}
