#pragma once

#include <cstddef>
#include <initializer_list>
#include "container/vector.hpp"
#include "debug.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "string.hpp"

namespace mrsuyi
{
template <class Key>
struct bst_node
{
    bst_node *l, *r, *parent;
    Key key;

    template <class... Args>
    bst_node(Args... args)
        : l(nullptr), r(nullptr), parent(nullptr), key(forward<Args>(args)...)
    {
    }
};

template <class Key, class Node = bst_node<Key>,
          class Compare = mrsuyi::less<Key>>
class bst
{
protected:
    template <class E>
    class iter;
    template <class E>
    class riter;

    // find the proper insert position
    bool insert_pos(const Key& key, Node**& _mount, Node*& _parent) const;
    // find the mount-point of [node]
    Node** mount_pos(Node* node) const;
    // min node in tree of [root]
    Node* min(Node* root) const;
    // max node in tree of [root]
    Node* max(Node* root) const;
    // find
    Node* search(const Key& key) const;
    // replace
    void replace(Node* cur, Node* tar);
    // height
    int height(Node* n) const;

public:
    using iterator = iter<Key>;
    using const_iterator = iter<const Key>;
    using reverse_iterator = riter<Key>;
    using const_reverse_iterator = riter<const Key>;

    // ctor & dtor
    // default
    bst(const Compare& cmp);
    bst(Compare&& cmp = Compare());
    // range
    template <class InputIter>
    bst(InputIter first, InputIter last, const Compare& cmp = Compare());
    // initializer_list
    bst(std::initializer_list<Key>, const Compare& = Compare());

    virtual ~bst();

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
    pair<iterator, bool> insert(const Key&);
    pair<iterator, bool> insert(Key&&);
    iterator erase(iterator);
    size_t erase(const Key&);

    // lookup
    iterator find(const Key&);
    const_iterator find(const Key&) const;

