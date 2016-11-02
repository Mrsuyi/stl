#include <cassert>
#include <iostream>
#include <vector>
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
equal(const int (&)[0], const int (&)[0])
{
    return true;
}

void
push()
{
    int a[] = {5, 1, 2, 4, 8};
    push_heap(a, a + 5);
    assert(equal(a, {8, 5, 2, 4, 1}));

    int c[] = {1};
    push_heap(c, c + 1);
    assert(equal(c, {1}));
}

void
pop()
{
    int a[] = {8, 5, 2, 4, 1};
    pop_heap(a, a + 5);
    assert(equal(a, {5, 4, 2, 1, 8}));

    int c[] = {1};
    pop_heap(c, c + 1);
    assert(equal(c, {1}));
}

void
sort()
{
    int a[] = {8, 5, 2, 4, 1};
    sort_heap(a, a + 5);
    assert(equal(a, {1, 2, 4, 5, 8}));
}

int
main()
{
    push();
    pop();
    sort();

    return 0;
};
