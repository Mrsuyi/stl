#include <cassert>
#include <iostream>
#include <vector>
#include "debug.hpp"
#include "string.hpp"
#include "vector.hpp"

using namespace mrsuyi;

void
ctor_dtor()
{
    // default
    vector<int> dft;
    // fill
    vector<int> fill_dft(1);
    assert(equal(fill_dft, {0}));
    vector<int> fill(3, 1);
    assert(equal(fill, {1, 1, 1}));
    // range
    int data[] = {1, 2, 3};
    vector<int> range(data, data + 3);
    assert(equal(range, {1, 2, 3}));
    // copy
    vector<int> cp(range);
    assert(equal(cp, {1, 2, 3}));
    //// move
    vector<int> mv(move(range));
    assert(equal(mv, {1, 2, 3}));
    vector<int> mv_from_rvalue(vector<int>({1, 2, 3}));
    assert(equal(mv_from_rvalue, {1, 2, 3}));
    //// = init
    vector<int> il = {1, 2, 3};
    assert(equal(il, {1, 2, 3}));
}

void
assign()
{
    vector<int> v = {1};
    // size, val
    v.assign(3, 1);
    assert(equal(v, {1, 1, 1}));
    // [first, last)
    int data[] = {1, 2, 3};
    v.assign(data, data + 3);
    assert(equal(v, {1, 2, 3}));
    // {initializer_list}
    v.assign({1, 2, 3});
    assert(equal(v, {1, 2, 3}));
}

void
capacity()
{
    vector<int> a;
    assert(a.capacity() == 0 && a.size() == 0);
    a.reserve(4);
    assert(a.capacity() == 4 && a.size() == 0);
    a.assign(4, 1);
    assert(a.capacity() == 4 && a.size() == 4);
    a.push_back(1);
    assert(a.capacity() == 8 && a.size() == 5);
    a.reserve(13);
    assert(a.capacity() == 16 && a.size() == 5);
    // resize
    a.resize(10, 1);
    assert(a.capacity() == 16 && a.size() == 10);
    a.resize(5, 1);
    assert(a.capacity() == 16 && a.size() == 5);
    // shrink
    a.shrink_to_fit();
    assert(a.capacity() == 8 && a.size() == 5);
}

void
iter()
{
    // non-const
    vector<int> v = {1, 2, 3};
    assert(equal(v, {1, 2, 3}));
    vector<int> rv = {1, 2, 3};
    assert(requal(rv, {3, 2, 1}));
}

void
push_emplace()
{
    // push_back & emplace_back
    vector<int> push = {1, 2};
    push.push_back(3);
    push.push_back(4);
    push.emplace_back(5);
    assert(equal(push, {1, 2, 3, 4, 5}));
    assert(push.capacity() == 8);
}

void
insert()
{
    vector<int> ints(2, 1);
    ints.insert(ints.begin(), 1, 100);
    assert(equal(ints, {100, 1, 1}));
    assert(ints.capacity() == 4);
    ints.insert(ints.begin() + 2, 2, 100);
    assert(equal(ints, {100, 1, 100, 100, 1}));

    // from range
    ints = {1, 4, 5};
    auto il = {2, 3};
    ints.insert(ints.begin() + 1, il.begin(), il.end());
    assert(equal(ints, {1, 2, 3, 4, 5}));
}

void
erase()
{
    vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin());
    assert(equal(v, {2, 3, 4, 5}));
    it = v.erase(it + 1, it + 3);
    assert(equal(v, {2, 5}));
}

void
swap()
{
    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5, 6};
    a.swap(b);
    assert(equal(a, {4, 5, 6}));
    assert(equal(b, {1, 2, 3}));
}

void
with_std()
{
    vector<string> strs;
    strs.push_back("fuck");
    strs.push_back("shit");
    strs.push_back("damn");
}

int
main()
{
    ctor_dtor();
    assign();
    capacity();
    iter();
    push_emplace();
    insert();
    erase();
    swap();


    return 0;
};