    // mrsuyi-special-functions :D
    // height recorded by root
    int height() const;
    // height calculated recursively
    int real_height() const;
    iterator root() const;
    // generate a tree-graph for console-print
    string graph(string (*)(Key)) const;

protected:
    Node* root_;
    Compare cmp_;
    size_t size_;
};

template <class Key, class Node, class Compare>
template <class E>
class bst<Key, Node, Compare>::iter
{
    friend class bst<Key, Node, Compare>;

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
    E& operator*() const { return node_->key; }
    E* operator->() const { return &(node_->key); }
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

template <class Key, class Node, class Compare>
template <class E>
class bst<Key, Node, Compare>::riter : public iter<E>
{
    friend class bst<Key, Node, Compare>;

public:
    riter() : iter<E>() {}
    riter(Node* ptr) : iter<E>(ptr) {}
    riter(const riter& it) : iter<E>(it) {}
    riter& operator++() { iter<E>::operator--(); }
    riter& operator--() { iter<E>::operator++(); }
    operator riter<const E>() const { return riter<const E>(iter<E>::node_); }
};

//=============================== protected ==================================//
template <class Key, class Node, class Compare>
bool
bst<Key, Node, Compare>::insert_pos(const Key& key, Node**& _mount,
                                    Node*& _parent) const
{
    while (*_mount)
    {
        _parent = *_mount;
        if (cmp_((*_mount)->key, key))
            _mount = &((*_mount)->r);
        else if (cmp_(key, (*_mount)->key))
            _mount = &((*_mount)->l);
        else
            return false;
    }
    return true;
}
template <class Key, class Node, class Compare>
Node**
bst<Key, Node, Compare>::mount_pos(Node* n) const
{
    Node** res = nullptr;
    if (n->parent) res = (n->parent->l == n) ? &(n->parent->l) : &(n->parent->r);
    return res;
}
template <class Key, class Node, class Compare>
Node*
bst<Key, Node, Compare>::min(Node* root) const
{
    if (!root) return nullptr;
    while (root->l) root = root->l;
    return root;
}
template <class Key, class Node, class Compare>
Node*
bst<Key, Node, Compare>::max(Node* root) const
{
    if (!root) return nullptr;
    while (root->r) root = root->r;
    return root;
}
template <class Key, class Node, class Compare>
void
bst<Key, Node, Compare>::replace(Node* cur, Node* tar)
{
    if (auto mount = mount_pos(cur)) *mount = tar;
    tar->parent = cur->parent;
    tar->l = cur->l;
    if (tar->l) tar->l->parent = tar;
    tar->r = cur->r;
    if (tar->r) tar->r->parent = tar;
}
template <class Key, class Node, class Compare>
Node*
bst<Key, Node, Compare>::search(const Key& key) const
{
    for (Node* n = root_; n;)
    {
        if (cmp_(n->key, key))
            n = n->r;
        else if (cmp_(key, n->key))
            n = n->l;
        else
            return n;
    }
    return nullptr;
}
template <class Key, class Node, class Compare>
int
bst<Key, Node, Compare>::height(Node* n) const
{
    if (!n) return 0;
    return max(height(n->l), height(n->r)) + 1;
}

//============================== ctor & dtor =================================//
// default
template <class Key, class Node, class Compare>
bst<Key, Node, Compare>::bst(const Compare& cmp)
    : root_(nullptr), cmp_(cmp), size_(0)
{
}
template <class Key, class Node, class Compare>
bst<Key, Node, Compare>::bst(Compare&& cmp)
    : root_(nullptr), cmp_(mrsuyi::move(cmp)), size_(0)
{
}
// range
template <class Key, class Node, class Compare>
template <class InputIter>
bst<Key, Node, Compare>::bst(InputIter first, InputIter last,
                             const Compare& cmp)
    : root_(nullptr), size_(0), cmp_(cmp)
{
    for (; first != last; ++first) insert(*first);
}
// initializer_list
template <class Key, class Node, class Compare>
bst<Key, Node, Compare>::bst(std::initializer_list<Key> il, const Compare& cmp)
    : bst(il.begin(), il.end(), cmp)
{
}
// dtor
template <class Key, class Node, class Compare>
bst<Key, Node, Compare>::~bst()
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
template <class Key, class Node, class Compare>
size_t
bst<Key, Node, Compare>::size() const
{
    return size_;
}

template <class Key, class Node, class Compare>
bool
bst<Key, Node, Compare>::empty() const
{
    return size() == 0;
}

//================================= iterators ================================//
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::iterator
bst<Key, Node, Compare>::begin() noexcept
{
    return iterator(min(root_));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::iterator
bst<Key, Node, Compare>::end() noexcept
{
    return iterator(nullptr);
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_iterator
bst<Key, Node, Compare>::begin() const noexcept
{
    return const_iterator(min(root_));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_iterator
bst<Key, Node, Compare>::end() const noexcept
{
    return const_iterator(nullptr);
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_iterator
bst<Key, Node, Compare>::cbegin() const noexcept
{
    return const_iterator(min(root_));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_iterator
bst<Key, Node, Compare>::cend() const noexcept
{
    return const_iterator(nullptr);
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::reverse_iterator
bst<Key, Node, Compare>::rbegin() noexcept
{
    return reverse_iterator(max(root_));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::reverse_iterator
bst<Key, Node, Compare>::rend() noexcept
{
    return reverse_iterator(nullptr);
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_reverse_iterator
bst<Key, Node, Compare>::rbegin() const noexcept
{
    return const_reverse_iterator(max(root_));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_reverse_iterator
bst<Key, Node, Compare>::rend() const noexcept
{
    return const_reverse_iterator(nullptr);
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_reverse_iterator
bst<Key, Node, Compare>::crbegin() const noexcept
{
    return const_reverse_iterator(max(root_));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_reverse_iterator
bst<Key, Node, Compare>::crend() const noexcept
{
    return const_reverse_iterator(nullptr);
}

//================================== lookup ==================================//
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::iterator
bst<Key, Node, Compare>::find(const Key& key)
{
    return iterator(search(key));
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::const_iterator
bst<Key, Node, Compare>::find(const Key& key) const
{
    return const_iterator(search(key));
}

//=============================== modifiers ==================================//
template <class Key, class Node, class Compare>
pair<typename bst<Key, Node, Compare>::iterator, bool>
bst<Key, Node, Compare>::insert(const Key& key)
{
    Node** mount = &root_;
    Node* parent = nullptr;
    if (insert_pos(key, mount, parent))
    {
        *mount = new Node(key);
        (*mount)->parent = parent;
        ++size_;
        return {*mount, true};
    }
    else
        return {*mount, false};
}
template <class Key, class Node, class Compare>
pair<typename bst<Key, Node, Compare>::iterator, bool>
bst<Key, Node, Compare>::insert(Key&& key)
{
    Node** mount = &root_;
    Node* parent = nullptr;
    if (insert_pos(key, mount, parent))
    {
        *mount = new Node(mrsuyi::move(key));
        (*mount)->parent = parent;
        ++size_;
        return {*mount, true};
    }
    else
        return {*mount, false};
}

// erase
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::iterator
bst<Key, Node, Compare>::erase(iterator it)
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
        if (auto mount = mount_pos(it.node_)) *mount = nullptr;
    }
    delete it.node_;
    --size_;
}
template <class Key, class Node, class Compare>
size_t
bst<Key, Node, Compare>::erase(const Key& key)
{
    auto it = find(key);
    if (it != end())
    {
        erase(it);
        return 1;
    }
    return 0;
}

//======================== mrsuyi-special-functions :D =======================//
template <class Key, class Node, class Compare>
int
bst<Key, Node, Compare>::height() const
{
    return root_->height;
}
template <class Key, class Node, class Compare>
int
bst<Key, Node, Compare>::real_height() const
{
    return height(root_);
}
template <class Key, class Node, class Compare>
typename bst<Key, Node, Compare>::iterator
bst<Key, Node, Compare>::root() const
{
    return iterator(root_);
}
template <class Key, class Node, class Compare>
string bst<Key, Node, Compare>::graph(string (*to_string)(Key)) const
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
            vals.back().push_back(to_string(node->key));
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
