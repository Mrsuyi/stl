#include "../memory/ptr_unique.hpp"
#include <iostream>
#include <functional>

using namespace std;

class shit
{
public:
    shit() { cout << "construct!\n"; }
    ~shit() { cout << "destruct!\n"; }
};

void
basic()
{
    mrsuyi::unique_ptr<shit> ptr(new shit());

    ptr.reset(new shit());

    auto raw = ptr.release();

    ptr.reset(raw);
}

class functor
{
public:
    void operator()(shit* s)
    {
        cout << "deleter work by functor!\n";
        delete s;
    }
};

void
func(shit* s)
{
    cout << "deleter work by function!\n";
    delete s;
}

auto lambda = [](shit* s)
{
    cout << "deleter work by lambda!\n";
    delete s;
};

void
del()
{
    mrsuyi::unique_ptr<shit, void(*)(shit*)> ptr(new shit(), func);
    mrsuyi::unique_ptr<shit, functor> ptr2(new shit(), functor());
    mrsuyi::unique_ptr<shit, decltype(lambda)> ptr3(new shit(), lambda);
}

int
main()
{
    basic();
    del();

    return 0;
};
