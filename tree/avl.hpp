#pragma once

#include <cstdlib>
#include "container/vector.hpp"
#include "utility.hpp"

namespace mrsuyi
{
template <class T, class Compare = less<T>>
class avl
{
    struct node;

    template <class E>
    class iter;
    template <class E>
    class riter;

private:
    // min node in tree
    node* min() const;
    // max node in tree
    node* max() const;
    // find
    node* search(const T& t) const;
    // balance node till root
    void balance(node* n);
    // spin
    void spinl(node* parent);
    void spinr(node* parent);

public:
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = riter<T>;
    using const_reverse_iterator = riter<const T>;

    // ctor & dtor
    avl(const Compare& = Compare());
    ~avl();

    // element access

    // capacity
    size_t size() const;
    bool empty() const;

    // iterators
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // modifiers
    iterator insert(const T&);
    iterator insert(T&&);

    iterator erase(const T&);
    iterator erase(const_iterator);

    // lookup
    iterator find(const T&);
    const_iterator find(const T&) const;

private:
    node* root_;
    Compare cmp_;
    size_t size_;
};

template <class T, class Compare>
struct avl<T, Compare>::node
{
    node *l, *r, *parent;
    int height;
    T t;

    template <class... Args>
    node(Args... args)
        : l(nullptr),
          r(nullptr),
          parent(nullptr),
          height(0),
          t(forward<Args>(args)...)
    {
    }
};

template <class T, class Compare>
template <class E>
class avl<T, Compare>::iter
{
    friend class avl<T, Compare>;

public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = bidirectional_iterator_tag;

