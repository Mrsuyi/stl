#include <cassert>
#include <iostream>
#include "rb.hpp"
#include "debug.hpp"

using namespace mrsuyi;

template <class T>
void
print_tree(const rb<T>& t)
{
    cout << t.graph(std::to_string) << endl;
}

void
ctor_dtor()
{
    for (int i = 0; i < 100; ++i)
    {
        rb<int> a;
        int cnt = rand() % 100;
        for (int j = 0; j < cnt; ++j) a.insert(rand() % 100);
    }
}

void
iter()
{
    rb<int> t = {3, 1, 1, 2, 2, 4, 5};
    assert(equal(t, {1, 2, 3, 4, 5}));
    assert(requal(t, {5, 4, 3, 2, 1}));
}

void
find()
{
    rb<int> t = {3, 1, 1, 2, 2, 4, 5};
    assert(*t.find(2) == 2);
    assert(*t.find(1) == 1);
    assert(t.find(100) == t.end());
};

void
erase()
{
    for (int i = 0; i < 100; ++i)
    {
        rb<int> a;
        int cnt = rand() % 100;
        for (int j = 0; j < cnt; ++j) a.insert(rand() % 100);

        while (a.size() > 1)
        {
            int offset = rand() % a.size();
            auto it = a.begin();
            for (int j = 0; j < offset; ++j, ++it)
                ;
            a.erase(it);
        }
    }
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
