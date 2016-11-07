#pragma once

#include "iterator.hpp"

namespace mrsuyi
{
//==================================== copy ==================================//
template <class InputIt, class ForwardIt>
ForwardIt
uninitialized_copy(InputIt first, InputIt last, ForwardIt result)
{
    for (; first != last; ++result, ++first)
        new (static_cast<void*>(&*result))
            typename iterator_traits<ForwardIt>::value_type(*first);
    return result;
}
template <class InputIt, class Size, class ForwardIt>
ForwardIt
uninitialized_copy_n(InputIt first, Size n, ForwardIt result)
{
    for (; n > 0; ++result, ++first, --n)
        new (static_cast<void*>(&*result))
            typename iterator_traits<ForwardIt>::value_type(*first);
    return result;
}

//==================================== move ==================================//
template <class InputIt, class ForwardIt>
ForwardIt
uninitialized_move(InputIt first, InputIt last, ForwardIt result)
{
    uninitialized_copy(make_move_iterator(first), make_move_iterator(last),
                       result);
}
template <class InputIt, class Size, class ForwardIt>
ForwardIt
uninitialized_move_n(InputIt first, Size n, ForwardIt result)
{
    uninitialized_copy_n(make_move_iterator(first), n, result);
}

//==================================== fill ==================================//
template <class ForwardIt, class T>
void
uninitialized_fill(ForwardIt first, ForwardIt last, const T& x)
{
    for (; first != last; ++first)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIt>::value_type(x);
}

template <class ForwardIt, class Size, class T>
ForwardIt
uninitialized_fill_n(ForwardIt first, Size n, const T& x)
{
    for (; n--; ++first)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIt>::value_type(x);
    return first;
}

//================================ default-ctor ==============================//
template <class ForwardIt>
void
uninitialized_default_construct(ForwardIt first, ForwardIt last)
{
    for (; first != last; ++first)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIt>::valu_type;
}

template <class ForwardIt, class Size>
void
uninitialized_default_construct_n(ForwardIt first, Size n)
{
    for (; n > 0; ++first, --n)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIt>::valu_type;
}

//================================ value-ctor ==============================//
template <class ForwardIt>
void
uninitialized_value_construct(ForwardIt first, ForwardIt last)
{
    for (; first != last; ++first)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIt>::valu_type();
}

template <class ForwardIt, class Size>
void
uninitialized_value_construct_n(ForwardIt first, Size n)
{
    for (; n > 0; ++first, --n)
        new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIt>::valu_type();
}

//==================================== dtor ==================================//
template <class T>
void
destroy_at(T* t)
{
    t->~T();
}
template <class ForwardIt>
void
destory(ForwardIt first, ForwardIt last)
{
    for (; first != last; ++first) destroy_at(&*first);
}
template <class ForwardIt, class Size>
void
destroy_n(ForwardIt first, Size n)
{
    for (; n > 0; ++first, --n) destroy_at(&*first);
}
}
