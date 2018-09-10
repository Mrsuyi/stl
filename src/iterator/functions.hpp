#pragma once

#include "iterator_category.hpp"
#include "iterator_traits.hpp"

namespace mrsuyi {
// advance
template <class InputIterator, class Distance>
void __advance(InputIterator& it, Distance n, input_iterator_tag) {
  while (n--)
    ++it;
}
template <class BidirectionalIterator, class Distance>
void __advance(BidirectionalIterator& it,
               Distance n,
               bidirectional_iterator_tag) {
  if (n > 0)
    while (n--)
      ++it;
  else
    while (n++)
      --it;
}
template <class RandomAccessIterator, class Distance>
void __advance(RandomAccessIterator& it,
               Distance n,
               random_access_iterator_tag) {
  it += n;
}
template <class InputIterator, class Distance>
void advance(InputIterator& it, Distance n) {
  typename iterator_traits<InputIterator>::difference_type diff = n;
  mrsuyi::__advance(it, diff, __iterator_category(it));
}

// distance
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
  typename iterator_traits<InputIterator>::difference_type res = 0;
  for (; first != last; ++first, ++res)
    ;
  return res;
}
template <class RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type __distance(
    RandomAccessIterator first,
    RandomAccessIterator last,
    random_access_iterator_tag) {
  return last - first;
}
template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type distance(
    InputIterator first,
    InputIterator last) {
  mrsuyi::__distance(first, last, __iterator_category(first));
}

// next
template <class ForwardIterator>
ForwardIterator next(
    ForwardIterator it,
    typename iterator_traits<ForwardIterator>::difference_type n = 1) {
  mrsuyi::advance(it, n);
  return it;
}
// prev
template <class ForwardIterator>
ForwardIterator prev(
    ForwardIterator it,
    typename iterator_traits<ForwardIterator>::difference_type n = 1) {
  mrsuyi::advance(it, -n);
  return it;
}
}  // namespace mrsuyi
