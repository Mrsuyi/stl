#pragma once

#include "functional.hpp"
#include "iterator.hpp"
#include "search.hpp"

namespace mrsuyi
{
//================================ permutations ==============================//
// is-permutation
template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
bool
is_permutation(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2,
               BinaryPredicate p)
{
    auto its = mrsuyi::mismatch(first1, last1, first2, p);

    first1 = its.first;
    first2 = its.second;

    if (first1 != last1)
    {
        auto last2 = first2;
        mrsuyi::advance(last2, mrsuyi::distance(first1, last1));

        for (auto it = first1; it != last1; ++it)
        {
            auto cmp = [&p, &it](
                const typename iterator_traits<ForwardIt1>::value_type& value) {
                return p(*it, value);
            };
            if (it != mrsuyi::find_if(first1, last1, cmp)) continue;
            auto cnt2 = mrsuyi::count_if(first2, last2, cmp);
            if (cnt2 == 0 || cnt2 != mrsuyi::count_if(first1, last1, cmp))
                return false;
        }
    }
    return true;
}
template <class ForwardIt1, class ForwardIt2>
bool
is_permutation(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
    return is_permutation(
        first1, last1, first2,
        mrsuyi::equal_to<
            typename mrsuyi::iterator_traits<ForwardIt1>::value_type>());
}
}
