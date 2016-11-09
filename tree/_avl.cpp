#include <cassert>
#include <iostream>
#include "avl.hpp"
#include "debug.hpp"

using namespace mrsuyi;

template <class T>
void
print_tree(const avl<T>& t)
{
    cout << t.graph(std::to_string) << endl;
}

void
ctor_dtor()
{
    avl<int> a = {1, 5, 4, 7, 9, 6, 8, 2, 3};
}

void
iter()
{
    avl<int> t = {3, 1, 1, 2, 2, 4, 5};
    assert(equal(t, {1, 2, 3, 4, 5}));
    assert(requal(t, {5, 4, 3, 2, 1}));
}

void
find()
{
    avl<int> t = {3, 1, 1, 2, 2, 4, 5};
    assert(*t.find(2) == 2);
    assert(*t.find(1) == 1);
    assert(t.find(100) == t.end());
};

void
erase()
{
    avl<int> a = {1, 5, 4, 7, 9, 6, 8, 2, 3};
    cout << a.graph(std::to_string) << endl;
    a.erase(1);
    cout << a.graph(std::to_string) << endl;
    a.erase(3);
    cout << a.graph(std::to_string) << endl;
    a.erase(8);
    cout << a.graph(std::to_string) << endl;
    a.erase(2);
    cout << a.graph(std::to_string) << endl;
    a.erase(4);
    cout << a.graph(std::to_string) << endl;
    a.erase(5);
    cout << a.graph(std::to_string) << endl;
    a.erase(6);
    cout << a.graph(std::to_string) << endl;
    a.erase(7);
    cout << a.graph(std::to_string) << endl;
    a.erase(9);
    cout << a.graph(std::to_string) << endl;
}

int
main()
{
    ctor_dtor();
    iter();
    find();
    erase();

    return 0;
};
