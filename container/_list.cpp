#include "list.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace mrsuyi;
using std::cout;
using std::endl;

void
print(const list<int>& li)
{
    cout << "=====\n";
    cout << "size: " << li.size() << endl;
    for (auto n : li) cout << n << " ";
    cout << endl;
}

void
ctor_dtor()
{
    // list
    list<int> il = {1, 2, 3};
    // default
    list<int> defau;
    // fill
    list<int> fill_defau(3);
    assert(list<int>({0, 0, 0}) == fill_defau);
    list<int> fill_val(3, 1);
    assert(list<int>({1, 1, 1}) == fill_val);
    // range
    std::vector<int> vec = {1, 2, 3};
    list<int> range(vec.begin(), vec.end());
    assert(list<int>({1, 2, 3}) == range);
    // copy
    list<int> cp(fill_val);
    assert(fill_val == cp);
    // move
    list<int> mv(fill_val);
    assert(cp == mv);
}

void
assign()
{
    list<int> raw = {1, 2, 3};

    list<int> cp = raw;
    assert(raw == cp);
    list<int> mv = move(raw);
    assert(cp == mv);
    raw = {1};
    assert(list<int>({1}) == raw);

    cp.assign(raw.begin(), raw.end());
    assert(list<int>({1}) == cp);
    cp.assign(3, 1);
    assert(list<int>({1, 1, 1}) == cp);
    cp.assign({1, 2, 3});
    assert(list<int>({1, 2, 3}) == cp);
}

void
element_access()
{
    list<int> li = {1, 2, 3};
    assert(1 == li.front());
    assert(3 == li.back());
}

void
iterators()
{
    list<int> li;
    li.push_back(1);
    li.push_back(2);
    li.push_back(3);

    auto it = li.begin();
    assert(1 == *(it++));
    assert(2 == *(it++));
    assert(3 == *(it++));
    it = li.end();
    assert(3 == *(--it));
    assert(2 == *(--it));
    assert(1 == *(--it));

    auto rit = li.rbegin();
    rit = li.rbegin();
    assert(3 == *(rit++));
    assert(2 == *(rit++));
    assert(1 == *(rit++));
}

void
capacity()
{
    list<int> li(3, 1);
    assert(3 == li.size());
    assert(!li.empty());
}

void
push_pop()
{
    list<int> li;
    li.push_front(100);
    assert(list<int>({100}) == li);
    li.push_back(999);
    assert(list<int>({100, 999}) == li);
    li.pop_front();
    assert(list<int>({999}) == li);
    li.pop_back();
    assert(list<int>({}) == li);
}

void
emplace()
{
    list<int> li = {1, 2, 3};
    li.emplace(li.begin(), 666);
    assert(list<int>({666, 1, 2, 3}) == li);
    li.emplace_front(333);
    li.emplace_back(999);
    assert(list<int>({333, 666, 1, 2, 3, 999}) == li);
}

void
insert()
{
    list<int> li;
    li.insert(li.begin(), 1);
    li.insert(li.end(), 2, 88);
    assert(list<int>({1, 88, 88}) == li);
    list<int> ins = {555, 666};
    li.insert(++li.begin(), ins.begin(), ins.end());
    assert(list<int>({1, 555, 666, 88, 88}) == li);
}

void
erase()
{
    list<int> il = {1, 2, 3, 4, 5};
    il.erase(++il.begin());
    il.erase(++++il.begin(), il.end());
    assert(list<int>({1, 3}) == il);
}

void
merge()
{
    list<int> l1 = {1, 3, 5};
    list<int> l2 = {2, 4, 6};
    l1.merge(move(l2));
    assert(list<int>({1, 2, 3, 4, 5, 6}) == l1);
    assert(list<int>({}) == l2);

    l1 = {1, 2, 3};
    l2 = {};
    l1.merge(move(l2));
    assert(list<int>({1, 2, 3}) == l1);
    assert(list<int>({}) == l2);
    l2.merge(move(l1));
    assert(list<int>({1, 2, 3}) == l2);
    assert(list<int>({}) == l1);
    
    l1 = {1, 5};
    l2 = {2, 3, 4};
    l2.merge(move(l1));
    assert(list<int>({1, 2, 3, 4, 5}) == l2);
    assert(list<int>({}) == l1);
}

void
splice()
{
    list<int> l1 = {1, 2};
    list<int> l2 = {3, 4};
    l1.splice(++l1.begin(), l2);
    assert(list<int>({1, 3, 4, 2}) == l1);
    assert(list<int>({}) == l2);

    l1 = {1, 2, 3};
    l2 = {4, 5, 6};
    l1.splice(l1.end(), l2, ++l2.begin());
    assert(list<int>({1, 2, 3, 5}) == l1);
    assert(list<int>({4, 6}) == l2);

    l1 = {1, 2, 3};
    l2 = {4, 5, 6};
    l1.splice(++l1.begin(), l2, ++l2.begin(), l2.end());
    assert(list<int>({1, 5, 6, 2, 3}) == l1);
    assert(list<int>({4}) == l2);
}

void
remove()
{
    list<int> li = {1, 2, 3};
    li.remove(2);
    assert(list<int>({1, 3}) == li);

    auto p = [](const int& num) { return num < 0; };
    li = {-1, 2, -2, 3};
    li.remove_if(p);
    assert(list<int>({2, 3}) == li);
}

void
unique()
{
    list<int> li = {};
    li.unique();
    assert(list<int>({}) == li);

    li = {1, 2, 2, 3, 3, 4};
    li.unique();
    assert(list<int>({1, 2, 3, 4}) == li);

    li = {1, 2, -2, 3, 3, 4};
    auto p = [](const int& a, const int& b) { return a == b || a == -b; };
    li.unique(p);
    assert(list<int>({1, 2, 3, 4}) == li);
}

void
sort()
{
    list<int> li = {1, 5, 2, 4, 3};
    li.sort();
    assert(list<int>({1, 2, 3, 4, 5}) == li);
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
    splice();
    remove();
    unique();
    sort();

    return 0;
};
