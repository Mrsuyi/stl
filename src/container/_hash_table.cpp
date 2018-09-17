#include <unordered_set>
#include "debug.hpp"
#include "hash_table.hpp"

using namespace mrsuyi;

template <class T>
void
insert_equal(hash_table<T>& ht, std::initializer_list<T> il)
{
    for (auto it = il.begin(); it != il.end(); ++it) ht.emplace_equal(*it);
}
template <class T>
void
insert_unique(hash_table<T>& ht, std::initializer_list<T> il)
{
    for (auto it = il.begin(); it != il.end(); ++it) ht.emplace_unique(*it);
}

void
ctor_dtor()
{
    // default
    hash_table<int> ht;
    ht.emplace_equal(1);
    ht.emplace_equal(1);
    ht.emplace_equal(2);
    ht.emplace_equal(2);
    assert(equal(ht, {1, 1, 2, 2}));
    // copy
    auto cp(ht);
    assert(equal(cp, {1, 1, 2, 2}));
    assert(equal(ht, {1, 1, 2, 2}));
    // move
    auto mv(mrsuyi::move(ht));
    assert(equal(mv, {1, 1, 2, 2}));
    assert(ht.empty());

    // swap
    mv.swap(ht);
    assert(equal(ht, {1, 1, 2, 2}));
    assert(mv.empty());

    // = copy
    mv = ht;
    assert(equal(ht, {1, 1, 2, 2}));
    assert(equal(mv, {1, 1, 2, 2}));

    //// = move
    mv = mrsuyi::move(ht);
    assert(equal(mv, {1, 1, 2, 2}));
    assert(ht.empty());
}

void
emplace()
{
    hash_table<int> ht;
    assert(ht.bucket_count() == 5);
    ht.emplace_equal(1);
    ht.emplace_equal(1);
    ht.emplace_equal(2);
    ht.emplace_equal(2);
    ht.emplace_equal(3);
    ht.emplace_equal(3);
    assert(ht.bucket_count() == 11);
    assert(equal(ht, {1, 1, 2, 2, 3, 3}));

    ht.clear();
    ht.emplace_unique(1);
    ht.emplace_unique(1);
    ht.emplace_unique(2);
    ht.emplace_unique(2);
    ht.emplace_unique(3);
    ht.emplace_unique(3);
    assert(equal(ht, {1, 2, 3}));
}

void
local_iter()
{
    hash_table<int> ht;
    ht.emplace_equal(1);
    ht.emplace_equal(1);
    ht.emplace_equal(2);
    ht.emplace_equal(2);
    ht.emplace_equal(3);
    ht.emplace_equal(3);

    auto bgn = ht.begin(ht.bucket(1));
    auto end = ht.end(ht.bucket(1));
    assert(*bgn == 1);
    ++bgn;
    assert(*bgn == 1);
    ++bgn;
    assert(bgn == end);
    bgn = end;
}

void
insert()
{
    hash_table<int> ht;
    insert_equal(ht, {1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9});
    assert(equal(ht, {1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9}));
    ht.clear();
    insert_unique(ht, {1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9});
    assert(equal(ht, {1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

void
erase()
{
    // single
    hash_table<int> ht;
    insert_equal(ht, {1, 1, 2, 2, 3, 3, 4});
    *ht.erase(ht.begin());
    assert(equal(ht, {1, 2, 2, 3, 3, 4}));

    // range
    ht.erase(ht.begin(), ++++++ht.begin());
    assert(equal(ht, {3, 3, 4}));

    // key-equal
    assert(ht.erase_equal(3) == 2);
    assert(equal(ht, {4}));
    assert(ht.erase_equal(10) == 0);
    assert(equal(ht, {4}));

    // key-unique
    insert_equal(ht, {8, 8});
    assert(ht.erase_unique(4) == 1);
    assert(equal(ht, {8, 8}));
    assert(ht.erase_unique(8) == 1);
    assert(equal(ht, {8}));
    assert(ht.erase_unique(8) == 1);
    assert(ht.empty());
}

void
lookup()
{
    hash_table<int> ht;
    insert_equal(ht, {1, 1, 2, 2, 3, 3});

    assert(ht.count_equal(1) == 2);

    assert(*ht.find(1) == 1);

    auto p = ht.equal_range(1);
    auto it = p.first;
    assert(*it++ == 1);
    assert(*it++ == 1);
    assert(it == p.second);

    p = ht.equal_range(3);
    it = p.first;
    assert(*it++ == 3);
    assert(*it++ == 3);
    assert(it == p.second);
}

int
main()
{
    ctor_dtor();
    emplace();
    local_iter();
    insert();
    erase();
    lookup();

    std::unordered_set<int> a;

    return 0;
}