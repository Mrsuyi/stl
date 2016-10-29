#include "../list.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace mrsuyi;

void
ctor_dtor()
{
    // list
    list<int> il = {1, 2, 3};
    // default
    list<int> defau;
    // fill
    list<int> fill_defau(3);
    assert(fill_defau == list<int>({0, 0, 0}));
    list<int> fill_val(3, 1);
    assert(fill_val == list<int>({1, 1, 1}));
    // range
    std::vector<int> vec = {1, 2, 3};
    list<int> range(vec.begin(), vec.end());
    assert(range == list<int>({1, 2, 3}));
    // copy
    list<int> cp(fill_val);
    assert(cp == fill_val);
    // move
    list<int> mv(fill_val);
    assert(mv == cp);
}

void
assign()
{
    list<int> raw = {1, 2, 3};

    list<int> cp = raw;
    assert(cp == raw);
    list<int> mv = move(raw);
    assert(mv == cp);
    raw = {1};
    assert(raw == list<int>({1}));

    cp.assign(raw.begin(), raw.end());
    assert(cp == list<int>({1}));
    cp.assign(3, 1);
    assert(cp == list<int>({1, 1, 1}));
    cp.assign({1, 2, 3});
    assert(cp == list<int>({1, 2, 3}));
}

void
element_access()
{
    list<int> li = {1, 2, 3};
    assert(li.front() == 1);
    assert(li.back() == 3);
}

void
iterators()
{
    list<int> li;
    li.push_back(1);
    li.push_back(2);
    li.push_back(3);

    auto it = li.begin();
    assert(*(it++) == 1);
    assert(*(it++) == 2);
    assert(*(it++) == 3);
    it = li.end();
    assert(*(--it) == 3);
    assert(*(--it) == 2);
    assert(*(--it) == 1);

    auto rit = li.rbegin();
    rit = li.rbegin();
    assert(*(rit++) == 3);
    assert(*(rit++) == 2);
    assert(*(rit++) == 1);
}

void
capacity()
{
    list<int> li(3, 1);
    assert(li.size() == 3);
    assert(!li.empty());
}

void
push_pop()
{
    list<int> li;
    li.push_front(100);
    assert(li == list<int>({100}));
    li.push_back(999);
    assert(li == list<int>({100, 999}));
    li.pop_front();
    assert(li == list<int>({999}));
    li.pop_back();
    assert(li == list<int>({}));
}

void
emplace()
{
    list<int> li = {1, 2, 3};
    li.emplace(li.begin(), 666);
    assert(li == list<int>({666, 1, 2, 3}));
    li.emplace_front(333);
    li.emplace_back(999);
    assert(li == list<int>({333, 666, 1, 2, 3, 999}));
}

void
insert()
{
    list<int> li;
    li.insert(li.begin(), 1);
    li.insert(li.end(), 2, 88);
    assert(li == list<int>({1, 88, 88}));
    list<int> ins = {555, 666};
    li.insert(++li.begin(), ins.begin(), ins.end());
    assert(li == list<int>({1, 555, 666, 88, 88}));
}

void
erase()
{
    list<int> il = {1, 2, 3, 4, 5};
    il.erase(++il.begin());
    il.erase(++++il.begin(), il.end());
    assert(il == list<int>({1, 3}));
}

void
merge()
{
    list<int> l1 = {1, 3, 5};
    list<int> l2 = {2, 4, 6};
    l1.merge(move(l2));
    assert(l1 == list<int>({1, 2, 3, 4, 5, 6}));
}

int
main()
{
    ctor_dtor();
    assign();
    element_access();
    iterators();
    capacity();
    push_pop();
    emplace();
    insert();
    erase();
    merge();

    return 0;
};
