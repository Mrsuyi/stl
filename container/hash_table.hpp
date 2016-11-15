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

template <class Value, class Key = Value, class Hash = mrsuyi::hash<Key>,
          class ExtractKey = mrsuyi::identity<Value>,
          class KeyEqual = mrsuyi::equal_to<Key>,
          class Allocator = mrsuyi::allocator<Key>>
class hash_table
{
    struct node
    {
        Value val;
        node* nxt;

        template <class... Args>
        node(Args&&... args) : nxt(nullptr), val(mrsuyi::forward<Args>(args)...)
        {
        }
    };

    template <class HashTable, class E>
    class iter;
    template <class E>
    class local_iter;

public:
    using key_type = Key;
    using value_type = Value;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    // TODO allocator_traits
    //using pointer = value_type*;
    //using const_pointer = const value_type*;

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
    // copy
    hash_table(const hash_table&);
    // move
    hash_table(hash_table&&);
    ~hash_table();

    hash_table& operator=(const hash_table& other);
    hash_table& operator=(hash_table&& other);

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
    mrsuyi::pair<iterator, bool> emplace_unique(Args... args);
    template <class... Args>
    mrsuyi::pair<iterator, bool> emplace_equal(Args... args);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_t erase_equal(const Key& key);
    size_t erase_unique(const Key& key);

    void swap(hash_table& other);

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
    // get first node
    node* first() const;
    // get next node
    node* next(size_t bucket_idx, node* n) const;
    // clear all nodes in buckets
    void clear_buckets();
    // find node with key
    node* search(const Key& key) const;
    // find range
    mrsuyi::pair<node*, node*> search_range(const Key& key) const;

protected:
    Hash hash_;
    ExtractKey extract_key_;
    KeyEqual key_equal_;
    Allocator alloc_;

    mrsuyi::vector<node*> buckets_;
    size_t node_count_;
    float max_load_factor_;
};

//=================================== iter ===================================//
template <class V, class K, class H, class Ex, class Kq, class A>
template <class HashTable, class E>
class hash_table<V, K, H, Ex, Kq, A>::iter
{
    friend class hash_table<V, K, H, Ex, Kq, A>;

public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = mrsuyi::forward_iterator_tag;

