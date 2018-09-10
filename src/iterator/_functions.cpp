#include <cassert>
#include <iostream>
#include "functions.hpp"
#include <vector>
#include <list>

void
advance()
{
    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    std::advance(it, 2);
    assert(*it == 3);

    std::list<int> li;
    li.push_back(1);
    li.push_back(2);
    li.push_back(3);

    auto it2 = li.begin();
    std::advance(it2, 2);
}

void
distance()
{
    std::vector<int> vec = {1, 2, 3};
    auto it = vec.begin();
    std::advance(it, 2);
    assert(std::distance(vec.begin(), it) == 2);
    assert(std::distance(vec.begin(), vec.end()) == 4);
}

int
main()
{
    return 0;
};
