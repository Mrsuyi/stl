#pragma once

#include <cstdlib>
#include <functional>
#include "bst.hpp"
#include "container/vector.hpp"
#include "debug.hpp"
#include "utility.hpp"

namespace mrsuyi
{
template <class Key>
struct avl_node
{
    avl_node *l, *r, *parent;
    int height;
    Key key;

    template <class... Args>
    avl_node(Args... args)
        : l(nullptr),
          r(nullptr),
          parent(nullptr),
          height(0),
          key(forward<Args>(args)...)
    {
    }
};

template <class Key, class Compare = mrsuyi::less<Key>>
class avl : public bst<Key, avl_node<Key>, Compare>
{
protected:
    using node = avl_node<Key>;
    using bst_t = bst<Key, avl_node<Key>, Compare>;

    // get height
    int height(node* n);
    // balance node till root
    void balance(node* n);
    // spin
    void spinl(node* old_root);
    void spinr(node* old_root);

public:
    using iterator = typename bst_t::iterator;
    using const_iterator = typename bst_t::const_iterator;
    using reverse_iterator = typename bst_t::reverse_iterator;
    using const_reverse_iterator = typename bst_t::const_reverse_iterator;

    // ctor & dtor
    // default
    avl(const Compare& = Compare());
    // range
    template <class InputIter>
    avl(InputIter first, InputIter last, const Compare& cmp = Compare());
    // initializer_list
    avl(std::initializer_list<Key>, const Compare& = Compare());
    // dtor
    virtual ~avl() override;

    // element access

    // modifiers
    pair<iterator, bool> insert(const Key&);
    pair<iterator, bool> insert(Key&&);
    iterator erase(iterator);
    size_t erase(const Key&);

    // mrsuyi's special functions :D
    bool height_check() const;
};

//=============================== protected ==================================//
// reset height
template <class Key, class Compare>
int
avl<Key, Compare>::height(node* n)
{
    int hl = n->l ? n->l->height : -1;
    int hr = n->r ? n->r->height : -1;
    return max(hl, hr) + 1;
}
// balance
template <class Key, class Compare>
void
avl<Key, Compare>::balance(node* n)
{
    while (n)
    {
        node* nl = n->l;
        node* nr = n->r;
        int hl = nl ? nl->height : -1;
        int hr = nr ? nr->height : -1;
        int h = max(hl, hr) + 1;

        if (abs(hl - hr) > 1)
        {
            if (hl > hr)
            {
                int hll = nl->l ? nl->l->height : -1;
                int hlr = nl->r ? nl->r->height : -1;

                if (hll > hlr)
                {
                    spinl(n);
                }
                else
                {
                    //++nl->r->height;
                    //--nl->height;
                    spinr(nl);
                    spinl(n);
                    nl->r->height = height(nl->r);
                    nl->height = height(nl);
                }
            }
            else
            {
                int hrl = nr->l ? nr->l->height : -1;
                int hrr = nr->r ? nr->r->height : -1;

                if (hrr > hrl)
                {
                    spinr(n);
                }
                else
                {
                    //++nr->l->height;
                    //--nr->height;
                    spinl(nr);
                    spinr(n);
                    nr->l->height = height(nr->l);
                    nr->height = height(nr);
                }
            }
            n->height = h - 2;
            return;
        }
        else
        {
            if (h != n->height)
            {
                n->height = h;
                n = n->parent;
            }
            else
                return;
        }
    }
}
// spin-left (parent & right-child)
template <class Key, class Compare>
void
avl<Key, Compare>::spinl(node* old_root)
{
    node* new_root = old_root->l;
    //
    if (auto mount = bst_t::mount_pos(old_root)) *mount = new_root;
    new_root->parent = old_root->parent;
    //
    old_root->l = new_root->r;
    if (new_root->r) new_root->r->parent = old_root;
    //
    new_root->r = old_root;
    old_root->parent = new_root;
}
// spin-right (parent & left-child)
template <class Key, class Compare>
void
avl<Key, Compare>::spinr(node* old_root)
{
    node* new_root = old_root->r;
    //
    if (auto mount = bst_t::mount_pos(old_root)) *mount = new_root;
    new_root->parent = old_root->parent;
    //
    old_root->r = new_root->l;
    if (new_root->l) new_root->l->parent = old_root;
    //
    new_root->l = old_root;
    old_root->parent = new_root;
}

//============================== ctor & dtor =================================//
// default
template <class Key, class Compare>
avl<Key, Compare>::avl(const Compare& cmp) : bst_t(cmp)
{
}
// range
template <class Key, class Compare>
template <class InputIt>
avl<Key, Compare>::avl(InputIt first, InputIt last, const Compare& cmp)
    : bst_t(cmp)
{
    for (; first != last; ++first) insert(*first);
}
// range
template <class Key, class Compare>
avl<Key, Compare>::avl(std::initializer_list<Key> il, const Compare& cmp)
    : avl(il.begin(), il.end(), cmp)
{
}
// dtor
template <class Key, class Compare>
avl<Key, Compare>::~avl()
{
}

//================================ modifiers =================================//
// insert
template <class Key, class Compare>
pair<typename avl<Key, Compare>::iterator, bool>
avl<Key, Compare>::insert(const Key& key)
{
    insert(move(Key(key)));
}
template <class Key, class Compare>
pair<typename avl<Key, Compare>::iterator, bool>
avl<Key, Compare>::insert(Key&& key)
{
    node** mount = &(bst_t::root_);
    node* parent = nullptr;
    if (bst_t::insert_pos(key, mount, parent))
    {
        *mount = new node(mrsuyi::move(key));
        (*mount)->parent = parent;
        ++bst_t::size_;
        balance(parent);
        return {*mount, true};
    }
    else
        return {*mount, false};
}
// erase
template <class Key, class Compare>
size_t
avl<Key, Compare>::erase(const Key& key)
{
    auto it = bst_t::find(key);
    if (it != bst_t::end())
    {
        erase(it);
        return 1;
    }
    return 0;
}
template <class Key, class Compare>
typename avl<Key, Compare>::iterator
avl<Key, Compare>::erase(iterator it)
{
    node* balance_pos;
    if (it.node_->l)
    {
        auto n = bst_t::max(it.node_->l);
        balance_pos = n->parent == it.node_ ? n : n->parent;
        *(bst_t::mount_pos(n)) = n->l;
        if (n->l) n->l->parent = n->parent;
        bst_t::replace(it.node_, n);
    }
    else if (it.node_->r)
    {
        auto n = bst_t::min(it.node_->r);
        balance_pos = n->parent == it.node_ ? n : n->parent;
        *(bst_t::mount_pos(n)) = n->r;
        if (n->r) n->r->parent = n->parent;
        bst_t::replace(it.node_, n);
    }
    else
    {
        balance_pos = it.node_->parent;
        if (auto mount = bst_t::mount_pos(it.node_)) *mount = nullptr;
    }
    balance(balance_pos);
    delete it.node_;
    --bst_t::size_;
}

template <class Key, class Compare>
bool
avl<Key, Compare>::height_check() const
{
    std::function<int(node*)> check = [&check](node* n) {
        if (!n) return -1;
        int l = check(n->l);
        int r = check(n->r);
        if (l == -2 || r == -2) return -2;
        int h = max(l, r) + 1;
        if (h != n->height) return -2;
        return h;
    };
    return check(bst_t::root_) != -2;
}
}
