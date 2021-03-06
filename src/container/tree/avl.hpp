#pragma once

#include <cstdlib>
#include <functional>
#include "bst.hpp"
#include "container/vector.hpp"
#include "debug.hpp"
#include "utility.hpp"

namespace mrsuyi
{
template <class T>
struct avl_node
{
    avl_node *l, *r, *parent;
    int height;
    T t;

    template <class... Args>
    avl_node(Args... args)
        : l(nullptr),
          r(nullptr),
          parent(nullptr),
          height(0),
          t(forward<Args>(args)...)
    {
    }
};

template <class T, class Compare = mrsuyi::less<T>>
class avl : public bst<T, avl_node<T>, Compare>
{
protected:
    using node = avl_node<T>;
    using bst_t = bst<T, avl_node<T>, Compare>;

    // reset height
    void reset_height(node* n);
    // check balance & return height
    int check(node* n) const;
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
    avl(std::initializer_list<T>, const Compare& = Compare());
    // dtor
    ~avl();

    // element access

    // modifiers
    pair<iterator, bool> insert(const T&);
    pair<iterator, bool> insert(T&&);
    iterator erase(iterator);
    size_t erase(const T&);

    // mrsuyi's special functions :D
    bool valid() const;
};

//=============================== protected ==================================//
// reset height
template <class T, class Compare>
void
avl<T, Compare>::reset_height(node* n)
{
    int hl = n->l ? n->l->height : -1;
    int hr = n->r ? n->r->height : -1;
    n->height = max(hl, hr) + 1;
}
// check, if balance, return
template <class T, class Compare>
int
avl<T, Compare>::check(node* n) const
{
    int hl = n->l ? n->l->height : -1;
    int hr = n->r ? n->r->height : -1;
    if (abs(hl - hr) > 1) return -2;
    return max(hl, hr) + 1;
}
// balance
template <class T, class Compare>
void
avl<T, Compare>::balance(node* n)
{
    node* nl = n->l;
    node* nr = n->r;
    int hl = nl ? nl->height : -1;
    int hr = nr ? nr->height : -1;

    if (hl > hr)
    {
        int hll = nl->l ? nl->l->height : -1;
        int hlr = nl->r ? nl->r->height : -1;

        if (hll >= hlr)
            spinl(n);
        else
        {
            spinr(nl);
            spinl(n);
        }
    }
    else
    {
        int hrl = nr->l ? nr->l->height : -1;
        int hrr = nr->r ? nr->r->height : -1;

        if (hrr >= hrl)
            spinr(n);
        else
        {
            spinl(nr);
            spinr(n);
        }
    }
}

// spin-left (parent & right-child)
template <class T, class Compare>
void
avl<T, Compare>::spinl(node* old_root)
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
    //
    reset_height(old_root);
    reset_height(new_root);
}
// spin-right (parent & left-child)
template <class T, class Compare>
void
avl<T, Compare>::spinr(node* old_root)
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
    //
    reset_height(old_root);
    reset_height(new_root);
}

//============================== ctor & dtor =================================//
// default
template <class T, class Compare>
avl<T, Compare>::avl(const Compare& cmp) : bst_t(cmp)
{
}
// range
template <class T, class Compare>
template <class InputIt>
avl<T, Compare>::avl(InputIt first, InputIt last, const Compare& cmp)
    : bst_t(cmp)
{
    for (; first != last; ++first) insert(*first);
}
// range
template <class T, class Compare>
avl<T, Compare>::avl(std::initializer_list<T> il, const Compare& cmp)
    : avl(il.begin(), il.end(), cmp)
{
}
// dtor
template <class T, class Compare>
avl<T, Compare>::~avl()
{
}

//================================ modifiers =================================//
// insert
template <class T, class Compare>
pair<typename avl<T, Compare>::iterator, bool>
avl<T, Compare>::insert(const T& t)
{
    insert(move(T(t)));
}
template <class T, class Compare>
pair<typename avl<T, Compare>::iterator, bool>
avl<T, Compare>::insert(T&& t)
{
    node** mount = &(bst_t::root_);
    node* parent = nullptr;
    if (bst_t::insert_pos(t, mount, parent))
    {
        *mount = new node(mrsuyi::move(t));
        (*mount)->parent = parent;
        ++bst_t::size_;

        while (parent)
        {
            int res = check(parent);
            if (res == -2)
                balance(parent);
            else if (res != parent->height)
            {
                parent->height = res;
                parent = parent->parent;
            }
            else
                break;
        }
        return {*mount, true};
    }
    else
        return {*mount, false};
}
// erase
template <class T, class Compare>
size_t
avl<T, Compare>::erase(const T& t)
{
    auto it = bst_t::find(t);
    if (it != bst_t::end())
    {
        erase(it);
        return 1;
    }
    return 0;
}
template <class T, class Compare>
typename avl<T, Compare>::iterator
avl<T, Compare>::erase(iterator it)
{
    node* balance_pos;
    if (it.node_->l)
    {
        auto n = bst_t::max(it.node_->l);
        balance_pos = n->parent == it.node_ ? n : n->parent;
        // remove n
        *(bst_t::mount_pos(n)) = n->l;
        if (n->l) n->l->parent = n->parent;
        // replace deleted-node with n
        bst_t::replace(it.node_, n);
        // balance
        while (balance_pos)
        {
            auto parent = balance_pos->parent;
            int res = check(balance_pos);
            if (res == -2)
                balance(balance_pos);
            else if (res != balance_pos->height)
                balance_pos->height = res;
            balance_pos = parent;
        }
    }
    else if (it.node_->r)
    {
        auto n = bst_t::min(it.node_->r);
        balance_pos = n->parent == it.node_ ? n : n->parent;
        // remove n
        *(bst_t::mount_pos(n)) = n->r;
        if (n->r) n->r->parent = n->parent;
        // replace deleted-node with n
        bst_t::replace(it.node_, n);
        while (balance_pos)
        {
            auto parent = balance_pos->parent;
            int res = check(balance_pos);
            if (res == -2)
                balance(balance_pos);
            else if (res != balance_pos->height)
                balance_pos->height = res;
            balance_pos = parent;
        }
    }
    else
    {
        balance_pos = it.node_->parent;
        if (auto mount = bst_t::mount_pos(it.node_)) *mount = nullptr;
        while (balance_pos)
        {
            auto parent = balance_pos->parent;
            int res = check(balance_pos);
            if (res == -2)
                balance(balance_pos);
            else if (res != balance_pos->height)
                balance_pos->height = res;
            balance_pos = parent;
        }
    }
    delete it.node_;
    --bst_t::size_;
}

//======================== mrsuyi-special-functions :D =======================//
template <class T, class Compare>
bool
avl<T, Compare>::valid() const
{
    std::function<int(node*)> check = [&check](node* n) {
        if (!n) return -1;
        int l = check(n->l);
        int r = check(n->r);
        // if (l == -2 || r == -2 || abs(l - r) > 1) return -2;
        if (l == -2 || r == -2 || abs(l - r) > 1)
        {
            cout << "height diff > 1" << endl;
            cout << n->t << endl;
            cout << n->height << endl;
            return -2;
        }
        int h = max(l, r) + 1;
        // if (h != n->height) return -2;
        if (h != n->height)
        {
            cout << "height not same with real-h" << endl;
            cout << n->t << endl;
            cout << n->height << endl;
            return -2;
        }
        return h;
    };
    return check(bst_t::root_) != -2;
}
}
