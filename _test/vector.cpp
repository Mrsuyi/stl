#include <iostream>
#include <cassert>
#include <vector>
#include "vector.hpp"

using namespace std;

struct shit
{
    shit() { cout << "construct shit\n"; }
    ~shit() { cout << "destruct shit\n"; }

    void* operator new (size_t sz, void*)
    {
        cout << "placement new shit\n";
        return ::operator new(sz);
    }

    void show() { cout << "shit\n"; }
};

void con_de()
{
    mrsuyi::vector<shit> v;
    v.push_back(shit());

    vector<int> shit = {1, 2, 3};
    mrsuyi::vector<int> fuck(shit.begin(), shit.end());
    assert(fuck.size() == shit.size());
}

void mem()
{
    mrsuyi::vector<int> ints;

    for (int i = 0; i < 17; ++i)
    {
        ints.push_back(i);
        printf("i=%d, size=%lu, capacity=%lu\n", i, ints.size(), ints.capacity());
    }
}
 
void iter()
{
    // non-const
    mrsuyi::vector<int> v(10, 1);
    int i = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        assert(*it == 1);
        *it = ++i;
    }
    i = 0;
    for (auto& val : v)
    {
        assert(val == ++i);
        val *= 2;
    }
}

int main()
{
    iter();
    con_de();

    return 0;
};

