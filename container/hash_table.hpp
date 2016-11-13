#pragma once

#include <cmath>
#include <initializer_list>
#include "algorithm.hpp"
#include "debug.hpp"
#include "functional.hpp"
#include "memory.hpp"
#include "vector.hpp"

namespace mrsuyi
{
static const std::size_t prime_nums[] = {
    /* 0     */ 5ul,
    /* 1     */ 11ul,
    /* 2     */ 23ul,
    /* 3     */ 47ul,
    /* 4     */ 97ul,
    /* 5     */ 199ul,
    /* 6     */ 409ul,
    /* 7     */ 823ul,
    /* 8     */ 1741ul,
    /* 9     */ 3469ul,
    /* 10    */ 6949ul,
    /* 11    */ 14033ul,
    /* 12    */ 28411ul,
    /* 13    */ 57557ul,
    /* 14    */ 116731ul,
    /* 15    */ 236897ul,
    /* 16    */ 480881ul,
    /* 17    */ 976369ul,
    /* 18    */ 1982627ul,
    /* 19    */ 4026031ul,
    /* 20    */ 8175383ul,
    /* 21    */ 16601593ul,
    /* 22    */ 33712729ul,
    /* 23    */ 68460391ul,
    /* 24    */ 139022417ul,
    /* 25    */ 282312799ul,
    /* 26    */ 573292817ul,
    /* 27    */ 1164186217ul,
    /* 28    */ 2364114217ul,
    /* 29    */ 4294967291ul,
    /* 30    */ (std::size_t)8589934583ull,
    /* 31    */ (std::size_t)17179869143ull,
    /* 32    */ (std::size_t)34359738337ull,
    /* 33    */ (std::size_t)68719476731ull,
    /* 34    */ (std::size_t)137438953447ull,
    /* 35    */ (std::size_t)274877906899ull,
    /* 36    */ (std::size_t)549755813881ull,
    /* 37    */ (std::size_t)1099511627689ull,
    /* 38    */ (std::size_t)2199023255531ull,
    /* 39    */ (std::size_t)4398046511093ull,
    /* 40    */ (std::size_t)8796093022151ull,
    /* 41    */ (std::size_t)17592186044399ull,
    /* 42    */ (std::size_t)35184372088777ull,
    /* 43    */ (std::size_t)70368744177643ull,
    /* 44    */ (std::size_t)140737488355213ull,
    /* 45    */ (std::size_t)281474976710597ull,
    /* 46    */ (std::size_t)562949953421231ull,
    /* 47    */ (std::size_t)1125899906842597ull,
    /* 48    */ (std::size_t)2251799813685119ull,
    /* 49    */ (std::size_t)4503599627370449ull,
    /* 50    */ (std::size_t)9007199254740881ull,
    /* 51    */ (std::size_t)18014398509481951ull,
    /* 52    */ (std::size_t)36028797018963913ull,
    /* 53    */ (std::size_t)72057594037927931ull,
    /* 54    */ (std::size_t)144115188075855859ull,
    /* 55    */ (std::size_t)288230376151711717ull,
    /* 56    */ (std::size_t)576460752303423433ull,
    /* 57    */ (std::size_t)1152921504606846883ull,
    /* 58    */ (std::size_t)2305843009213693951ull,
    /* 59    */ (std::size_t)4611686018427387847ull,
    /* 60    */ (std::size_t)9223372036854775783ull,
    /* 61    */ (std::size_t)18446744073709551557ull,
};

template <class Key>
struct hash_table_node
{
    Key key;
    hash_table_node* nxt;

