#include "vector.hpp"
#include <cassert>
#include <iostream>
#include <vector>

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
cmp()
{
    vector<int> v123 = {1, 2, 3};
    vector<int> v12 = {1, 2};
    vector<int> v122 = {1, 2, 2};

    assert(v123 == vector<int>({1, 2, 3}));
    assert(v123 != v12);
    assert(v123 > v12);
    assert(v123 >= v12);
    assert(v12 < v123);
    assert(v12 <= v123);

    assert(v12 != v122);
    assert(v123 > v122);
    assert(v123 >= v122);
    assert(v122 < v123);
    assert(v122 <= v123);
}

void
con_de()
{
    // default
    vector<int> dft;
    // fill
    vector<int> fill_dft(1);
    assert(fill_dft == vector<int>({0}));
    vector<int> fill(3, 1);
    assert(fill == vector<int>({1, 1, 1}));
    //// range
    std::vector<int> src = {1, 2, 3};
    vector<int> cmp = {1, 2, 3};
    vector<int> range(src.begin(), src.end());
    assert(range == cmp);
    //// copy
    vector<int> copy(range);
    assert(copy == range);
    //// move
    vector<int> move(std::move(range));
    assert(move == cmp);
    // = copy
    vector<int> oper_copy;
    oper_copy = copy;
    assert(oper_copy == copy);
    // = move
    vector<int> oper_move;
    oper_move = (std::move(move));
    assert(oper_move == cmp);
    // = init
    vector<int> oper_il;
    oper_il = {1, 2, 3};
    assert(oper_il == cmp);
}

void
assign()
{
    using vec = vector<int>;
    vec cmp = {1, 2, 3};

    vec v = {1};
    v.assign(3, 1);
    assert(v == vec({1, 1, 1}));
    v.assign(cmp.begin(), cmp.end());
    assert(v == cmp);
    v.assign({1, 2, 3});
    assert(v == cmp);
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
    std::vector<int> std_vec;

    vector<int> a;
    vector<int> basic = {1, 2, 3};
    assert(basic.size() == 3);
    assert(basic[0] == 1);
    assert(basic[1] == 2);
    assert(basic[2] == 3);

    cmp();
    con_de();
    assign();

    iter_rev();

    return 0;
};