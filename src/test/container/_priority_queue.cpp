#include <iostream>

#include <cassert>
#include <queue>
#include "priority_queue.hpp"

using namespace mrsuyi;
using vec = vector<int>;
using std::cout;
using std::endl;

template <class T, size_t N>
bool
equal(const priority_queue<T>& q, const T (&x)[N])
{
    priority_queue<T> tmp(q);
    for (size_t i = 0; i < N; ++i)
    {
        if (tmp.top() != x[i]) return false;
        tmp.pop();
    }
    return tmp.empty();
}

template <class T>
void
print(const priority_queue<T>& q)
{
    priority_queue<T> tmp(q);

    for (; !tmp.empty();)
    {
        cout << tmp.top() << " ";
        tmp.pop();
    }
    cout << endl;
}

void
ctor_dtor()
{
    // default
    priority_queue<int> defau;
    priority_queue<int> cont(less<int>(), vec({1, 2, 3}));
    assert(equal(cont, {3, 2, 1}));
    // copy
    priority_queue<int> cp(cont);
    assert(equal(cp, {3, 2, 1}));
    priority_queue<int> mv(mrsuyi::move(cp));
    assert(equal(mv, {3, 2, 1}));
    // iter
    int data[] = {4, 5, 6};
    priority_queue<int> iter(data, data + 3);
    assert(equal(iter, {6, 5, 4}));
}

void
oper()
{
    priority_queue<int> a(less<int>(), {1, 2, 3});
    priority_queue<int> b(less<int>(), {4, 5, 6});

    auto tmp = move(a);
    a = b;
    b = tmp;

    assert(equal(a, {6, 5, 4}));
    assert(equal(b, {3, 2, 1}));

    a.swap(b);

    assert(equal(a, {3, 2, 1}));
    assert(equal(b, {6, 5, 4}));
}

void
push_pop()
{
    priority_queue<int> q(less<int>(), {1, 4, 6, 7, 5, 3, 8, 2, 9});
    q.push(10);
    q.push(14);
    q.push(15);
    q.push(12);
    q.push(13);
    q.push(11);
    assert(equal(q, {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}));
}

int
main()
{
    ctor_dtor();
    oper();
    push_pop();

    return 0;
}
