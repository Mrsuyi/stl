#pragma once

#include "iterator.hpp"
#include "utility.hpp"
#include "container/vector.hpp"

namespace mrsuyi
{
template <class T, class Compare>
class avl_tree
{
    struct node;

    template <class E>
    class iter;

public:
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

    // ctor & dtor
    avl_tree(const Compare& = Compare());
    ~avl_tree();

    // modifiers
    void insert(const T&);
    void insert(T&&);
    void erase(const const_iterator&);

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

private:
    node* root_;
    Compare cmp_;
    size_t size_;
};

template <class T, class Compare>
struct avl_tree<T, Compare>::node
{
    node *l, *r, *parent;
    int height;
    T t;

    template <class... Args>
    node(Args... args) : t(forward<Args>(args)...)
    {
    }
};

template <class T, class Compare>
template <class E>
class avl_tree<T, Compare>::iter
{
    friend class avl_tree<T, Compare>;

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
private:
    node* node_;
};

// ctor & dtor
template <class T, class Compare>
avl_tree<T, Compare>::avl_tree(const Compare& cmp) : root_(nullptr), cmp_(cmp)
{
}
template <class T, class Compare>
avl_tree<T, Compare>::~avl_tree()
{
    vector<node*> dels;
    dels.reserve(size_);
    if (root_) dels.push_back(root_);

    while (!dels.empty())
    {
        node* n = dels.back();
        dels.pop();

        if (n->l) dels.push_back(n->l);
        if (n->r) dels.push_back(n->r);

        delete n;
    }
}

// modifiers
template <class T, class Compare>
void
avl_tree<T, Compare>::insert(const T& t)
{
    node* n = root_;
}

// iterators
template <class T, class Compare>
typename avl_tree<T, Compare>::iterator
avl_tree<T, Compare>::begin() noexcept
{
    return iterator(root_);
}

template <class T, class Compare>
typename avl_tree<T, Compare>::iterator
avl_tree<T, Compare>::end() noexcept
{
    return iterator(nullptr);
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_iterator
avl_tree<T, Compare>::begin() const noexcept
{
    return const_iterator(root_);
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_iterator
avl_tree<T, Compare>::end() const noexcept
{
    return const_iterator(nullptr);
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_iterator
avl_tree<T, Compare>::cbegin() const noexcept
{
    return const_iterator(root_);
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_iterator
avl_tree<T, Compare>::cend() const noexcept
{
    return const_iterator(nullptr);
}

template <class T, class Compare>
typename avl_tree<T, Compare>::reverse_iterator
avl_tree<T, Compare>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <class T, class Compare>
typename avl_tree<T, Compare>::reverse_iterator
avl_tree<T, Compare>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_reverse_iterator
avl_tree<T, Compare>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_reverse_iterator
avl_tree<T, Compare>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_reverse_iterator
avl_tree<T, Compare>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, class Compare>
typename avl_tree<T, Compare>::const_reverse_iterator
avl_tree<T, Compare>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}
}
