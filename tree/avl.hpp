#pragma once

#include <cstdlib>
#include "bst.hpp"
#include "container/vector.hpp"
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
    using iterator = typename bst_t::iterator;
    using const_iterator = typename bst_t::const_iterator;

    // balance node till root
    void balance(node* n);
    // spin
    void spinl(node* parent);
    void spinr(node* parent);

public:
    // ctor & dtor
    avl(const Compare& = Compare());
    virtual ~avl() override;

    // element access

    // modifiers
    iterator insert(const Key&);
    iterator insert(Key&&);
    iterator erase(const Key&);
    iterator erase(const_iterator);
};

//=============================== protected ==================================//
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
template <class Key, class Compare>
void
avl<Key, Compare>::spinl(node* parent)
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
template <class Key, class Compare>
void
avl<Key, Compare>::spinr(node* parent)
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
template <class Key, class Compare>
avl<Key, Compare>::avl(const Compare& cmp) : bst_t(cmp)
{
}
template <class Key, class Compare>
avl<Key, Compare>::~avl()
{
}

//================================ modifiers =================================//
// insert
template <class Key, class Compare>
typename avl<Key, Compare>::iterator
avl<Key, Compare>::insert(const Key& key)
{
    insert(Key(key));
}
template <class Key, class Compare>
typename avl<Key, Compare>::iterator
avl<Key, Compare>::insert(Key&& key)
{
    node** mount = &(bst_t::root_);
    node* parent = nullptr;

    while (*mount)
    {
        parent = *mount;

        if (bst_t::cmp_(key, (*mount)->key))
            mount = &((*mount)->l);
        else
            mount = &((*mount)->r);
    }

    if (!parent || bst_t::cmp_(key, parent->key) || bst_t::cmp_(parent->key, key))
    {
        *mount = new node(move(key));
        (*mount)->parent = parent;
        ++bst_t::size_;
    }
}
// erase
template <class Key, class Compare>
typename avl<Key, Compare>::iterator
avl<Key, Compare>::erase(const Key& key)
{
}
template <class Key, class Compare>
typename avl<Key, Compare>::iterator
avl<Key, Compare>::erase(const_iterator it)
{
}
}
