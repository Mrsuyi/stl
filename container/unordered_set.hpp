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
    unordered_set() : ht_() {}
    explicit unordered_set(size_t bucket_suggest, const Hash& hs = Hash(),
                           const KeyEqual& ke = KeyEqual(),
                           const Allocator& al = Allocator())
        : ht_(bucket_suggest, hs, ke, al)
    {
    }
    // copy
    unordered_set(const unordered_set& other) : ht_(other.ht_) {}
    // move
    unordered_set(unordered_set&& other) : ht_(mrsuyi::move(other.ht_)) {}
    ~unordered_set() {}
    // range
    template <class InputIt>
    unordered_set(InputIt first, InputIt last) : ht_()
    {
        for (; first != last; ++first) insert(*first);
    }
    // il
    unordered_set(std::initializer_list<Key> il)
        : unordered_set(il.begin(), il.end())
    {
    }

    unordered_set& operator=(const unordered_set& other) { ht_ = other.ht_; }
    unordered_set& operator=(unordered_set&& other)
    {
        ht_ = mrsuyi::move(other.ht_);
    }

    Allocator get_allocator() const { return ht_.get_allocator(); }
    // iterators
    iterator begin() noexcept { return ht_.begin(); }
    iterator end() noexcept { return ht_.end(); }
    const_iterator begin() const noexcept { return ht_.begin(); }
    const_iterator end() const noexcept { return ht_.end(); }
    const_iterator cbegin() const noexcept { return ht_.cbegin(); }
    const_iterator cend() const noexcept { return ht_.cend(); }
    // capacity
    bool empty() const noexcept { return empty(); }
    size_t size() const noexcept { return size(); }
    size_t max_size() const noexcept { return max_size(); }
    // modifiers
    void clear() { ht_.clear(); }
    // insert
    mrsuyi::pair<iterator, bool> insert(const Key& key)
    {
        return insert(Key(key));
    }
    mrsuyi::pair<iterator, bool> insert(Key&& key)
    {
        return emplace(mrsuyi::move(key));
    }
    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first) insert(*first);
    }
    void insert(std::initializer_list<Key> il)
    {
        insert(il.begin(), il.end());
    }
    template <class... Args>
    mrsuyi::pair<iterator, bool> emplace(Args... args)
    {
        return ht_.emplace_unique(mrsuyi::forward<Args>(args)...);
    }

    iterator erase(const_iterator pos) { return ht_.erase(pos); }
    iterator erase(const_iterator first, const_iterator last)
    {
        return ht_.erase(first, last);
    }
    size_t erase(const Key& key) { return ht_.erase_unique(key); }
    void swap(unordered_set& other) { ht_.swap(other.ht_); }
    // lookup
    size_t count(const Key& key) const { return ht_.count_unique(key); }
    iterator find(const Key& key) { return ht_.find(key); }
    const_iterator find(const Key& key) const { return ht_.find(key); }
    mrsuyi::pair<iterator, iterator> equal_range(const Key& key)
    {
        return ht_.equal_range(key);
    }
    mrsuyi::pair<const_iterator, const_iterator> equal_range(
        const Key& key) const
    {
        return ht_.equal_range(key);
    }

    // bucket interface
    local_iterator begin(size_t n) noexcept { return ht_.begin(n); }
    local_iterator end(size_t n) noexcept { return ht_.end(n); }
    const_local_iterator begin(size_t n) const noexcept { return ht_.begin(n); }
    const_local_iterator end(size_t n) const noexcept { return ht_.end(n); }
    const_local_iterator cbegin(size_t n) const noexcept
    {
        return ht_.cbegin(n);
    }
    const_local_iterator cend(size_t n) const noexcept { return ht_.cend(n); }
    // bucket
    size_t bucket_count() const { return ht_.bucket_count(); }
    size_t max_bucket_count() const { return ht_.max_bucket_count(); }
    size_t bucket_size(size_t n) const { return ht_.bucket_size(n); }
    size_t bucket(const Key& key) const { return ht_.bucket(key); }
    // hash policy
    float load_factor() const { return ht_.load_factor(); }
    float max_load_factor() const { return ht_.max_load_factor(); }
    void max_load_factor(float ml) { ht_.max_load_factor(ml); }
    void rehash(size_t count) { ht_.rehash(count); }
    void reserve(size_t count) { ht_.reserve(count); }
    // observers
    Hash hash_function() const { return ht_.hash_function(); }
    KeyEqual key_eq() const { return ht_.key_eq(); }
protected:
    ht_t ht_;
};
}
