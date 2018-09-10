#pragma once

#include <cstddef>
#include <cstdlib>

namespace mrsuyi {
template <class T>
class allocator {
 public:
  using value_type = T;

  template <class U>
  using other = allocator<U>;

  // ctor & dtor
  allocator() noexcept;
  template <class U>
  allocator(const allocator<U>& alloc) noexcept;
  ~allocator() noexcept;

  // mem
  T* allocate(std::size_t);
  void deallocate(T* p, std::size_t n);
};

template <class T>
allocator<T>::allocator() noexcept {}

template <class T>
template <class U>
allocator<T>::allocator(const allocator<U>&) noexcept {}

template <class T>
allocator<T>::~allocator() noexcept {}

template <class T>
T* allocator<T>::allocate(std::size_t n) {
  return (T*)malloc(n * sizeof(value_type));
}

template <class T>
void allocator<T>::deallocate(T* p, std::size_t) {
  free(p);
}
};  // namespace mrsuyi
