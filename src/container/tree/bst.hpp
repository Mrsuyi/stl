#pragma once

#include <cstddef>
#include <initializer_list>
#include "container/vector.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "string.hpp"

namespace mrsuyi
{
template <class T>
struct bst_node
{
    bst_node *l, *r, *parent;
    T t;

    template <class... Args>
    bst_node(Args... args)
        : l(nullptr), r(nullptr), parent(nullptr), t(forward<Args>(args)...)
    {
    }
};

template <class T, class Node = bst_node<T>, class Compare = mrsuyi::less<T>>
class bst
{
protected:
    template <class E>
    class iter;
    template <class E>
    class riter;

    // find the proper insert position
    bool insert_pos(const T& t, Node**& _mount, Node*& _parent) const;
    // find the mount-point of [node]
    Node** mount_pos(Node* node);
    // min node in tree of [root]
    Node* min(Node* root) const;
    // max node in tree of [root]
    Node* max(Node* root) const;
    // find
    Node* search(const T& t) const;
    // replace
    void replace(Node* cur, Node* tar);

public:
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = riter<T>;
    using const_reverse_iterator = riter<const T>;

    // ctor & dtor
    // default
    bst(const Compare& cmp = Compare());
    // range
    template <class InputIt>
    bst(InputIt first, InputIt last, const Compare& cmp = Compare());
    // initializer_list
    bst(std::initializer_list<T>, const Compare& = Compare());
    // dtor
    ~bst();

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
    pair<iterator, bool> insert(const T&);
    pair<iterator, bool> insert(T&&);
    iterator erase(iterator);
    size_t erase(const T&);

    // lookup
    iterator find(const T&);
    const_iterator find(const T&) const;

