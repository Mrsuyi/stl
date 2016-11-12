#pragma once

#include "functional.hpp"
#include "memory.hpp"

namespace mrsuyi
{
template <class Key, class T, class Hash = mrsuyi::hash<Key>,
          class KeyEqual = mrsuyi::equal_to<Key>,
          class Allocator = mrsuyi::allocator<mrsuyi::pair<const Key, T>>>
class unordered_map
{
};
}
