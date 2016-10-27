#include <iostream>
#include <vector>
#include "../list.hpp"

void push_pop()
{
    mrsuyi::list<int> li;
    li.push_back(1);
    li.push_back(1);
    li.pop_back();
    li.pop_back();
}

int main()
{
    std::vector<int> a;
    mrsuyi::list<int> li;

    return 0;
};

