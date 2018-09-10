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
struct rb_node
{
    rb_node *l, *r, *parent;
    bool color;
    T t;

    template <class... Args>
    rb_node(Args... args)
        : l(nullptr), r(nullptr), parent(nullptr), t(forward<Args>(args)...)
    {
    }
};

template <class T, class Compare = mrsuyi::less<T>>
class rb : public bst<T, rb_node<T>, Compare>
{
protected:
    constexpr static bool RED = true;
    constexpr static bool BLACK = false;

    using node = rb_node<T>;
    using bst_t = bst<T, rb_node<T>, Compare>;

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
    rb(const Compare& = Compare());
    // range
    template <class InputIter>
    rb(InputIter first, InputIter last, const Compare& cmp = Compare());
    // initializer_list
    rb(std::initializer_list<T>, const Compare& = Compare());
    // dtor
    ~rb();

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
// default
template <class T, class Compare>
rb<T, Compare>::rb(const Compare& cmp) : bst_t(cmp)
{
}
// range
template <class T, class Compare>
template <class InputIt>
rb<T, Compare>::rb(InputIt first, InputIt last, const Compare& cmp) : bst_t(cmp)
{
    for (; first != last; ++first) insert(*first);
}
// range
template <class T, class Compare>
rb<T, Compare>::rb(std::initializer_list<T> il, const Compare& cmp)
    : rb(il.begin(), il.end(), cmp)
{
}
// dtor
template <class T, class Compare>
rb<T, Compare>::~rb()
{
}

//================================ modifiers =================================//
// insert
template <class T, class Compare>
pair<typename rb<T, Compare>::iterator, bool>
rb<T, Compare>::insert(const T& t)
{
    insert(move(T(t)));
}
template <class T, class Compare>
pair<typename rb<T, Compare>::iterator, bool>
rb<T, Compare>::insert(T&& t)
{
    node** mount = &(bst_t::root_);
    node* parent = nullptr;
    if (bst_t::insert_pos(t, mount, parent))
    {
        *mount = new node(mrsuyi::move(t));
        (*mount)->parent = parent;
        ++bst_t::size_;
        return {*mount, true};
    }
    else
        return {*mount, false};
}
// erase
template <class T, class Compare>
size_t
rb<T, Compare>::erase(const T& t)
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
typename rb<T, Compare>::iterator
rb<T, Compare>::erase(iterator it)
{
    node* balance_pos;
    if (it.node_->l)
    {
        auto n = bst_t::max(it.node_->l);
        // remove n
        *(bst_t::mount_pos(n)) = n->l;
        if (n->l) n->l->parent = n->parent;
        // replace deleted-node with n
        bst_t::replace(it.node_, n);
    }
    else if (it.node_->r)
    {
        auto n = bst_t::min(it.node_->r);
        // remove n
        *(bst_t::mount_pos(n)) = n->r;
        if (n->r) n->r->parent = n->parent;
        // replace deleted-node with n
        bst_t::replace(it.node_, n);
    }
    else
    {
        balance_pos = it.node_->parent;
        *(bst_t::mount_pos(it.node_)) = nullptr;
    }
    delete it.node_;
    --bst_t::size_;
}

//======================== mrsuyi-special-functions :D =======================//
template <class T, class Compare>
bool
rb<T, Compare>::valid() const
{
    return true;
}
}
