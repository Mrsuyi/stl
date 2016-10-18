#include "vector.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

struct shit
{
    shit() { cout << "construct shit\n"; }
    ~shit() { cout << "destruct shit\n"; }
    void* operator new(size_t sz, void*)
    {
        cout << "placement new shit\n";
        return ::operator new(sz);
    }

    void show() { cout << "shit\n"; }
};

void
con_de()
{
    mrsuyi::vector<shit> v;
    v.push_back(shit());

    std::vector<int> shit = {1, 2, 3};
    mrsuyi::vector<int> fuck(shit.begin(), shit.end());
    assert(fuck.size() == shit.size());
}

void
mem()
{
    mrsuyi::vector<int> ints;
    printf("pushed=0, size=%lu, capacity=%lu\n", ints.size(), ints.capacity());

    for (int i = 0; i < 17; ++i)
    {
        ints.push_back(i);
        printf("pushed=%d, size=%lu, capacity=%lu\n", i + 1, ints.size(),
               ints.capacity());
    }
}

void
insert()
{
    mrsuyi::vector<int> ints(2, 1);
    auto it = ints.begin();
    ++it;
    it = ints.insert(it, 10);
    ++it;
    std::vector<int> ins = {1, 2, 3};
    ints.insert(it, ins.begin(), ins.end());
    it = ints.end();
    ints.insert(it, 2, 666);

    assert(ints.size() == 8);
    assert(ints[0] == 1);
    assert(ints[1] == 10);
    assert(ints[2] == 1);
    assert(ints[3] == 2);
    assert(ints[4] == 3);
    assert(ints[5] == 1);
    assert(ints[6] == 666);
    assert(ints[7] == 666);
}

void
emplace()
{
    mrsuyi::vector<int> ints(2, 1);
    auto it = ints.begin();
    ++it;
    ints.emplace(it, 10);
    ints.emplace_back(20);
    assert(ints.size() == 4);
    assert(ints[0] == 1);
    assert(ints[1] == 10);
    assert(ints[2] == 1);
    assert(ints[3] == 20);
}

void
iter()
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

int
main()
{
    iter();
    mem();
    con_de();
    emplace();
    insert();

    return 0;
};
