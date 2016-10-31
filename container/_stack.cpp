#include <iostream>
#include <cassert>
#include "stack.hpp"

using namespace mrsuyi;
using std::cout;
using std::endl;

int main()
{
   stack<int> a; 

   a.push(1);
   a.push(2);
   a.push(3);

   stack<int> b(a);

   assert(a == b);

    return 0;
};