    iter() : ht_(nullptr), node_(nullptr) {}
    iter(HashTable* ht, node* node) : ht_(ht), node_(node) {}
    iter(const iter& it) : ht_(it.ht_), node_(it.node_) {}
    iter& operator=(const iter& it)
    {
        ht_ = it.ht_;
        node_ = it.node_;
    }
    E& operator*() const { return node_->val; }
    E* operator->() const { return &(node_->val); }
    bool operator==(const iter& it) { return node_ == it.node_; }
    bool operator!=(const iter& it) { return node_ != it.node_; }
    iter& operator++()
    {
        node* old = node_;
        node_ = node_->nxt;

        if (!node_)
        {
            size_t n = ht_->bucket(ht_->extract_key_(old->val));
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
    node* node_;
};

template <class V, class K, class H, class Ex, class Kq, class A>
template <class E>
class hash_table<V, K, H, Ex, Kq, A>::local_iter
{
public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using local_iterator_category = mrsuyi::forward_iterator_tag;

    local_iter() : node_(nullptr) {}
    local_iter(node* node) : node_(node) {}
    local_iter(const local_iter& it) : node_(it.node_) {}
    local_iter& operator=(const local_iter& it) { node_ = it.node_; }
    E& operator*() const { return node_->val; }
    E* operator->() const { return &(node_->val); }
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
    node* node_;
};

//================================= protected ================================//
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::node*
hash_table<V, K, H, Ex, Kq, A>::first() const
{
    node* res = buckets_[0];
    size_t n = 1;
    for (; !res && n < bucket_count(); ++n) res = buckets_[n];
    return res;
}
template <class V, class K, class H, class Ex, class Kq, class A>
void
hash_table<V, K, H, Ex, Kq, A>::clear_buckets()
{
    for (size_t i = 0; i < buckets_.size(); ++i)
    {
        node* cur = buckets_[i];
        while (cur)
        {
            node* tmp = cur;
            cur = cur->nxt;
            delete tmp;
        }
    }
    node_count_ = 0;
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::node*
hash_table<V, K, H, Ex, Kq, A>::search(const K& key) const
{
    node* cur = buckets_[bucket(key)];
    while (cur && !key_equal_(extract_key_(cur->val), key)) cur = cur->nxt;
    return cur;
}
template <class V, class K, class H, class Ex, class Kq, class A>
mrsuyi::pair<typename hash_table<V, K, H, Ex, Kq, A>::node*,
             typename hash_table<V, K, H, Ex, Kq, A>::node*>
hash_table<V, K, H, Ex, Kq, A>::search_range(const K& key) const
{
    auto bucket_idx = bucket(key);
    node* bgn = buckets_[bucket_idx];
    while (bgn && !key_equal_(extract_key_(bgn->val), key)) bgn = bgn->nxt;
    if (!bgn) return {bgn, nullptr};

    node* end = bgn->nxt;
    while (end && key_equal_(extract_key_(end->val), key)) end = end->nxt;
    while (!end && ++bucket_idx < bucket_count()) end = buckets_[bucket_idx];
    return {bgn, end};
}

//=================================== basic ==================================//
// ctor & dtor
// default
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>::hash_table() : hash_table(prime_nums[0])
{
}
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>::hash_table(size_t bucket_suggest, const H& hash,
                                           const Kq& key_equal, const A& alloc)
    : hash_(hash),
      extract_key_(Ex()),
      key_equal_(key_equal),
      alloc_(alloc),
      buckets_(bucket_suggest),
      node_count_(0),
      max_load_factor_(1)
{
}
// copy
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>::hash_table(const hash_table& other)
    : hash_(other.hash_),
      extract_key_(Ex()),
      key_equal_(other.key_equal_),
      alloc_(other.alloc_),
      buckets_(other.bucket_count()),
      node_count_(other.node_count_),
      max_load_factor_(other.max_load_factor_)
{
    for (size_t i = 0; i < bucket_count(); ++i)
    {
        node** mount = buckets_.data() + i;
        node* cur = other.buckets_[i];
        while (cur)
        {
            *mount = new node(cur->val);
            cur = cur->nxt;
            mount = &((*mount)->nxt);
        }
    }
}
// move
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>::hash_table(hash_table&& other)
    : hash_(other.hash_),
      extract_key_(Ex()),
      key_equal_(other.key_equal_),
      alloc_(other.alloc_),
      buckets_(mrsuyi::move(other.buckets_)),
      node_count_(other.node_count_),
      max_load_factor_(other.max_load_factor_)
{
    other.node_count_ = 0;
}
// dtor
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>::~hash_table()
{
    clear_buckets();
}
// = copy
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>&
hash_table<V, K, H, Ex, Kq, A>::operator = (const hash_table& other)
{
    hash_table(other).swap(*this);
    return *this;
}
// = move
template <class V, class K, class H, class Ex, class Kq, class A>
hash_table<V, K, H, Ex, Kq, A>&
hash_table<V, K, H, Ex, Kq, A>::operator = (hash_table&& other)
{
    hash_table(mrsuyi::move(other)).swap(*this);
    return *this;
}

//================================= iterators ================================//
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::iterator
hash_table<V, K, H, Ex, Kq, A>::begin() noexcept
{
    return iterator(this, first());
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::iterator
hash_table<V, K, H, Ex, Kq, A>::end() noexcept
{
    return iterator(this, nullptr);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_iterator
hash_table<V, K, H, Ex, Kq, A>::begin() const noexcept
{
    return const_iterator(this, first());
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_iterator
hash_table<V, K, H, Ex, Kq, A>::end() const noexcept
{
    return const_iterator(this, nullptr);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_iterator
hash_table<V, K, H, Ex, Kq, A>::cbegin() const noexcept
{
    return const_iterator(this, first());
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_iterator
hash_table<V, K, H, Ex, Kq, A>::cend() const noexcept
{
    return const_iterator(this, nullptr);
}

//================================= capacity =================================//
template <class V, class K, class H, class Ex, class Kq, class A>
bool
hash_table<V, K, H, Ex, Kq, A>::empty() const noexcept
{
    return size() == 0;
}
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::size() const noexcept
{
    return node_count_;
}
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::max_size() const noexcept
{
    return prime_nums[61];
}

//================================= modifiers ================================//
// clear
template <class V, class K, class H, class Ex, class Kq, class A>
void
hash_table<V, K, H, Ex, Kq, A>::clear()
{
    clear_buckets();
    buckets_.assign(buckets_.size(), nullptr);
}
// emplace
template <class V, class K, class H, class Ex, class Kq, class A>
template <class... Args>
mrsuyi::pair<typename hash_table<V, K, H, Ex, Kq, A>::iterator, bool>
hash_table<V, K, H, Ex, Kq, A>::emplace_unique(Args... args)
{
    reserve(size() + 1);
    node* res = new node(mrsuyi::forward<Args>(args)...);
    size_t n = bucket(extract_key_(res->val));
    for (node* cur = buckets_[n]; cur; ++cur)
        if (key_equal_(extract_key_(res->val), extract_key_(cur->val)))
        {
            delete res;
            return {iterator(this, cur), false};
        }
    res->nxt = buckets_[n];
    buckets_[n] = res;
    ++node_count_;
    return {iterator(this, res), true};
}
template <class V, class K, class H, class Ex, class Kq, class A>
template <class... Args>
mrsuyi::pair<typename hash_table<V, K, H, Ex, Kq, A>::iterator, bool>
hash_table<V, K, H, Ex, Kq, A>::emplace_equal(Args... args)
{
    reserve(size() + 1);
    node* res = new node(mrsuyi::forward<Args>(args)...);
    size_t n = bucket(extract_key_(res->val));
    for (node* cur = buckets_[n]; cur; ++cur)
    {
        if (key_equal_(extract_key_(res->val), extract_key_(cur->val)))
        {
            node* tmp = cur->nxt;
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
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::iterator
hash_table<V, K, H, Ex, Kq, A>::erase(const_iterator pos)
{
    auto res = pos;
    ++res;
    auto bucket_idx = bucket(extract_key_(pos.node_->val));
    node** mount = buckets_.data() + bucket_idx;
    node* cur = buckets_[bucket_idx];
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
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::iterator
hash_table<V, K, H, Ex, Kq, A>::erase(const_iterator first, const_iterator last)
{
    for (; first != last; first = erase(first))
        ;
}
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::erase_equal(const K& key)
{
    auto bucket_idx = bucket(key);
    node** mount = buckets_.data() + bucket_idx;
    node* cur = buckets_[bucket_idx];
    while (cur && !key_equal_(extract_key_(cur->val), key))
    {
        mount = &(cur->nxt);
        cur = cur->nxt;
    }
    size_t res = 0;
    while (cur && key_equal_(extract_key_(cur->val), key))
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
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::erase_unique(const K& key)
{
    auto bucket_idx = bucket(key);
    node** mount = buckets_.data() + bucket_idx;
    node* cur = buckets_[bucket_idx];
    while (cur && !key_equal_(extract_key_(cur->val), key))
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
// swap
template <class V, class K, class H, class Ex, class Kq, class A>
void
hash_table<V, K, H, Ex, Kq, A>::swap(hash_table& other)
{
    mrsuyi::swap(hash_, other.hash_);
    mrsuyi::swap(extract_key_, other.extract_key_);
    mrsuyi::swap(key_equal_, other.key_equal_);
    mrsuyi::swap(alloc_, other.alloc_);
    mrsuyi::swap(buckets_, other.buckets_);
    mrsuyi::swap(node_count_, other.node_count_);
    mrsuyi::swap(max_load_factor_, other.max_load_factor_);
}

//================================== lookup ==================================//
// count
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::count_equal(const K& key) const
{
    size_t res = 0;
    node* cur = buckets_[bucket(key)];
    while (cur && key_equal_(extract_key_(cur->val), key))
    {
        ++res;
        cur = cur->nxt;
    }
    return res;
}
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::count_unique(const K& key) const
{
    node* cur = buckets_[bucket(key)];
    while (cur)
    {
        if (key_equal_(extract_key_(cur->val), key))
            return 1;
        else
            cur = cur->nxt;
    }
    return 0;
}
// find
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::iterator
hash_table<V, K, H, Ex, Kq, A>::find(const K& key)
{
    return iterator(this, search(key));
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_iterator
hash_table<V, K, H, Ex, Kq, A>::find(const K& key) const
{
    return const_iterator(this, search(key));
}
template <class V, class K, class H, class Ex, class Kq, class A>
mrsuyi::pair<typename hash_table<V, K, H, Ex, Kq, A>::iterator,
             typename hash_table<V, K, H, Ex, Kq, A>::iterator>
hash_table<V, K, H, Ex, Kq, A>::equal_range(const K& key)
{
    auto p = search_range(key);
    return {iterator(this, p.first), iterator(this, p.second)};
}
template <class V, class K, class H, class Ex, class Kq, class A>
mrsuyi::pair<typename hash_table<V, K, H, Ex, Kq, A>::const_iterator,
             typename hash_table<V, K, H, Ex, Kq, A>::const_iterator>
hash_table<V, K, H, Ex, Kq, A>::equal_range(const K& key) const
{
    auto p = search_range(key);
    return {const_iterator(this, p.first), const_iterator(this, p.second)};
}

//============================== bucket interface ============================//
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::local_iterator
hash_table<V, K, H, Ex, Kq, A>::begin(size_t n) noexcept
{
    return local_iterator(buckets_[n]);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::local_iterator
    hash_table<V, K, H, Ex, Kq, A>::end(size_t) noexcept
{
    return local_iterator(nullptr);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_local_iterator
hash_table<V, K, H, Ex, Kq, A>::begin(size_t n) const noexcept
{
    return const_local_iterator(buckets_[n]);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_local_iterator
    hash_table<V, K, H, Ex, Kq, A>::end(size_t) const noexcept
{
    return local_iterator(nullptr);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_local_iterator
hash_table<V, K, H, Ex, Kq, A>::cbegin(size_t n) const noexcept
{
    return const_local_iterator(buckets_[n]);
}
template <class V, class K, class H, class Ex, class Kq, class A>
typename hash_table<V, K, H, Ex, Kq, A>::const_local_iterator
    hash_table<V, K, H, Ex, Kq, A>::cend(size_t) const noexcept
{
    return local_iterator(nullptr);
}

// bucket-count
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::bucket_count() const
{
    return buckets_.size();
}
// max-bucket-count
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::max_bucket_count() const
{
    return prime_nums[61];
}
// bucket-size
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::bucket_size(size_t n) const
{
    return mrsuyi::distance(begin(n), end(n));
}
// bucket
template <class V, class K, class H, class Ex, class Kq, class A>
size_t
hash_table<V, K, H, Ex, Kq, A>::bucket(const K& key) const
{
    return hash_(key) % buckets_.size();
}

//=============================== hash policy ================================//
template <class V, class K, class H, class Ex, class Kq, class A>
float
hash_table<V, K, H, Ex, Kq, A>::load_factor() const
{
    return size() / bucket_count();
}
template <class V, class K, class H, class Ex, class Kq, class A>
float
hash_table<V, K, H, Ex, Kq, A>::max_load_factor() const
{
    return max_load_factor_;
}
template <class V, class K, class H, class Ex, class Kq, class A>
void
hash_table<V, K, H, Ex, Kq, A>::max_load_factor(float ml)
{
    max_load_factor_ = ml;
}
template <class V, class K, class H, class Ex, class Kq, class A>
void
hash_table<V, K, H, Ex, Kq, A>::rehash(size_t count)
{
    count = max(count, size_t(size() / max_load_factor()));
    if (count <= buckets_.size()) return;
    count = *mrsuyi::lower_bound(prime_nums, prime_nums + 61, count);

    auto tmp = mrsuyi::move(buckets_);
    buckets_ = mrsuyi::vector<node*>(count);
    for (size_t i = 0; i < tmp.size(); ++i)
    {
        node* cur = tmp[i];
        while (cur)
        {
            auto new_idx = bucket(extract_key_(cur->val));
            node* nxt = cur->nxt;
            cur->nxt = buckets_[new_idx];
            buckets_[new_idx] = cur;
            cur = nxt;
        }
    }
}
template <class V, class K, class H, class Ex, class Kq, class A>
void
hash_table<V, K, H, Ex, Kq, A>::reserve(size_t count)
{
    rehash(std::ceil(count / max_load_factor()));
}
//=============================== hash policy ================================//
template <class V, class K, class H, class Ex, class Kq, class A>
H
hash_table<V, K, H, Ex, Kq, A>::hash_function() const
{
    return hash_;
}
template <class V, class K, class H, class Ex, class Kq, class A>
Kq
hash_table<V, K, H, Ex, Kq, A>::key_eq() const
{
    return key_equal_;
}
}
