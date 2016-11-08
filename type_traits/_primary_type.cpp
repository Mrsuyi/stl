#include <debug.hpp>
#include "primary_type.hpp"

using namespace mrsuyi;

int main()
{
    cout << is_integral<int>::value << endl;
    cout << is_integral<double>::value << endl;

    return 0;
}

