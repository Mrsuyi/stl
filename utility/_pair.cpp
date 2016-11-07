#include <iostream>
#include "pair.hpp"
#include "debug.hpp"
#include <utility>

using namespace mrsuyi;

int
main()
{
    //std::pair<int, int> p = {1, 2};
    pair<int, int> a = {1, 2};

    cout << a.first << endl;
    cout << a.second << endl;

    return 0;
}
