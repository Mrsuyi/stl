#include "debug.hpp"
#include "hash.hpp"

using namespace mrsuyi;

int
main()
{
    double a = 123.3345;
    cout << hash<double>()(a) << endl;

    return 0;
}
