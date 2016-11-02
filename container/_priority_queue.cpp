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
    priority_queue<int> from_cont(less<int>(), vec({1, 2, 3}));
    print(from_cont);
    return;
    //assert(equal(cmp_cont, {3, 2, 1}));
    // copy
    priority_queue<int> cp(from_cont);
    assert(equal(cp, {3, 2, 1}));
    priority_queue<int> mv(mrsuyi::move(cp));
}

int
main()
{
    ctor_dtor();

    return 0;
}
