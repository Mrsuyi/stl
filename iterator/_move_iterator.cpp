#include "debug.hpp"
#include "move_iterator.hpp"
#include "utility.hpp"

using namespace mrsuyi;

struct shit
{
    shit() { cout << "default ctor\n"; }
    shit(const shit&) { cout << "copy ctor\n"; }
    shit(shit&&) { cout << "move ctor\n"; }
};

int main()
{
    shit shits[1];

    shit tmp(*move_iterator<shit*>(shits));

    return 0;
}

