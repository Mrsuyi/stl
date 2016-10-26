#include <iostream>
#include <cassert>
#include "../shared_ptr.hpp"

using namespace std;

class shit
{
public:
    shit() { cout << "construct!\n"; }
    ~shit() { cout << "destruct!\n"; }
    void fuck() { cout << "fuck\n"; }
};

void
basic()
{
    mrsuyi::shared_ptr<shit> ptr1(new shit());
    mrsuyi::shared_ptr<shit> ptr2(ptr1);
    assert(ptr1.use_count() == 2);
    assert(ptr2.use_count() == 2);

    ptr1.reset(new shit());
    assert(ptr1.use_count() == 1);
    assert(ptr2.use_count() == 1);

    ptr2->fuck();
    ptr2.swap(ptr1);
    
    ptr2 = ptr1;
    assert(ptr1.use_count() == 2);
    assert(ptr2.use_count() == 2);
    assert(ptr1.get() == ptr2.get());
}

auto lambda = [](shit* s)
{
    cout << "deleter work by lambda!\n";
    delete s;
};

void
del()
{
    mrsuyi::shared_ptr<shit> ptr(new shit(), lambda);
}

int
main()
{
    basic();
    del();

    return 0;
};
