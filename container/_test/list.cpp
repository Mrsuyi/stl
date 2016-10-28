#include <iostream>
#include <vector>
#include <cassert>
#include "../list.hpp"

using namespace std;

void ctor_dtor()
{
    // default
    mrsuyi::list<int> defau;
    // fill 
    mrsuyi::list<int> fill_defau(3);
    mrsuyi::list<int> fill_val(3, 1);
    // copy
}

void push_pop()
{
    mrsuyi::list<int> li;
    li.push_back(1);
    li.push_back(1);
    li.pop_back();
    li.pop_back();
}

void size_empty()
{
    mrsuyi::list<int> li(3, 1);
    assert(li.size() == 3);
    assert(!li.empty());
}

void traverse()
{
    mrsuyi::list<int> li;
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

int main()
{
    ctor_dtor();
    push_pop();
    traverse();
    size_empty();

    return 0;
};

