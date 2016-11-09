#include <debug.hpp>

#include "bst.hpp"

using namespace mrsuyi;

template <class T>
void
print_tree(const bst<T>& t)
{
    cout << t.graph(std::to_string) << endl;
}

void
insert()
{
    bst<int> a = {1, 2, 3, 4, 5};
    assert(equal(a, {1, 2, 3, 4, 5}));
}

void
erase()
{
    bst<int> a = {7, 1, 5, 3, 2, 4, 6, 8, 9};
    a.erase(2);
    a.erase(5);
    a.erase(8);
    a.erase(9);
    a.erase(1);
    assert(equal(a, {3, 4, 6, 7}));
}

int
main()
{
    insert();
    erase();

    return 0;
}
