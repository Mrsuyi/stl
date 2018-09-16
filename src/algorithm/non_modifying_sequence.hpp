#pragma once

#include "iterator.hpp"
#include "utility.hpp"

namespace mrsuyi {

template <class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T& value) {
  for (; first != last; ++first) {
    if (*first == value)
      return first;
  }
  return last;
}
template <class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
  for (; first != last; ++first) {
    if (p(*first))
      return first;
  }
  return last;
}
template <class InputIt, class UnaryPredicate>
InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q) {
  for (; first != last; ++first) {
    if (!q(*first))
      return first;
  }
  return last;
}

template <class InputIt, class UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
  return find_if_not(first, last, p) == last;
}
template <class InputIt, class UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
  return find_if(first, last, p) != last;
}
template <class InputIt, class UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
  return find_if(first, last, p) == last;
}

template <class InputIt1, class InputIt2>
pair<InputIt1, InputIt2> mismatch(InputIt1 first1,
                                  InputIt1 last1,
                                  InputIt2 first2) {
  for (; first1 != last1 && *first1 == *first2; ++first1, ++first2) {
  }
  return {first1, first2};
}
template <class InputIt1, class InputIt2, class BinaryPredicate>
std::pair<InputIt1, InputIt2> mismatch(InputIt1 first1,
                                       InputIt1 last1,
                                       InputIt2 first2,
                                       BinaryPredicate p) {
  for (; first1 != last1 && p(*first1, *first2); ++first1, ++first2) {
  }
  return {first1, first2};
}

template <class InputIt, class T>
typename iterator_traits<InputIt>::difference_type count(InputIt first,
                                                         InputIt last,
                                                         const T& value) {
  typename iterator_traits<InputIt>::difference_type ret = 0;
  for (; first != last; ++first) {
    if (*first == value) {
      ret++;
    }
  }
  return ret;
}
template <class InputIt, class UnaryPredicate>
typename iterator_traits<InputIt>::difference_type count_if(InputIt first,
                                                            InputIt last,
                                                            UnaryPredicate p) {
  typename iterator_traits<InputIt>::difference_type ret = 0;
  for (; first != last; ++first) {
    if (p(*first)) {
      ret++;
    }
  }
  return ret;
}

template <class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!(*first1 == *first2))
      return false;
  }
  return true;
}

// lexicographical_compare (aa < aab,  a < b)
template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1,
                             InputIterator1 last1,
                             InputIterator2 first2,
                             InputIterator2 last2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (first2 == last2 || *first2 < *first1)
      return false;
    if (*first1 < *first2)
      return true;
  }
  return first2 != last2;
}
}  // namespace mrsuyi
