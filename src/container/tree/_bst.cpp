#include <algorithm>
#include <debug.hpp>
#include <set>

#include "bst.hpp"

using namespace mrsuyi;

template <class T>
void
print_tree(const bst<T>& t)
{
    cout << t.graph(std::to_string) << endl;
}

int
main()
{
    for (int i = 0; i < 100; ++i)
    {
        std::set<int> s;
        for (int j = 0; j < 100; ++j) s.insert(rand() % 666);
        bst<int> t(s.begin(), s.end());
        assert(mrsuyi::equal(t.begin(), t.end(), s.begin()));
    }

    return 0;
}