    template <class... Args>
    hash_table_node(Args&&... args)
        : nxt(this), key(mrsuyi::forward<Args>(args)...)
    {
    }
};

template <class Key, class Node = hash_table_node<Key>,
          class Hash = mrsuyi::hash<Key>,
          class KeyEqual = mrsuyi::equal_to<Key>,
          class Allocator = mrsuyi::allocator<Key>>
class hash_table
{
    template <class HashTable, class E>
    class iter;
    template <class E>
    class local_iter;

public:
    using iterator = iter<hash_table, Key>;
    using const_iterator = iter<const hash_table, const Key>;
    using local_iterator = local_iter<Key>;
    using const_local_iterator = local_iter<const Key>;

public:
    // ctor & dtor
    hash_table();
    explicit hash_table(size_t bucket_suggest, const Hash& = Hash(),
                        const KeyEqual& = KeyEqual(),
                        const Allocator& = Allocator());
    /*    // range*/
    // template <class InputIt>
    // hash_table(InputIt first, InputIt last, size_t bucket_count,
    // const Hash& = Hash(), const KeyEqual& = KeyEqual(),
    // const Allocator& = Allocator());
    //// initializer_list
    // hash_table(std::initializer_list<Key> init, size_t bucket_suggest,
    // const Hash& = Hash(), const KeyEqual& = KeyEqual(),
    // const Allocator& = Allocator());
    ~hash_table();

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

    mrsuyi::pair<iterator, bool> insert_unique(const Key& key);
    mrsuyi::pair<iterator, bool> insert_unique(Key&& key);
    template <class InputIt>
    void insert_unique(InputIt first, InputIt last);
    void insert_unique(std::initializer_list<Key> il);

    mrsuyi::pair<iterator, bool> insert_equal(const Key& key);
    mrsuyi::pair<iterator, bool> insert_equal(Key&& key);
    template <class InputIt>
    void insert_equal(InputIt first, InputIt last);
    void insert_equal(std::initializer_list<Key> il);

    template <class... Args>
    mrsuyi::pair<iterator, bool> emplace_unique(Args... args);
    template <class... Args>
    mrsuyi::pair<iterator, bool> emplace_equal(Args... args);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_t erase_equal(const Key& key);
    size_t erase_unique(const Key& key);

    // lookup
    size_t count_equal(const Key& key) const;
    size_t count_unique(const Key& key) const;
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
    // Node* first();
    Node* first() const;
    // clear all nodes in buckets
    void clear_buckets();
    // find node with key
    Node* search(const Key& key) const;
    // find range
    mrsuyi::pair<Node*, Node*> search_range(const Key& key) const;

protected:
    Hash hash_;
    KeyEqual key_equal_;

    mrsuyi::vector<Node*> buckets_;
    size_t node_count_;
    float max_load_factor_;
};

//=================================== iter ===================================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
template <class HashTable, class E>
class hash_table<Key, Node, Hash, KeyEqual, Allocator>::iter
{
    friend class hash_table<Key, Node, Hash, KeyEqual, Allocator>;

public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = mrsuyi::forward_iterator_tag;

    iter() : ht_(nullptr), node_(nullptr) {}
    iter(HashTable* ht, Node* node) : ht_(ht), node_(node) {}
    iter(const iter& it) : ht_(it.ht_), node_(it.node_) {}
    iter& operator=(const iter& it)
    {
        ht_ = it.ht_;
        node_ = it.node_;
    }
    E& operator*() const { return node_->key; }
    E* operator->() const { return &(node_->key); }
    bool operator==(const iter& it) { return node_ == it.node_; }
    bool operator!=(const iter& it) { return node_ != it.node_; }
    iter& operator++()
    {
        Node* old = node_;
        node_ = node_->nxt;

        if (!node_)
        {
            size_t n = ht_->bucket(old->key);
            while (!node_ && ++n < ht_->bucket_count())
                node_ = ht_->buckets_[n];
        }
        return *this;
    }
    iter operator++(int)
    {
        auto res = *this;
        ++*this;
        return res;
    }
    operator iter<const HashTable, const E>() const
    {
        return iter<const HashTable, const E>(ht_, node_);
    }

protected:
    HashTable* ht_;
    Node* node_;
};

template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
template <class E>
class hash_table<Key, Node, Hash, KeyEqual, Allocator>::local_iter
{
public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using local_iterator_category = mrsuyi::forward_iterator_tag;

