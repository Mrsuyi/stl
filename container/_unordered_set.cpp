#include "debug.hpp"
#include "unordered_set.hpp"

using namespace mrsuyi;

void
uset()
{
    unordered_set<int> defau;
    unordered_set<int> il = {1, 2, 3, 4};
    assert(equal(il, {1, 2, 3, 4}));
}

void
umset()
{
}

int
main()
{
    uset();
    umset();

    return 0;
}
