#include "debug.hpp"
#include <type_traits>

using namespace std;

int main()
{
    std::is_integral<int>::type a;

    return 0;
}