    // mrsuyi-special-functions :D
    // generate a tree-graph for console-print
    string graph(string (*)(T)) const;

public:
    // protected:
    Node* root_;
    Compare cmp_;
    size_t size_;
};

//============================== iter & riter ================================//
template <class T, class Node, class Compare>
template <class E>
class bst<T, Node, Compare>::iter
{
    friend class bst<T, Node, Compare>;

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
public:
    Node* node_;
};

template <class T, class Node, class Compare>
template <class E>
class bst<T, Node, Compare>::riter : public iter<E>
{
    friend class bst<T, Node, Compare>;

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
bool
bst<T, Node, Compare>::insert_pos(const T& t, Node**& _mount,
                                  Node*& _parent) const
{
    while (*_mount)
    {
        _parent = *_mount;
        if (cmp_((*_mount)->t, t))
            _mount = &((*_mount)->r);
        else if (cmp_(t, (*_mount)->t))
            _mount = &((*_mount)->l);
        else
            return false;
    }
    return true;
}
template <class T, class Node, class Compare>
Node**
bst<T, Node, Compare>::mount_pos(Node* n)
{
    Node** res = &root_;
    if (n->parent)
        res = (n->parent->l == n) ? &(n->parent->l) : &(n->parent->r);
    return res;
}
template <class T, class Node, class Compare>
Node*
bst<T, Node, Compare>::min(Node* root) const
{
    if (!root) return nullptr;
    while (root->l) root = root->l;
    return root;
}
template <class T, class Node, class Compare>
Node*
bst<T, Node, Compare>::max(Node* root) const
{
    if (!root) return nullptr;
    while (root->r) root = root->r;
    return root;
}
template <class T, class Node, class Compare>
void
bst<T, Node, Compare>::replace(Node* cur, Node* tar)
{
    *mount_pos(cur) = tar;
    tar->parent = cur->parent;
    tar->l = cur->l;
    if (tar->l) tar->l->parent = tar;
    tar->r = cur->r;
    if (tar->r) tar->r->parent = tar;
}
template <class T, class Node, class Compare>
Node*
bst<T, Node, Compare>::search(const T& t) const
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

//============================== ctor & dtor =================================//
// default
template <class T, class Node, class Compare>
bst<T, Node, Compare>::bst(const Compare& cmp)
    : root_(nullptr), cmp_(cmp), size_(0)
{
}
// range
template <class T, class Node, class Compare>
template <class InputIt>
bst<T, Node, Compare>::bst(InputIt first, InputIt last, const Compare& cmp)
    : root_(nullptr), size_(0), cmp_(cmp)
{
    for (; first != last; ++first) insert(*first);
}
// initializer_list
template <class T, class Node, class Compare>
bst<T, Node, Compare>::bst(std::initializer_list<T> il, const Compare& cmp)
    : bst(il.begin(), il.end(), cmp)
{
}
// dtor
template <class T, class Node, class Compare>
bst<T, Node, Compare>::~bst()
{
    vector<Node*> dels;
    dels.reserve(size_ / 2);
    if (root_) dels.push_back(root_);

    while (!dels.empty())
    {
        Node* n = dels.back();
        dels.pop_back();

        if (n->l) dels.push_back(n->l);
        if (n->r) dels.push_back(n->r);

        delete n;
    }
}

//================================ capacity ==================================//
template <class T, class Node, class Compare>
size_t
bst<T, Node, Compare>::size() const
{
    return size_;
}

template <class T, class Node, class Compare>
bool
bst<T, Node, Compare>::empty() const
{
    return size() == 0;
}

//================================= iterators ================================//
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::iterator
bst<T, Node, Compare>::begin() noexcept
{
    return iterator(min(root_));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::iterator
bst<T, Node, Compare>::end() noexcept
{
    return iterator(nullptr);
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_iterator
bst<T, Node, Compare>::begin() const noexcept
{
    return const_iterator(min(root_));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_iterator
bst<T, Node, Compare>::end() const noexcept
{
    return const_iterator(nullptr);
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_iterator
bst<T, Node, Compare>::cbegin() const noexcept
{
    return const_iterator(min(root_));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_iterator
bst<T, Node, Compare>::cend() const noexcept
{
    return const_iterator(nullptr);
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::reverse_iterator
bst<T, Node, Compare>::rbegin() noexcept
{
    return reverse_iterator(max(root_));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::reverse_iterator
bst<T, Node, Compare>::rend() noexcept
{
    return reverse_iterator(nullptr);
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_reverse_iterator
bst<T, Node, Compare>::rbegin() const noexcept
{
    return const_reverse_iterator(max(root_));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_reverse_iterator
bst<T, Node, Compare>::rend() const noexcept
{
    return const_reverse_iterator(nullptr);
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_reverse_iterator
bst<T, Node, Compare>::crbegin() const noexcept
{
    return const_reverse_iterator(max(root_));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_reverse_iterator
bst<T, Node, Compare>::crend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

//================================== lookup ==================================//
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::iterator
bst<T, Node, Compare>::find(const T& t)
{
    return iterator(search(t));
}
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::const_iterator
bst<T, Node, Compare>::find(const T& t) const
{
    return const_iterator(search(t));
}

//=============================== modifiers ==================================//
template <class T, class Node, class Compare>
pair<typename bst<T, Node, Compare>::iterator, bool>
bst<T, Node, Compare>::insert(const T& t)
{
    insert(mrsuyi::move(T(t)));
}
template <class T, class Node, class Compare>
pair<typename bst<T, Node, Compare>::iterator, bool>
bst<T, Node, Compare>::insert(T&& t)
{
    Node** mount = &root_;
    Node* parent = nullptr;
    if (insert_pos(t, mount, parent))
    {
        *mount = new Node(mrsuyi::move(t));
        (*mount)->parent = parent;
        ++size_;
        return {*mount, true};
    }
    else
        return {*mount, false};
}

// erase
template <class T, class Node, class Compare>
typename bst<T, Node, Compare>::iterator
bst<T, Node, Compare>::erase(iterator it)
{
    if (it.node_->l)
    {
        auto n = max(it.node_->l);
        *mount_pos(n) = n->l;
        if (n->l) n->l->parent = n->parent;
        replace(it.node_, n);
    }
    else if (it.node_->r)
    {
        auto n = min(it.node_->r);
        *mount_pos(n) = n->r;
        if (n->r) n->r->parent = n->parent;
        replace(it.node_, n);
    }
    else
    {
        *mount_pos(it.node_) = nullptr;
    }
    delete it.node_;
    --size_;
}
template <class T, class Node, class Compare>
size_t
bst<T, Node, Compare>::erase(const T& t)
{
    auto it = find(t);
    if (it != end())
    {
        erase(it);
        return 1;
    }
    return 0;
}

//======================== mrsuyi-special-functions :D =======================//
template <class T, class Node, class Compare>
string bst<T, Node, Compare>::graph(string (*to_string)(T)) const
{
    vector<Node*> nodes;
    vector<vector<string>> vals;

    if (root_) nodes.push_back(root_);

    while (!nodes.empty())
    {
        vector<Node*> tmp;
        vals.push_back(vector<string>());

        for (auto& node : nodes)
        {
            vals.back().push_back(to_string(node->t));
            if (node->l) tmp.push_back(node->l);
            if (node->r) tmp.push_back(node->r);
        }
        swap(nodes, tmp);
    }

    string res;

    for (auto& row : vals)
    {
        string line = "";
        for (auto& str : row)
        {
            line += str + "  ";
        }
        res += line + '\n';
    }

    return res;
}
}