    iter() : node_(nullptr) {}
    iter(node* ptr) : node_(ptr) {}
    iter(const iter& it) : node_(it.node_) {}
    iter& operator=(const iter& it) { node_ = it.node_; }
    E& operator*() const { return node_->t; }
    E* operator->() const { return &(node_->t); }
    bool operator==(const iter& it) { return node_ == it.node_; }
    bool operator!=(const iter& it) { return node_ != it.node_; }
    iter& operator++()
    {
        if (node_->r)
        {
            node_ = node_->r;
            while (node_->l) node_ = node_->l;
        }
        else
        {
            auto old = node_;
            do
            {
                old = node_;
                node_ = node_->parent;
            } while (node_ && node_->r == old);
        }
    }
    iter operator++(int)
    {
        auto res = *this;
        ++*this;
        return res;
    }
    iter& operator--()
    {
        if (node_->l)
        {
            node_ = node_->l;
            while (node_->r) node_ = node_->r;
        }
        else
        {
            auto old = node_;
            do
            {
                old = node_;
                node_ = node_->parent;
            } while (node_ && node_->l == old);
        }
    }
    iter operator--(int)
    {
        auto res = *this;
        --*this;
        return res;
    }
    operator iter<const E>() const { return iter<const E>(node_); }
protected:
    node* node_;
};

template <class T, class Compare>
template <class E>
class avl<T, Compare>::riter : public iter<E>
{
    friend class avl<T, Compare>;

public:
    riter() : iter<E>() {}
    riter(node* ptr) : iter<E>(ptr) {}
    riter(const riter& it) : iter<E>(it) {}
    riter& operator++() { iter<E>::operator--(); }
    riter& operator--() { iter<E>::operator++(); }
    operator riter<const E>() const { return riter<const E>(iter<E>::node_); }
};

//============================== private =================================//
template <class T, class Compare>
typename avl<T, Compare>::node*
avl<T, Compare>::min() const
{
    if (!root_) return nullptr;
    node* res = root_;
    while (res->l) res = res->l;
    return res;
}
template <class T, class Compare>
typename avl<T, Compare>::node*
avl<T, Compare>::max() const
{
    if (!root_) return nullptr;
    node* res = root_;
    while (res->r) res = res->r;
    return res;
}
template <class T, class Compare>
typename avl<T, Compare>::node*
avl<T, Compare>::search(const T& t) const
{
    for (node* n = root_; n;)
    {
        if (cmp_(n->t, t))
            n = n->r;
        else if (cmp_(t, n->t))
            n = n->l;
        else
            return n;
    }
    return nullptr;
}
template <class T, class Compare>
void
avl<T, Compare>::balance(node* n)
{
    while (n)
    {
        node* nl = n->l;
        node* nr = n->r;
        int hl = nl ? nl->height : -1;
        int hr = nr ? nr->height : -1;

        if (abs(hl - hr) > 1)
        {
            node* parent = n->parent;
            node** mount = parent
                               ? (parent->l == n ? &(parent->l) : &(parent->r))
                               : nullptr;

            if (hl > hr)
            {
                int hll = nl->l ? nl->l->height : -1;
                int hlr = nl->r ? nl->r->height : -1;

                if (hll > hlr)
                // single spin
                {
                    spinl(n);
                    if (mount) *mount = nl;
                }
                else
                // double spin
                {
                    spinr(nl);
                    spinl(n);
                    if (mount) *mount = nl->r;
                }
            }
            else
            {
                int hrl = nr->l ? nr->l->height : -1;
                int hrr = nr->r ? nr->r->height : -1;

                if (hrr > hrl)
                {
                    spinr(n);
                    if (mount) *mount = nr;
                }
                else
                {
                    spinl(nr);
                    spinr(n);
                    if (mount) *mount = nr->l;
                }
            }
            n = parent;
        }
        else
        {
            if (hl >= n->height)
            {
                n->height = hl + 1;        
                n = n->parent;
            }
            else if (hr >= n->height)
            {
                n->height = hr + 1;
                n = n->parent;
            }
            else
                break;
        }
    }
}
// spin-left (parent & right-child)
template <class T, class Compare>
void
avl<T, Compare>::spinl(node* parent)
{
    node* child = parent->l;
    //
    parent->l = child->r;
    if (child->r) child->r->parent = parent;
    //
    child->r = parent;
    parent->parent = child;
    //
    --parent->height;
}
// spin-right (parent & left-child)
template <class T, class Compare>
void
avl<T, Compare>::spinr(node* parent)
{
    node* child = parent->r;
    //
    parent->r = child->l;
    if (child->l) child->l->parent = parent;
    //
    child->l = parent;
    parent->parent = child;
    //
    --parent->height;
}

//============================== ctor & dtor =================================//
template <class T, class Compare>
avl<T, Compare>::avl(const Compare& cmp)
    : root_(nullptr), cmp_(cmp), size_(0)
{
}
template <class T, class Compare>
avl<T, Compare>::~avl()
{
    vector<node*> dels;
    dels.reserve(size_ / 2);
    if (root_) dels.push_back(root_);

    while (!dels.empty())
    {
        node* n = dels.back();
        dels.pop_back();

        if (n->l) dels.push_back(n->l);
        if (n->r) dels.push_back(n->r);

        delete n;
    }
}

//================================ capacity ==================================//
template <class T, class Compare>
size_t
avl<T, Compare>::size() const
{
    return size_;
}

template <class T, class Compare>
bool
avl<T, Compare>::empty() const
{
    return size() == 0;
}

//================================ modifiers =================================//
// insert
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::insert(const T& t)
{
    insert(T(t));
}
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::insert(T&& t)
{
    node* parent = nullptr;
    node** mount = &root_;

    while (*mount)
    {
        parent = *mount;

        if (cmp_(t, (*mount)->t))
            mount = &((*mount)->l);
        else
            mount = &((*mount)->r);
    }

    if (!parent || cmp_(t, parent->t) || cmp_(parent->t, t))
    {
        *mount = new node(move(t));
        (*mount)->parent = parent;
        ++size_;
    }
}
// erase
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::erase(const T& t)
{
}
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::erase(const_iterator it)
{
}

//================================= iterators ================================//
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::begin() noexcept
{
    return iterator(min());
}

template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::end() noexcept
{
    return iterator(nullptr);
}

template <class T, class Compare>
typename avl<T, Compare>::const_iterator
avl<T, Compare>::begin() const noexcept
{
    return const_iterator(min());
}

template <class T, class Compare>
typename avl<T, Compare>::const_iterator
avl<T, Compare>::end() const noexcept
{
    return const_iterator(nullptr);
}

template <class T, class Compare>
typename avl<T, Compare>::const_iterator
avl<T, Compare>::cbegin() const noexcept
{
    return const_iterator(min());
}

template <class T, class Compare>
typename avl<T, Compare>::const_iterator
avl<T, Compare>::cend() const noexcept
{
    return const_iterator(nullptr);
}

template <class T, class Compare>
typename avl<T, Compare>::reverse_iterator
avl<T, Compare>::rbegin() noexcept
{
    return reverse_iterator(max());
}

template <class T, class Compare>
typename avl<T, Compare>::reverse_iterator
avl<T, Compare>::rend() noexcept
{
    return reverse_iterator(nullptr);
}

template <class T, class Compare>
typename avl<T, Compare>::const_reverse_iterator
avl<T, Compare>::rbegin() const noexcept
{
    return const_reverse_iterator(max());
}

template <class T, class Compare>
typename avl<T, Compare>::const_reverse_iterator
avl<T, Compare>::rend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

template <class T, class Compare>
typename avl<T, Compare>::const_reverse_iterator
avl<T, Compare>::crbegin() const noexcept
{
    return const_reverse_iterator(max());
}

template <class T, class Compare>
typename avl<T, Compare>::const_reverse_iterator
avl<T, Compare>::crend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

//================================= lookup ================================//
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::find(const T& t)
{
    return iterator(search(t));
}
template <class T, class Compare>
typename avl<T, Compare>::const_iterator
avl<T, Compare>::find(const T& t) const
{
    return const_iterator(search(t));
}
}
