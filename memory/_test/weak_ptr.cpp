#include "../weak_ptr.hpp"
#include <cassert>
#include <iostream>

using namespace std;

int
main()
{
    mrsuyi::shared_ptr<int> share(new int(1));
    assert(share.use_count() == 1);

    mrsuyi::weak_ptr<int> weak(share);
    assert(weak.use_count() == 1);

    auto tmp = weak.lock();
    assert(*tmp == 1);
    assert(share.use_count() == 2);
    assert(weak.use_count() == 2);

    return 0;
};
