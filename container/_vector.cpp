#include <cassert>
#include <iostream>
#include <vector>
#include "debug.hpp"
#include "string.hpp"
#include "vector.hpp"

using namespace mrsuyi;

void
ctor_dtor()
{
    // default
    vector<int> dft;
    // fill
    vector<int> fill_dft(1);
    assert(equal(fill_dft, {0}));
    vector<int> fill(3, 1);
    assert(equal(fill, {1, 1, 1}));
    // range
    int data[] = {1, 2, 3};
    vector<int> range(data, data + 3);
    assert(equal(range, {1, 2, 3}));
    // copy
    vector<int> cp(range);
    assert(equal(cp, {1, 2, 3}));
    //// move
    vector<int> mv(move(range));
    assert(equal(mv, {1, 2, 3}));
    vector<int> mv_from_rvalue(vector<int>({1, 2, 3}));
    assert(equal(mv_from_rvalue, {1, 2, 3}));
    //// = init
    vector<int> il = {1, 2, 3};
    assert(equal(il, {1, 2, 3}));
}

void
assign()
{
    vector<int> v = {1};
    // size, val
    v.assign(3, 1);
    assert(equal(v, {1, 1, 1}));
    // [first, last)
    int data[] = {1, 2, 3};
    v.assign(data, data + 3);
    assert(equal(v, {1, 2, 3}));
    // {initializer_list}
    v.assign({1, 2, 3});
    assert(equal(v, {1, 2, 3}));
}

void
capacity()
{
    vector<int> a;
    assert(a.capacity() == 0 && a.size() == 0);
    a.reserve(4);
    assert(a.capacity() == 4 && a.size() == 0);
    a.assign(4, 1);
    assert(a.capacity() == 4 && a.size() == 4);
    a.push_back(1);
    assert(a.capacity() == 8 && a.size() == 5);
    a.reserve(13);
    assert(a.capacity() == 16 && a.size() == 5);
    // resize
    a.resize(10, 1);
    assert(a.capacity() == 16 && a.size() == 10);
    a.resize(5, 1);
    assert(a.capacity() == 16 && a.size() == 5);
    // shrink
    a.shrink_to_fit();
    assert(a.capacity() == 8 && a.size() == 5);
}

void
iter()
{
    // non-const
    vector<int> v = {1, 2, 3};
    assert(equal(v, {1, 2, 3}));
    vector<int> rv = {1, 2, 3};
    assert(requal(rv, {3, 2, 1}));
}

void
insert()
{
    vector<int> ints(2, 1);
    auto it = ints.begin();
    ++it;
    it = ints.insert(it, 10);
    ++it;
    auto il = {1, 2, 3};
    ints.insert(it, il.begin(), il.end());
    it = ints.end();
    ints.insert(it, 2, 666);

    assert(equal(ints, {1, 10, 1, 2, 3, 1, 666, 666}));
}

void
emplace()
{
    vector<int> ints(2, 1);
    ints.emplace(ints.begin() + 1, 10);
    ints.emplace_back(20);

    assert(equal(ints, {1, 10, 1, 20}));
}

void
erase()
{
    vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin());
    assert(equal(v, {2, 3, 4, 5}));
    it = v.erase(it + 1, it + 3);
    assert(equal(v, {2, 5}));
}

void
swap()
{
    vector<int> a;
    for (int i = 0; i < 5; ++i)
    {
        vector<int> tmp;
        tmp.push_back(1);
        tmp.push_back(2);
        tmp.push_back(3);
        swap(tmp, a);
    }
}

/*void*/
// nest()
//{
// vector<string> tmp;
// vector<vector<string>> matrix;

// for (int row = 0; row < 2; ++row)
//{
// matrix.push_back(tmp);
// for (int col = 0; col < 2; ++col)
//{
// matrix.back().push_back(string());
//}
//}
/*}*/

int
main()
{
    ctor_dtor();
    assign();
    capacity();
    iter();
    // insert();
    // emplace();
    // erase();
    // iter();
    // swap();
    // nest();
    // nest2();
    // nest3();

    std::vector<int> ints;
    // vector<string> strs;
    // strs.push_back(string());
    // strs.push_back(string());

    return 0;
};
