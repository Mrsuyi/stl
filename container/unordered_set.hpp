#pragma once

#include "functional.hpp"
#include "hash_table.hpp"
#include "memory.hpp"

namespace mrsuyi
{
template <class Key, class Hash, class KeyEqual, class Allocator>
using set_hash_table =
    hash_table<Key, Key, Hash, mrsuyi::identity<Key>, KeyEqual, Allocator>;

template <class Key, class Hash = mrsuyi::hash<Key>,
          class KeyEqual = mrsuyi::equal_to<Key>,
          class Allocator = mrsuyi::allocator<Key>>
class unordered_set
{
    using ht_t = mrsuyi::set_hash_table<Key, Hash, KeyEqual, Allocator>;

public:
    using key_type = typename ht_t::key_type;
    using value_type = typename ht_t::value_type;
    using size_type = typename ht_t::size_type;
    using difference_type = typename ht_t::difference_type;
    using hasher = typename ht_t::hasher;
    using key_equal = typename ht_t::key_equal;
    using allocator_type = typename ht_t::allocator_type;
    using reference = typename ht_t::reference;
    using const_reference = typename ht_t::const_reference;
    using pointer = typename ht_t::pointer;
    using const_pointer = typename ht_t::const_pointer;
    using iterator = typename ht_t::iterator;
    using const_iterator = typename ht_t::const_iterator;
    using local_iterator = typename ht_t::local_iterator;
    using const_local_iterator = typename ht_t::const_local_iterator;

    // ctor & dtor
    unordered_set();
    explicit unordered_set(size_t bucket_suggest, const Hash& = Hash(),
                           const KeyEqual& = KeyEqual(),
                           const Allocator& = Allocator());
    // copy
    unordered_set(const unordered_set&);
    // move
    unordered_set(unordered_set&&);
    ~unordered_set();

    unordered_set& operator=(const unordered_set& other);
    unordered_set& operator=(unordered_set&& other);

    Allocator get_allocator() const;

    // iterators
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // capacity
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t max_size() const noexcept;

    // modifiers
    void clear();

    template <class... Args>
    mrsuyi::pair<iterator, bool> emplace(Args... args);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_t erase(const Key& key);

    void swap(unordered_set& other);

    // lookup
    size_t count(const Key& key) const;
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    mrsuyi::pair<iterator, iterator> equal_range(const Key& key);
    mrsuyi::pair<const_iterator, const_iterator> equal_range(
        const Key& key) const;

    // bucket interface
    local_iterator begin(size_t n) noexcept;
    local_iterator end(size_t n) noexcept;
    const_local_iterator begin(size_t n) const noexcept;
    const_local_iterator end(size_t n) const noexcept;
    const_local_iterator cbegin(size_t n) const noexcept;
    const_local_iterator cend(size_t n) const noexcept;

    size_t bucket_count() const;
    size_t max_bucket_count() const;
    size_t bucket_size(size_t n) const;
    size_t bucket(const Key& key) const;

    // hash policy
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml);
    void rehash(size_t count);
    void reserve(size_t count);

    // observers
    Hash hash_function() const;
    KeyEqual key_eq() const;

protected:
    ht_t ht_;
};
}