    local_iter() : node_(nullptr) {}
    local_iter(Node* node) : node_(node) {}
    local_iter(const local_iter& it) : node_(it.node_) {}
    local_iter& operator=(const local_iter& it) { node_ = it.node_; }
    E& operator*() const { return node_->key; }
    E* operator->() const { return &(node_->key); }
    bool operator==(const local_iter& it) { return node_ == it.node_; }
    bool operator!=(const local_iter& it) { return node_ != it.node_; }
    local_iter& operator++() { node_ = node_->nxt; }
    local_iter operator++(int)
    {
        auto res = *this;
        ++*this;
        return res;
    }
    operator local_iter<const E>() const { return local_iter<const E>(node_); }
private:
    Node* node_;
};

//================================= protected ================================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
Node*
hash_table<Key, Node, Hash, KeyEqual, Allocator>::first() const
{
    Node* res = buckets_[0];
    size_t n = 1;
    for (; !res && n < bucket_count(); ++n) res = buckets_[n];
    return res;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::clear_buckets()
{
    for (size_t i = 0; i < buckets_.size(); ++i)
    {
        Node* node = buckets_[i];
        while (node)
        {
            Node* tmp = node;
            node = node->nxt;
            delete tmp;
        }
    }
    node_count_ = 0;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
Node*
hash_table<Key, Node, Hash, KeyEqual, Allocator>::search(const Key& key) const
{
    Node* cur = buckets_[bucket(key)];
    while (cur && !key_equal(cur->key, key)) cur = cur->nxt;
    return cur;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<Node*, Node*>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::search_range(
    const Key& key) const
{
    Node* bgn = buckets_[bucket(key)];
    while (bgn && !key_equal(bgn->key, key)) bgn = bgn->nxt;
    Node* end = bgn;
    while (end && key_equal(end->key, key)) end = end->nxt;
    return {bgn, end};
}

//=================================== basic ==================================//
// ctor & dtor
// default
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::hash_table()
    : hash_table(prime_nums[0])
{
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::hash_table(
    size_t bucket_suggest, const Hash& hash, const KeyEqual& key_equal,
    const Allocator&)
    : hash_(hash),
      key_equal_(key_equal),
      buckets_(bucket_suggest),
      node_count_(0),
      max_load_factor_(1)
{
}
/*// range*/
// template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
// hash_table<Key, Node, Hash, KeyEqual, Allocator>::hash_table(
// InputIt first, InputIt last, size_t bucket_suggest, const Hash& hash,
// const KeyEqual& key_equal, const Allocator& alloc)
//: hash_table(bucket_suggest, hash, key_equal, alloc)
//{
// for (; first != last; ++first, emplace_
//}
//// initializer_list
// template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
// hash_table<Key, Node, Hash, KeyEqual, Allocator>::hash_table(
// std::initializer_list<Key> init, size_t bucket_suggest, const Hash&,
// const KeyEqual&, const Allocator&)
//{
//}

// dtor
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::~hash_table()
{
    clear_buckets();
}

//================================= iterators ================================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::begin() noexcept
{
    return iterator(this, first());
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::end() noexcept
{
    return iterator(this, nullptr);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::begin() const noexcept
{
    return const_iterator(this, first());
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::end() const noexcept
{
    return const_iterator(this, nullptr);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::cbegin() const noexcept
{
    return const_iterator(this, first());
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

//================================= capacity =================================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
bool
hash_table<Key, Node, Hash, KeyEqual, Allocator>::empty() const noexcept
{
    return size() == 0;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::size() const noexcept
{
    return node_count_;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::max_size() const noexcept
{
    return prime_nums[61];
}

//================================= modifiers ================================//
// clear
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::clear()
{
    clear_buckets();
    buckets_.assign(buckets_.size(), nullptr);
}
// insert
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator, bool>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_unique(const Key& key)
{
    return insert_unique(Key(key));
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator, bool>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_unique(Key&& key)
{
    return emplace_unique(mrsuyi::move(key));
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
template <class InputIt>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_unique(InputIt first,
                                                                InputIt last)
{
    for (; first != last; ++first) insert_unique(*first);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_unique(
    std::initializer_list<Key> il)
{
    insert_unique(il.begin(), il.end());
}
// insert
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator, bool>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_equal(const Key& key)
{
    return insert_equal(Key(key));
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator, bool>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_equal(Key&& key)
{
    return emplace_equal(mrsuyi::move(key));
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
template <class InputIt>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_equal(InputIt first,
                                                               InputIt last)
{
    for (; first != last; ++first) insert_equal(*first);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::insert_equal(
    std::initializer_list<Key> il)
{
    insert_equal(il.begin(), il.end());
}
// emplace
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
template <class... Args>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator, bool>
    hash_table<Key, Node, Hash, KeyEqual, Allocator>::emplace_unique(
        Args... args)
{
    reserve(size() + 1);
    Node* res = new Node(mrsuyi::forward<Args>(args)...);
    size_t n = bucket(res->key);
    for (Node* cur = buckets_[n]; cur; ++cur)
        if (key_equal_(res->key, cur->key))
        {
            delete res;
            return {iterator(this, cur), false};
        }
    res->nxt = buckets_[n];
    buckets_[n] = res;
    ++node_count_;
    return {iterator(this, res), true};
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
template <class... Args>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator, bool>
    hash_table<Key, Node, Hash, KeyEqual, Allocator>::emplace_equal(
        Args... args)
{
    reserve(size() + 1);
    Node* res = new Node(mrsuyi::forward<Args>(args)...);
    size_t n = bucket(res->key);
    for (Node* cur = buckets_[n]; cur; ++cur)
    {
        if (key_equal_(res->key, cur->key))
        {
            Node* tmp = cur->nxt;
            cur->nxt = res;
            res->nxt = tmp;
            ++node_count_;
            return {iterator(this, cur), true};
        }
    }
    res->nxt = buckets_[n];
    buckets_[n] = res;
    ++node_count_;
    return {iterator(this, res), true};
}
// erase
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::erase(const_iterator pos)
{
    auto res = pos;
    ++res;
    auto bucket_index = bucket(pos.node_->key);
    Node** mount = buckets_.data() + bucket_index;
    Node* cur = buckets_[bucket_index];
    while (cur != pos.node_)
    {
        mount = &(cur->nxt);
        cur = cur->nxt;
    }
    *mount = pos.node_->nxt;
    delete pos.node_;
    --node_count_;
    return iterator(this, res.node_);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::erase(const_iterator first,
                                                        const_iterator last)
{
    for (; first != last; first = erase(first))
        ;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::erase_equal(const Key& key)
{
    auto bucket_index = bucket(key);
    Node** mount = buckets_.data() + bucket_index;
    Node* cur = buckets_[bucket_index];
    while (cur && !key_equal_(cur->key, key))
    {
        mount = &(cur->nxt);
        cur = cur->nxt;
    }
    size_t res = 0;
    while (cur && key_equal_(cur->key, key))
    {
        auto nxt = cur->nxt;
        delete cur;
        cur = nxt;
        ++res;
    }
    *mount = cur;
    node_count_ -= res;
    return res;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::erase_unique(const Key& key)
{
    auto bucket_index = bucket(key);
    Node** mount = buckets_.data() + bucket_index;
    Node* cur = buckets_[bucket_index];
    while (cur && !key_equal_(cur->key, key))
    {
        mount = &(cur->nxt);
        cur = cur->nxt;
    }
    if (cur)
    {
        *mount = cur->nxt;
        delete cur;
        --node_count_;
    }
    return 1;
}

//================================== lookup ==================================//
// count
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::count_equal(
    const Key& key) const
{
    size_t res = 0;
    Node* cur = buckets_[bucket(key)];
    while (cur && key_equal_(cur->key, key))
    {
        ++res;
        cur = cur->nxt;
    }
    return res;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::count_unique(
    const Key& key) const
{
    Node* cur = buckets_[bucket(key)];
    while (cur)
    {
        if (key_equal_(cur->key, key))
            return 1;
        else
            cur = cur->nxt;
    }
    return 0;
}
// find
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::find(const Key& key)
{
    return iterator(this, search(key));
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::find(const Key& key) const
{
    return const_iterator(this, search(key));
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator,
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::iterator>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::equal_range(const Key& key)
{
    auto p = search_range(key);
    return {iterator(this, p.first), iterator(this, p.second)};
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
mrsuyi::pair<
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator,
    typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_iterator>
hash_table<Key, Node, Hash, KeyEqual, Allocator>::equal_range(
    const Key& key) const
{
    auto p = search_range(key);
    return {const_iterator(this, p.first), const_iterator(this, p.second)};
}

//============================== bucket interface ============================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::local_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::begin(size_t n) noexcept
{
    return local_iterator(buckets_[n]);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::local_iterator
    hash_table<Key, Node, Hash, KeyEqual, Allocator>::end(size_t) noexcept
{
    return local_iterator(nullptr);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_local_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::begin(size_t n) const noexcept
{
    return const_local_iterator(buckets_[n]);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_local_iterator
    hash_table<Key, Node, Hash, KeyEqual, Allocator>::end(size_t) const noexcept
{
    return local_iterator(nullptr);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_local_iterator
hash_table<Key, Node, Hash, KeyEqual, Allocator>::cbegin(size_t n) const
    noexcept
{
    return const_local_iterator(buckets_[n]);
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
typename hash_table<Key, Node, Hash, KeyEqual, Allocator>::const_local_iterator
    hash_table<Key, Node, Hash, KeyEqual, Allocator>::cend(size_t) const
    noexcept
{
    return local_iterator(nullptr);
}

// bucket-count
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::bucket_count() const
{
    return buckets_.size();
}
// max-bucket-count
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::max_bucket_count() const
{
    return prime_nums[61];
}
// bucket-size
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::bucket_size(size_t n) const
{
    return mrsuyi::distance(begin(n), end(n));
}
// bucket
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
size_t
hash_table<Key, Node, Hash, KeyEqual, Allocator>::bucket(const Key& key) const
{
    return hash_(key) % buckets_.size();
}

//=============================== hash policy ================================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
float
hash_table<Key, Node, Hash, KeyEqual, Allocator>::load_factor() const
{
    return size() / bucket_count();
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
float
hash_table<Key, Node, Hash, KeyEqual, Allocator>::max_load_factor() const
{
    return max_load_factor_;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::max_load_factor(float ml)
{
    max_load_factor_ = ml;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::rehash(size_t count)
{
    count = max(count, size_t(size() / max_load_factor()));
    if (count <= buckets_.size()) return;
    count = *mrsuyi::lower_bound(prime_nums, prime_nums + 61, count);

    auto tmp = mrsuyi::move(buckets_);
    buckets_ = mrsuyi::vector<Node*>(count);
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        Node* node = tmp[i];
        while (node)
        {
            auto new_index = bucket(node->key);
            Node* nxt = node->nxt;
            node->nxt = buckets_[new_index];
            buckets_[new_index] = node;
            node = nxt;
        }
    }
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
void
hash_table<Key, Node, Hash, KeyEqual, Allocator>::reserve(size_t count)
{
    rehash(std::ceil(count / max_load_factor()));
}
//=============================== hash policy ================================//
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
Hash
hash_table<Key, Node, Hash, KeyEqual, Allocator>::hash_function() const
{
    return hash_;
}
template <class Key, class Node, class Hash, class KeyEqual, class Allocator>
KeyEqual
hash_table<Key, Node, Hash, KeyEqual, Allocator>::key_eq() const
{
    return key_equal_;
}
}
