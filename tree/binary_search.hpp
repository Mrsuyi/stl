#pragma once

#include <cstddef>
#include "iterator.hpp"

namespace mrsuyi
{
template <class T, class Node, class Compare>
class binary_search
{
    template <class E>
    class iter;
    template <class E>
    class riter;

protected:
    // min node in tree
    Node* min() const;
    // max node in tree
    Node* max() const;
    // find
    Node* search(const T& t) const;

public:
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = riter<T>;
    using const_reverse_iterator = riter<const T>;

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

    // lookup
    iterator find(const T&);
    const_iterator find(const T&) const;

protected:
    Node* root_;
    Compare cmp_;
    size_t size_;
};

template <class T, class Node, class Compare>
template <class E>
class binary_search<T, Node, Compare>::iter
{
    friend class binary_search<T, Node, Compare>;

public:
    using value_type = E;
    using difference_type = size_t;
    using reference = E&;
    using pointer = E*;
    using iterator_category = bidirectional_iterator_tag;

    iter() : node_(nullptr) {}
    iter(Node* ptr) : node_(ptr) {}
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
    Node* node_;
};

template <class T, class Node, class Compare>
template <class E>
class binary_search<T, Node, Compare>::riter : public iter<E>
{
    friend class binary_search<T, Node, Compare>;

public:
    riter() : iter<E>() {}
    riter(Node* ptr) : iter<E>(ptr) {}
    riter(const riter& it) : iter<E>(it) {}
    riter& operator++() { iter<E>::operator--(); }
    riter& operator--() { iter<E>::operator++(); }
    operator riter<const E>() const { return riter<const E>(iter<E>::node_); }
};

//=============================== protected ==================================//
template <class T, class Node, class Compare>
Node*
binary_search<T, Node, Compare>::min() const
{
    if (!root_) return nullptr;
    Node* res = root_;
    while (res->l) res = res->l;
    return res;
}
template <class T, class Node, class Compare>
Node*
binary_search<T, Node, Compare>::max() const
{
    if (!root_) return nullptr;
    Node* res = root_;
    while (res->r) res = res->r;
    return res;
}
template <class T, class Node, class Compare>
Node*
binary_search<T, Node, Compare>::search(const T& t) const
{
    for (Node* n = root_; n;)
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

//================================= iterators ================================//
template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::iterator
binary_search<T, Node, Compare>::begin() noexcept
{
    return iterator(min());
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::iterator
binary_search<T, Node, Compare>::end() noexcept
{
    return iterator(nullptr);
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_iterator
binary_search<T, Node, Compare>::begin() const noexcept
{
    return const_iterator(min());
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_iterator
binary_search<T, Node, Compare>::end() const noexcept
{
    return const_iterator(nullptr);
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_iterator
binary_search<T, Node, Compare>::cbegin() const noexcept
{
    return const_iterator(min());
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_iterator
binary_search<T, Node, Compare>::cend() const noexcept
{
    return const_iterator(nullptr);
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::reverse_iterator
binary_search<T, Node, Compare>::rbegin() noexcept
{
    return reverse_iterator(max());
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::reverse_iterator
binary_search<T, Node, Compare>::rend() noexcept
{
    return reverse_iterator(nullptr);
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_reverse_iterator
binary_search<T, Node, Compare>::rbegin() const noexcept
{
    return const_reverse_iterator(max());
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_reverse_iterator
binary_search<T, Node, Compare>::rend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_reverse_iterator
binary_search<T, Node, Compare>::crbegin() const noexcept
{
    return const_reverse_iterator(max());
}

template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_reverse_iterator
binary_search<T, Node, Compare>::crend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

//================================= lookup ================================//
template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::iterator
binary_search<T, Node, Compare>::find(const T& t)
{
    return iterator(search(t));
}
template <class T, class Node, class Compare>
typename binary_search<T, Node, Compare>::const_iterator
binary_search<T, Node, Compare>::find(const T& t) const
{
    return const_iterator(search(t));
}
}
