#include <debug.hpp>

#include "bst.hpp"

using namespace mrsuyi;

int main()
{
    bst<int> a;
    a.insert(1);
    a.insert(5);
    a.insert(3);
    a.insert(2);
    a.insert(4);

    return 0;
}

