#include <cassert>
#include <iostream>
#include "heap.hpp"

using namespace mrsuyi;
using std::cout;
using std::endl;

template <class T>
void
print(const T& t)
{
    for (auto n : t) cout << n << " ";
    cout << endl;
}

template <class T, size_t L>
bool
equal(const T (&lhs)[L], const T (&rhs)[L])
{
    for (size_t i = 0; i < L; ++i)
        if (lhs[i] != rhs[i]) return false;
    return true;
}
bool
equal(int[0], int[0])
{
    return true;
}

void
push()
{
    int a[] = {5, 1, 2, 4, 8};
    push_heap(a, a + 5);
    assert(equal(a, {8, 5, 2, 4, 1}));

    int b[] = {1};
    push_heap(b, b + 1);
    assert(equal(b, {1}));

    int c[] = {};
    push_heap(c, c);
    assert(equal(c, {}));
}

void
pop()
{
    int a[] = {8, 5, 2, 4, 1};
    pop_heap(a, a + 5);
    assert(equal(a, {5, 4, 2, 1, 8}));

    int b[] = {1};
    pop_heap(b, b + 1);
    assert(equal(b, {1}));

    int c[] = {};
    pop_heap(c, c);
    assert(equal(c, {}));
}

void
sort()
{
    int a[] = {8, 5, 2, 4, 1};
    sort_heap(a, a + 5);
    assert(equal(a, {1, 2, 4, 5, 8}));
}

void
make()
{
    int a[] = {1, 2, 3, 4, 5};
    make_heap(a, a + 5);
    print(a);

    int b[] = {1, 2, 3};
    make_heap(b, b + 3);
    print(b);

    int c[] = {};
    make_heap(c, c);
    print(c);
}

void
_is_heap()
{
    int a[] = {8, 5, 2, 4, 1};
    assert(is_heap(a, a + 5));

    int b[] = {1, 2, 3, 4, 5};
    assert(!is_heap(b, b + 5));
}

void
_is_heap_until()
{
    int a[] = {2, 1, 0};
    assert(is_heap_until(a, a + 3) == a + 3);

    int b[] = {5, 4, 3, 8};
    assert(is_heap_until(b, b + 4) == b + 3);

    int c[] = {5, 4, 3, 1, 9};
    assert(is_heap_until(c, c + 5) == c + 4);
}

int
main()
{
    push();
    pop();
    sort();
    make();
    _is_heap();
    _is_heap_until();

    return 0;
};
