#include "debug.hpp"
#include "move_iterator.hpp"
#include "utility.hpp"

using namespace mrsuyi;

int main()
{
    shit shits[1];

    shit tmp(*move_iterator<shit*>(shits));

    return 0;
}

