#include "debug.hpp"
#include "unordered_set.hpp"

using namespace mrsuyi;

void
uset()
{
    unordered_set<int> defau;
    assert(defau.empty());
    unordered_set<int> il = {1, 2, 3, 4};
    assert(equal(il, {1, 2, 3, 4}));

    // insert
    il.insert(1);
    il.insert(2);
    il.insert(3);
    il.insert(5);
    il.insert(6);
    assert(equal(il, {1, 2, 3, 4, 5, 6}));

    // erase
    il.erase(5);
    il.erase(6);
    il.erase(7);
    assert(equal(il, {1, 2, 3, 4}));
    assert(il == unordered_set<int>({1, 2, 3, 4}));
}

void
umset()
{
    unordered_multiset<int> defau;
    assert(defau.empty());
    unordered_multiset<int> il = {1, 1, 2, 2, 3, 3};
    assert(equal(il, {1, 1, 2, 2, 3, 3}));

    // insert
    il.insert(4);
    il.insert(4);
    assert(equal(il, {1, 1, 2, 2, 3, 3, 4, 4}));

    // erase
    assert(il.erase(1) == 2);
    il.erase(2);
    assert(equal(il, {3, 3, 4, 4}));
    assert(il == unordered_multiset<int>({3, 3, 4, 4}));
    assert(il != unordered_multiset<int>({1, 2, 3, 4}));
}

int
main()
{
    uset();
    umset();

    return 0;
}
