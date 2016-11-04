#include <cassert>
#include <iostream>
#include "avl.hpp"
#include "debug.hpp"

using namespace mrsuyi;

template <class T, size_t N>
avl<T>
make(const T (&x)[N])
{
    avl<T> res;
    for (size_t i = 0; i < N; ++i) res.insert(x[i]);
    return res;
}

void
insert()
{
    auto t = make({3, 1, 1, 2, 2, 4, 5});
    cout << t.graph(std::to_string) << endl;
    assert(equal(t, {1, 2, 3, 4, 5}));
    assert(requal(t, {5, 4, 3, 2, 1}));
}

void
find()
{
    auto t = make({5, 2, 3, 4, 1});
    assert(*t.find(2) == 2);
    assert(*t.find(1) == 1);
    assert(t.find(100) == t.end());
};

int
main()
{
    insert();
    find();

    return 0;
};
