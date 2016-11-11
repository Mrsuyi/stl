#include "debug.hpp"
#include "forward_list.hpp"

using namespace mrsuyi;

void
ctor_dtor()
{
    // default
    forward_list<int> defau;
    // fill
    forward_list<int> fill_defau(3);
    assert(equal(fill_defau, {0, 0, 0}));
    forward_list<int> fill_val(3, 1);
    assert(equal(fill_val, {1, 1, 1}));
    // range
    int vec[] = {1, 2, 3};
    forward_list<int> range(vec, vec + 3);
    assert(equal(range, {1, 2, 3}));
    // copy
    forward_list<int> cp(range);
    assert(equal(cp, {1, 2, 3}));
    // move
    forward_list<int> mv(cp);
    assert(equal(mv, {1, 2, 3}));
}

void
assign()
{
    forward_list<int> raw = {1, 2, 3};

    // =
    forward_list<int> fli;
    fli = raw;
    assert(equal(fli, {1, 2, 3}));
    fli = move(raw);
    assert(equal(fli, {1, 2, 3}));
    fli = {4, 5, 6};
    assert(equal(fli, {4, 5, 6}));

    // assign
    int nums[] = {7, 8, 9};
    fli.assign(nums, nums + 3);
    assert(equal(fli, {7, 8, 9}));
    fli.assign(3, 0);
    assert(equal(fli, {0, 0, 0}));
    fli.assign({10, 11, 12});
    assert(equal(fli, {10, 11, 12}));
}

void
element_access()
{
    forward_list<int> fli = {1, 2, 3};
    assert(fli.front() == 1);
}

void
capacity()
{
    forward_list<int> fli;
    assert(fli.empty());
    fli = {1, 2, 3};
    assert(!fli.empty());
    fli.resize(2);
    assert(equal(fli, {1, 2}));
    fli.resize(5, 0);
    assert(equal(fli, {1, 2, 0, 0, 0}));
}

void
insert()
{
    forward_list<int> fli;
    fli.insert_after(fli.before_begin(), 1);
    assert(equal(fli, {1}));
    int nums[] = {2, 3, 4};
    fli.insert_after(fli.begin(), nums, nums + 3);
    assert(equal(fli, {1, 2, 3, 4}));
    fli.insert_after(fli.begin(), 3, 0);
    assert(equal(fli, {1, 0, 0, 0, 2, 3, 4}));
}

void
erase()
{
    forward_list<int> fli = {1, 2, 3, 4, 5};
    fli.erase_after(fli.before_begin());
    assert(equal(fli, {2, 3, 4, 5}));
    fli.erase_after(fli.begin(), fli.end());
    assert(equal(fli, {2}));
}

void
merge()
{
    forward_list<int> l1 = {1, 3, 5};
    forward_list<int> l2 = {2, 4, 6};
    l1.merge(move(l2));
    assert(equal(l1, {1, 2, 3, 4, 5, 6}));
    assert(l2.empty());

    l1 = {1, 2, 3};
    l2 = {};
    l1.merge(move(l2));
    assert(equal(l1, {1, 2, 3}));
    assert(l2.empty());
    l2.merge(move(l1));
    assert(equal(l2, {1, 2, 3}));
    assert(l1.empty());

    l1 = {1, 5};
    l2 = {2, 3, 4};
    l2.merge(move(l1));
    assert(equal(l2, {1, 2, 3, 4, 5}));
    assert(l1.empty());
}

void
splice()
{
    forward_list<int> l1 = {1, 2};
    forward_list<int> l2 = {3, 4};
    l1.splice_after(l1.begin(), l2);
    assert(equal(l1, {1, 3, 4, 2}));
    assert(l2.empty());

    l1 = {1, 2, 3};
    l2 = {4, 5, 6};
    l1.splice_after(++l1.begin(), l2, l2.begin());
    assert(equal(l1, {1, 2, 5, 3}));
    assert(equal(l2, {4, 6}));

    l1 = {1, 2, 3};
    l2 = {4, 5, 6};
    l1.splice_after(++++l1.begin(), l2, l2.begin(), ++++l2.begin());
    assert(equal(l1, {1, 2, 3, 5}));
    assert(equal(l2, {4, 6}));
}

void
remove()
{
    forward_list<int> li = {1, 2, 3};
    li.remove(2);
    assert(equal(li, {1, 3}));

    auto p = [](const int& num) { return num < 0; };
    li = {-1, 2, -2, 3};
    li.remove_if(p);
    assert(equal(li, {2, 3}));
}

void
unique()
{
    forward_list<int> li = {};
    li.unique();
    assert(li.empty());

    li = {1, 2, 2, 3, 3, 4};
    li.unique();
    assert(equal(li, {1, 2, 3, 4}));

    li = {1, 2, -2, 3, 3, 4};
    auto p = [](const int& a, const int& b) { return a == b || a == -b; };
    li.unique(p);
    assert(equal(li, {1, 2, 3, 4}));
}

void
sort()
{
    forward_list<int> li = {1, 5, 2, 4, 3};
    li.sort();
    assert(forward_list<int>({1, 2, 3, 4, 5}) == li);
}

int
main()
{
    ctor_dtor();
    assign();
    element_access();
    capacity();
    insert();
    erase();
    merge();
    splice();
    remove();
    unique();
    sort();

    return 0;
};
