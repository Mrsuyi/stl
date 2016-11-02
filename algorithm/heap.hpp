#pragma once

#include "iterator.hpp"
#include "utility.hpp"

namespace mrsuyi
{
// push-heap
template <class RandomIt>
void
push_heap(RandomIt first, RandomIt last)
{
    using T = typename iterator_traits<RandomIt>::value_type;
    push_heap(first, last,
              [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class RandomIt, class Compare>
void
push_heap(RandomIt first, RandomIt last, Compare cmp)
{
    for (auto pos = mrsuyi::distance(first, last) - 1; pos >= 1;)
    {
        auto parent = (pos - 1) / 2;

        if (cmp(*(first + parent), *(first + pos)))
        {
            swap(*(first + parent), *(first + pos));
            pos = parent;
        }
        else
            break;
    }
}

// pop-heap
template <class RandomIt>
void
pop_heap(RandomIt first, RandomIt last)
{
    using T = typename iterator_traits<RandomIt>::value_type;
    pop_heap(first, last, [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class RandomIt, class Compare>
void
pop_heap(RandomIt first, RandomIt last, Compare cmp)
{
    auto len = distance(first, last);
    decltype(len) pos = 0;

    while (true)
    {
        auto l = 2 * pos + 1;
        auto r = 2 * pos + 2;

        if (r >= len) break;

        if (cmp(*(first + l), *(first + r)))
        {
            swap(*(first + pos), *(first + r));
            pos = r;
        }
        else
        {
            swap(*(first + pos), *(first + l));
            pos = l;
        }
    }
    swap(*(first + pos), *(first + len - 1));
}

// sort-heap
template <class RandomIt>
void
sort_heap(RandomIt first, RandomIt last)
{
    using T = typename iterator_traits<RandomIt>::value_type;
    sort_heap(first, last,
              [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class RandomIt, class Compare>
void
sort_heap(RandomIt first, RandomIt last, Compare cmp)
{
    for (; distance(first, last) > 1; --last) pop_heap(first, last, cmp);
}

// make-heap
template <class RandomIt>
void
make_heap(RandomIt first, RandomIt last)
{
    using T = typename RandomIt::value_type;
    make_heap(first, last,
              [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class RandomIt, class Compare>
void
make_heap(RandomIt first, RandomIt last, Compare cmp)
{
}
}
