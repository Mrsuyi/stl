#include <cassert>
#include <iostream>
#include "../functions.hpp"
#include "container/list.hpp"
#include "container/vector.hpp"

void
advance()
{
    mrsuyi::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    mrsuyi::advance(it, 2);
    assert(*it == 3);

    mrsuyi::list<int> li;
    li.push_back(1);
    li.push_back(2);
    li.push_back(3);

    auto it2 = li.begin();
    mrsuyi::advance(it2, 2);
}

void
distance()
{
    mrsuyi::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    mrsuyi::advance(it, 2);
    assert(mrsuyi::distance(vec.begin(), it) == 2);
    assert(mrsuyi::distance(vec.begin(), vec.end()) == 4);
}

int
main()
{
    return 0;
};
