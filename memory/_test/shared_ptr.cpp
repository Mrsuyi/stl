#include "memory/shared_ptr.hpp"
#include <cassert>
#include <iostream>
#include "utility.hpp"

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
    {
        auto lambda = [](int* i) {
            cout << "deleter work by lambda!\n";
            delete i;
        };
        mrsuyi::shared_ptr<int> deleter(new int(1), lambda);
    }
    // copy
    {
        mrsuyi::shared_ptr<int> copy_ctor(from_pointer);
    }
    // copy from weak
    {
        mrsuyi::weak_ptr<int> weak(from_pointer);
        {
            mrsuyi::shared_ptr<int> from_weak(weak);
            assert(*from_weak == 1);
            assert(weak.use_count() == 2);
        }
        assert(weak.use_count() == 1);
    }
    // move
    {
        mrsuyi::shared_ptr<int> origin(new int(1));
        mrsuyi::shared_ptr<int> move_from_shared(move(origin));
        mrsuyi::unique_ptr<int> unique(new int(1));
        mrsuyi::shared_ptr<int> move_from_unique(move(unique));
    }
    // alias
    {
        int* tmp = new int(100);
        mrsuyi::shared_ptr<int> alias(from_pointer, tmp);
        assert(*alias == 100);
        delete tmp;
    }
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

void
cmp()
{
    int* tmp1 = new int(2);
    int* tmp2 = new int(2);
    mrsuyi::shared_ptr<int> ptr1(tmp1);       // store t1, manage t1
    mrsuyi::shared_ptr<int> ptr2(ptr1, tmp1); // store t1, manage t1
    mrsuyi::shared_ptr<int> ptr3(ptr1, tmp2); // store t2, manage t1

    assert(ptr1 == ptr2);
    assert(!(ptr1.owner_before(ptr2)));
    assert(ptr1 != ptr3);
    assert(!(ptr1.owner_before(ptr3)));

    delete tmp2;
}

int
main()
{
    ctor();
    operations();
    cmp();

    return 0;
};
