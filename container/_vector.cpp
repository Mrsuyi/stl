#include <cassert>
#include <iostream>
#include <vector>
#include "vector.hpp"

using namespace mrsuyi;
using std::cout;
using std::endl;

template <class T>
void
print(const T& t)
{
    for (auto& ele : t) cout << ele << "  ";
    cout << endl;
}

template <class T, size_t N>
bool
equal(const vector<T>& vec, const T (&x)[N])
{
    if (vec.size() != N) return false;
    for (size_t i = 0; i < N; ++i)
        if (vec[i] != x[i]) return false;
    return true;
}

struct shit
{
    shit() { cout << "construct shit\n"; }
    shit(const shit&) { cout << "construct copy\n"; }
    shit(shit&&) { cout << "construct move\n"; }
    ~shit() { cout << "destruct shit\n"; }
    void* operator new(size_t sz, void*)
    {
        cout << "placement new shit\n";
        return ::operator new(sz);
    }

    void show() { cout << "shit\n"; }
};

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
    //// range
    int data[] = {1, 2, 3};
    vector<int> range(data, data + 3);
    assert(equal(range, {1, 2, 3}));
    //// copy
    vector<int> cp(range);
    assert(equal(cp, {1, 2, 3}));
    //// move
    vector<int> mv(move(range));
    assert(equal(mv, {1, 2, 3}));
    vector<int> mv_from_rvalue(vector<int>({1, 2, 3}));
    assert(equal(mv_from_rvalue, {1, 2, 3}));
    // = init
    vector<int> il = {1, 2, 3};
    assert(equal(il, {1, 2, 3}));
}

void
assign()
{
    vector<int> v = { 1 };
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
iter()
{
    // non-const
    vector<int> v(10, 1);
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

void
iter_rev()
{
    vector<int> v = {1, 2, 3};
    auto it = v.rbegin();
    for (int i = 3; i >= 1; --i, ++it) assert(*it == i);
}

void
mem()
{
    vector<int> ints;
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
    vector<int> ints(2, 1);
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
    vector<int> ints(2, 1);
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
erase()
{
    vector<int> v = {1, 2, 3, 4, 5};
    auto it = v.erase(v.begin());
    assert(*it == 2);
    it = v.erase(it + 1, it + 3);
    assert(*it == 5);
    assert(v.size() == 2);
    assert(v[0] == 2);
    assert(v[1] == 5);
}

int
main()
{
    ctor_dtor();
    assign();

    iter_rev();

    return 0;
};
