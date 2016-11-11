#include "comparisons.hpp"
#include "debug.hpp"

using namespace mrsuyi;

int
main()
{
    assert(less<int>()(1, 2));

    return 0;
}
