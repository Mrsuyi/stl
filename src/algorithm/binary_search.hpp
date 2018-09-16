#pragma once

#include "functional.hpp"
#include "iterator.hpp"

namespace mrsuyi {
// lower-bound
template <class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first,
                      ForwardIt last,
                      const T& value,
                      Compare cmp) {
  ForwardIt it;
  typename iterator_traits<ForwardIt>::difference_type step, dist;
  dist = distance(first, last);

  while (dist > 0) {
    it = first;
    step = dist / 2;
    advance(it, step);

    if (cmp(*it, value)) {
      first = ++it;
      dist -= step + 1;
    } else
      dist = step;
  }
  return first;
}
template <class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
  return lower_bound(first, last, value, less<T>());
}
// upper-bound
template <class ForwardIt, class T, class Compare>
ForwardIt upper_bound(ForwardIt first,
                      ForwardIt last,
                      const T& value,
                      Compare cmp) {
  ForwardIt it;
  typename iterator_traits<ForwardIt>::difference_type step, dist;
  dist = distance(first, last);

  while (dist > 0) {
    it = first;
    step = dist / 2;
    advance(it, step);

    if (!cmp(value, *it)) {
      first = ++it;
      dist -= step + 1;
    } else
      dist = step;
  }
  return first;
}
template <class ForwardIt, class T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value) {
  return upper_bound(first, last, value, less<T>());
}
// binary-search
template <class ForwardIt, class T, class Compare>
ForwardIt binary_search(ForwardIt first,
                        ForwardIt last,
                        const T& value,
                        Compare cmp) {
  first = lower_bound(first, last, value, cmp);
  return (first != last) && (!cmp(value, *first)) ? first : last;
}
template <class ForwardIt, class T>
ForwardIt binary_search(ForwardIt first, ForwardIt last, const T& value) {
  return binary_search(first, last, value, less<T>());
}
// equal-range
template <class ForwardIt, class T, class Compare>
pair<ForwardIt, ForwardIt> equal_range(ForwardIt first,
                                       ForwardIt last,
                                       const T& value,
                                       Compare cmp) {
  return {lower_bound(first, last, value, cmp),
          upper_bound(first, last, value, cmp)};
}
template <class ForwardIt, class T>
pair<ForwardIt, ForwardIt> equal_range(ForwardIt first,
                                       ForwardIt last,
                                       const T& value) {
  return equal_range(first, last, value, less<T>());
}
}  // namespace mrsuyi
