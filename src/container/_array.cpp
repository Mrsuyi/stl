#include "array.hpp"
#include <array>
#include <cassert>
#include <iostream>

using namespace std;

int
main()
{
    mrsuyi::array<int, 3> my = {1, 2, 3};
    std::array<int, 3> stl = {1, 2, 3};

    for (size_t i = 0; i < 3; ++i) assert(my[i] == stl[i]);

    return 0;
};
