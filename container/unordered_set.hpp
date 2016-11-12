#pragma once

#include "functional.hpp"
#include "memory.hpp"

namespace mrsuyi
{
template <class Key, class Hash = mrsuyi::hash<Key>,
          class KeyEqual = mrsuyi::equal_to<Key>,
          class Allocator = mrsuyi::allocator<Key>>
class unordered_set
{
};
}
