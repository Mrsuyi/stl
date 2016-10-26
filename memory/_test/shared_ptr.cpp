#include <iostream>
#include <cassert>
#include "utility.hpp"
#include "memory/shared_ptr.hpp"

using std::cout;
using std::endl;

void
ctor()
{
    // default
    mrsuyi::shared_ptr<int> default_ctor;
    mrsuyi::shared_ptr<int> default_null(nullptr);
    
    // from pointer
    mrsuyi::shared_ptr<int> from_pointer(new int(1));

    // from deleter
    auto lambda = [](int* i)
    {
        cout << "deleter work by lambda!\n";
        delete i;
    };
    mrsuyi::shared_ptr<int> ptr(new int(1), lambda);

    // copy
    mrsuyi::shared_ptr<int> copy_ctor(from_pointer);

    // copy from weak

    // move
    mrsuyi::shared_ptr<int> origin(new int(1));
    mrsuyi::shared_ptr<int> move_from_shared(move(origin));
    mrsuyi::unique_ptr<int> unique(new int(1));
    mrsuyi::shared_ptr<int> move_from_unique(move(unique));

    // alias
    int* tmp = new int(100);
    mrsuyi::shared_ptr<int> alias(from_pointer, tmp);
    assert(*alias == 100);
    delete tmp;
}

void
operations()
{
    mrsuyi::shared_ptr<int> ptr1(new int(1));
    mrsuyi::shared_ptr<int> ptr2(ptr1);
    assert(ptr1.use_count() == 2);
    assert(ptr2.use_count() == 2);

    ptr1.reset(new int(1));
    assert(ptr1.use_count() == 1);
    assert(ptr2.use_count() == 1);

    ptr2.swap(ptr1);
    
    ptr2 = ptr1;
    assert(ptr1.use_count() == 2);
    assert(ptr2.use_count() == 2);
    assert(ptr1.get() == ptr2.get());
}

int
main()
{
    ctor();
    operations();

    return 0;
};
