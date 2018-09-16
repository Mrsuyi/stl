#pragma once

namespace mrsuyi {
// forward the arg
template <class T>
struct identity {
  const T& operator()(const T& t) const { return t; }
};

// select in pair
template <class Pair>
struct select1st {
  const typename Pair::first_type& operator()(const Pair& p) const {
    return p.first;
  }
};
template <class Pair>
struct select2nd {
  const typename Pair::second_type& operator()(const Pair& p) const {
    return p.second;
  }
};
}  // namespace mrsuyi
