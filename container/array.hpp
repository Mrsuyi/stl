#pragma once

#include <cstddef>
#include <stdexcept>
#include "algorithm.hpp"
#include "utility.hpp"
#include "iterator.hpp"

namespace mrsuyi
{
template <class T, size_t N>
struct array
{
    // typedefs
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    // iterators
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

    // mem
    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr bool empty() const noexcept;

    // set
    void swap(array&);
    void fill(const_reference val);

    // get
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    pointer data() noexcept;
    const_pointer data() const noexcept;

    // iterate
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    T ptr_[N];
};

//================ mem ===============//
template <class T, size_t N>
constexpr typename array<T, N>::size_type
array<T, N>::size() const noexcept
{
    return N;
}

template <class T, size_t N>
constexpr typename array<T, N>::size_type
array<T, N>::max_size() const noexcept
{
    return N;
}

template <class T, size_t N>
constexpr bool
array<T, N>::empty() const noexcept
{
    return N == 0;
}

//================ set ===============//
template <class T, size_t N>
void
array<T, N>::swap(array& x)
{
    for (size_type i = 0; i < N; ++i) swap(ptr_[i], x[i]);
}

template <class T, size_t N>
void
array<T, N>::fill(const_reference val)
{
    for (size_type i = 0; i < N; ++i) ptr_[i] = val;
}

//================ get ===============//
template <class T, size_t N>
typename array<T, N>::reference
array<T, N>::front()
{
    return ptr_[0];
}

template <class T, size_t N>
typename array<T, N>::const_reference
array<T, N>::front() const
{
    return ptr_[0];
}

template <class T, size_t N>
typename array<T, N>::reference
array<T, N>::back()
{
    return ptr_[N - 1];
}

template <class T, size_t N>
typename array<T, N>::const_reference
array<T, N>::back() const
{
    return ptr_[N - 1];
}

template <class T, size_t N>
typename array<T, N>::reference
array<T, N>::at(size_type n)
{
    if (n >= N) throw std::out_of_range("out of range");
    return ptr_[n];
}

template <class T, size_t N>
typename array<T, N>::const_reference
array<T, N>::at(size_type n) const
{
    if (n >= N) throw std::out_of_range("out of range");
    return ptr_[n];
}

template <class T, size_t N>
typename array<T, N>::reference array<T, N>::operator[](size_type n)
{
    return ptr_[n];
}

template <class T, size_t N>
typename array<T, N>::const_reference array<T, N>::operator[](size_type n) const
{
    return ptr_[n];
}

template <class T, size_t N>
typename array<T, N>::pointer
array<T, N>::data() noexcept
{
    return ptr_;
}

template <class T, size_t N>
typename array<T, N>::const_pointer
array<T, N>::data() const noexcept
{
    return ptr_;
}

//============== iterate =============//
template <class T, size_t N>
typename array<T, N>::iterator
array<T, N>::begin() noexcept
{
    return ptr_;
}

template <class T, size_t N>
typename array<T, N>::iterator
array<T, N>::end() noexcept
{
    return ptr_ + N;
}

template <class T, size_t N>
typename array<T, N>::const_iterator
array<T, N>::begin() const noexcept
{
    return ptr_;
}

template <class T, size_t N>
typename array<T, N>::const_iterator
array<T, N>::end() const noexcept
{
    return ptr_ + N;
}

template <class T, size_t N>
typename array<T, N>::const_iterator
array<T, N>::cbegin() const noexcept
{
    return ptr_;
}

template <class T, size_t N>
typename array<T, N>::const_iterator
array<T, N>::cend() const noexcept
{
    return ptr_ + N;
}

template <class T, size_t N>
typename array<T, N>::reverse_iterator
array<T, N>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <class T, size_t N>
typename array<T, N>::reverse_iterator
array<T, N>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <class T, size_t N>
typename array<T, N>::const_reverse_iterator
array<T, N>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, size_t N>
typename array<T, N>::const_reverse_iterator
array<T, N>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <class T, size_t N>
typename array<T, N>::const_reverse_iterator
array<T, N>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <class T, size_t N>
typename array<T, N>::const_reverse_iterator
array<T, N>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

//=========================== non-member functions ===========================//
template <class T, size_t N>
bool
operator==(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T, size_t N>
bool
operator!=(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return !(lhs == rhs);
}

template <class T, size_t N>
bool
operator<(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                   rhs.end());
}

template <class T, size_t N>
bool
operator<=(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return !(rhs < lhs);
}

template <class T, size_t N>
bool
operator>(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return rhs < lhs;
}

template <class T, size_t N>
bool
operator>=(const array<T, N>& lhs, const array<T, N>& rhs)
{
    return !(lhs < rhs);
}
}
