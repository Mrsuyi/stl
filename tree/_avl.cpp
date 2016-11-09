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
    for (int i = 0; i < 100; ++i)
    {
        avl<int> a;
        int cnt = rand() % 100;
        for (int j = 0; j < cnt; ++j)
        {
            a.insert(rand() % 100);
        }
        assert(a.height_check());
    }
    // avl<int> a = {1, 5, 4, 7, 9, 6, 8, 2, 3};
    // print_tree(a);
    // assert(a.height_check());
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
    print_tree(a);
    a.erase(1);
    print_tree(a);
    assert(a.height_check());
    a.erase(3);
    print_tree(a);
    assert(a.height_check());
    a.erase(8);
    print_tree(a);
    assert(a.height_check());
    a.erase(2);
    print_tree(a);
    assert(a.height_check());
    a.erase(4);
    print_tree(a);
    cout << a.root_->l->height << endl;
    cout << a.root_->r->height << endl;
    cout << a.root_->r->r->height << endl;
    assert(a.height_check());
    a.erase(5);
    print_tree(a);
    assert(a.height_check());
    a.erase(6);
    assert(a.height_check());
    a.erase(7);
    assert(a.height_check());
    a.erase(9);
    assert(a.height_check());
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
