#include "debug.hpp"
#include "hash_table.hpp"

using namespace mrsuyi;

void
ctor_dtor()
{
    // default
    hash_table<int> ht;
}

void
emplace()
{
    hash_table<int> ht;
    ht.emplace_equal(1);
    ht.emplace_equal(1);
    ht.emplace_equal(2);
    ht.emplace_equal(2);
    ht.emplace_equal(3);
    ht.emplace_equal(3);
    assert(equal(ht, {1, 1, 2, 2, 3, 3}));

    ht.clear();
    ht.emplace_unique(1);
    ht.emplace_unique(1);
    ht.emplace_unique(2);
    ht.emplace_unique(2);
    ht.emplace_unique(3);
    ht.emplace_unique(3);
    assert(equal(ht, {1, 2, 3}));
}

int
main()
{
    ctor_dtor();
    emplace();

    return 0;
}
