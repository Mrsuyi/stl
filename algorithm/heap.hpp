#pragma once

#include "functional.hpp"
#include "iterator.hpp"
#include "utility.hpp"

namespace mrsuyi
{
// push-heap
template <class RandomIt>
void
push_heap(RandomIt first, RandomIt last)
{
    return push_heap(first, last,
                     less<typename iterator_traits<RandomIt>::value_type>());
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
    return pop_heap(first, last,
                    less<typename iterator_traits<RandomIt>::value_type>());
}
template <class RandomIt, class Compare>
void
pop_heap(RandomIt first, RandomIt last, Compare cmp)
{
    auto len = distance(first, last);
    if (len < 2) return;

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
    return sort_heap(first, last,
                     less<typename iterator_traits<RandomIt>::value_type>());
}
template <class RandomIt, class Compare>
void
sort_heap(RandomIt first, RandomIt last, Compare cmp)
{
    for (; distance(first, last) > 1; --last) pop_heap(first, last, cmp);
}

// make-heap
template <class RandomIt, class Distance, class Compare>
void
__make_heap_aux(RandomIt first, Distance parent, Distance len, Compare cmp)
{
    while (true)
    {
        auto l = 2 * parent + 1;
        auto r = 2 * parent + 2;

        // reach the bottom
        if (r >= len)
        {
            if (r == len && cmp(*(first + parent), *(first + l)))
                swap(*(first + parent), *(first + l));
            return;
        }

        // parent < l
        if (cmp(*(first + parent), *(first + l)))
        {
            // parent < l < r
            if (cmp(*(first + l), *(first + r)))
            {
                swap(*(first + parent), *(first + r));
                parent = r;
            }
            else
            // parent/r < l
            {
                swap(*(first + parent), *(first + l));
                parent = l;
            }
        }
        // l < parent
        else
        {
            // l < parent < r
            if (cmp(*(first + parent), *(first + r)))
            {
                swap(*(first + parent), *(first + r));
                parent = r;
            }
            else
            // l/r < parent
            {
                return;
            }
        }
    }
}
template <class RandomIt>
void
make_heap(RandomIt first, RandomIt last)
{
    using T = typename iterator_traits<RandomIt>::value_type;
    make_heap(first, last,
              [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class RandomIt, class Compare>
void
make_heap(RandomIt first, RandomIt last, Compare cmp)
{
    auto len = distance(first, last);
    if (len < 2) return;

    auto parent = (len - 2) / 2;

    for (; parent >= 0; --parent) __make_heap_aux(first, parent, len, cmp);
}

// is-heap
template <class RandomIt>
bool
is_heap(RandomIt first, RandomIt last)
{
    return is_heap(first, last,
                   less<typename iterator_traits<RandomIt>::value_type>());
}
template <class RandomIt, class Compare>
bool
is_heap(RandomIt first, RandomIt last, Compare cmp)
{
    auto len = distance(first, last);

    for (decltype(len) parent = 0;; ++parent)
    {
        auto l = 2 * parent + 1;
        auto r = 2 * parent + 2;

        if (r >= len)
        {
            return l > len || cmp(*(first + l), *(first + parent));
        }
        if (cmp(*(first + parent), *(first + l)) ||
            cmp(*(first + parent), *(first + r)))
            return false;
    }
}

// is-heap-until
template <class RandomIt>
RandomIt
is_heap_until(RandomIt first, RandomIt last)
{
    return is_heap_until(
        first, last, less<typename iterator_traits<RandomIt>::value_type>());
}
template <class RandomIt, class Compare>
RandomIt
is_heap_until(RandomIt first, RandomIt last, Compare cmp)
{
    auto len = distance(first, last);

    for (decltype(len) parent = 0;; ++parent)
    {
        auto l = 2 * parent + 1;
        auto r = 2 * parent + 2;

        if (r >= len)
        {
            if (l >= len || cmp(*(first + parent), *(first + l)))
                return first + l;
            else
                return first + r;
        }
        if (cmp(*(first + parent), *(first + l))) return first + l;
        if (cmp(*(first + parent), *(first + r))) return first + r;
    }
}
}
