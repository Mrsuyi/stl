#pragma once

#include "functional.hpp"
#include "iterator.hpp"

namespace mrsuyi
{
//================================ permutations ==============================//
// is-permutation
template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
bool
is_permutation(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2,
               BinaryPredicate p)
{
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
