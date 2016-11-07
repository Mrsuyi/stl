#include "debug.hpp"
#include "functions.hpp"

using namespace mrsuyi;

int
main()
{
    shit* shits = new shit[10];
    shit* shits2 = (shit*)malloc(sizeof(shit) * 10);

    uninitialized_move(shits, shits + 10, shits2);

    return 0;
}
