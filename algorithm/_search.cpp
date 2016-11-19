#include "debug.hpp"
#include "search.hpp"

using namespace mrsuyi;

int
main()
{
    int a[] = {1, 2, 3};
    int b[] = {1, 2, 4};
    assert(mismatch(a, a + 3, b) == (mrsuyi::pair<int*, int*>(a + 2, b + 2)));

    a[2] = 3;
    b[2] = 3;
    assert(mismatch(a, a + 3, b) == (mrsuyi::pair<int*, int*>(a + 3, b + 3)));

    return 0;
}
