#include "debug.hpp"
#include "permutation.hpp"

using namespace mrsuyi;

int
main()
{
    int a[] = {1, 2, 2, 4, 4};
    int b[] = {4, 2, 4, 2, 1};
    int c[] = {4, 2, 4, 2, 3};

    assert(is_permutation(a, a + 5, b));
    assert(!is_permutation(b, b + 5, c));
}
