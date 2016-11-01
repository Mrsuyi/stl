#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace mrsuyi
{
template <class T, class Alloc = allocator<T>>
class deque
{
    template <class E>
    class iter;

    // typedefs
public:
    using value_type = T;
    using allocator_type = Alloc;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    // iterators
    using iterator = iter<T>;
    using const_iterator = iter<const T>;
    using reverse_iterator = mrsuyi::reverse_iterator<iterator>;
    using const_reverse_iterator = mrsuyi::reverse_iterator<const_iterator>;

public:
    // ctor & dtor
    // default
    deque();
    explicit deque(const Alloc& alloc);
    // fill
    explicit deque(size_t n, const Alloc& alloc = Alloc());
    explicit deque(size_t n, const T& val, const Alloc& alloc = Alloc());
    // range
    template <class InputIterator>
    deque(InputIterator first, InputIterator last, const Alloc& alloc = Alloc(),
          typename std::enable_if<
              !std::is_integral<InputIterator>::value>::type* = 0);
    // copy
    deque(const deque& x);
    deque(const deque& x, const Alloc& alloc);
    // move
    deque(deque&& x);
    deque(deque&& x, const Alloc& alloc);
    // list
    deque(std::initializer_list<T> il, const Alloc& alloc = Alloc());
    // des
    ~deque() noexcept;

    // assignment
    deque& operator=(const deque& x);
    deque& operator=(deque&& x);
    deque& operator=(std::initializer_list<T> il);

    template <class InputIterator>
    void assign(InputIterator first, InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void assign(size_t n, const T& val);
    void assign(std::initializer_list<T> il);

    Alloc get_allocator() const noexcept;

    // element access
    T& at(size_t n);
    const T& at(size_t n) const;
    T& operator[](size_t n);
    const T& operator[](size_t n) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // iterators
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

    // capacity
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t max_size() const;
    void shrink_to_fit();

    // modifiers
    void clear();

    iterator insert(const_iterator position, const T& val);
    iterator insert(const_iterator position, T&& val);
    void insert(const_iterator position, size_t n, const T& val);
    template <class InputIterator>
    void insert(const_iterator position, InputIterator first,
                InputIterator last,
                typename std::enable_if<
                    !std::is_integral<InputIterator>::value>::type* = 0);
    void insert(const_iterator position, std::initializer_list<T> il);

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void push_front(const T& val);
    void push_front(T&& val);
    template <class... Args>
    void emplace_front(Args&&... args);
    void pop_front();

    void push_back(const T& val);
    void push_back(T&& val);
    template <class... Args>
    void emplace_back(Args&&... args);
    void pop_back();

    void resize(size_t size, T val = T());

    void swap(deque& x);

protected:
    T* ptr_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    Alloc alloc_;
};

////================================== basic ===================================//
//// default
//template <class T, class Alloc>
//deque<T, Alloc>::deque() : deque(Alloc())
//{
//}
//template <class T, class Alloc>
//deque<T, Alloc>::deque(const Alloc& alloc) : alloc_(alloc)
//{
//}
//// fill
//template <class T, class Alloc>
//deque<T, Alloc>::deque(size_t n, const Alloc& alloc)
    //: deque(n, T(), alloc)
//{
//}
//template <class T, class Alloc>
//deque<T, Alloc>::deque(size_t n, const T& val, const Alloc& alloc)
    //: alloc_(alloc)
//{
    //for (size_t i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    //size_ = n;
//}
//// range
//template <class T, class Alloc>
//template <class InputIterator>
//deque<T, Alloc>::deque(
    //InputIterator first, InputIterator last, const Alloc& alloc,
    //typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
    //: alloc_(alloc)
//{
    //for (; first != last; ++first) push_back(*first);
//}
//// copy
//template <class T, class Alloc>
//deque<T, Alloc>::deque(const deque& x) : deque(x, Alloc())
//{
//}
//template <class T, class Alloc>
//deque<T, Alloc>::deque(const deque& x, const Alloc& alloc) : alloc_(alloc)
//{
    //size_ = x.size_;
    //for (size_t i = 0; i < x.size_; ++i)
        //alloc_.construct(ptr_ + i, *(x.ptr_ + i));
//}
//// move
//template <class T, class Alloc>
//deque<T, Alloc>::deque(deque&& x) : deque(move(x), Alloc())
//{
//}
//template <class T, class Alloc>
//deque<T, Alloc>::deque(deque&& x, const Alloc& alloc) : alloc_(alloc)
//{
    //ptr_ = x.ptr_;
    //size_ = x.size_;
    //capacity_ = x.capacity_;
//}
//// list
//template <class T, class Alloc>
//deque<T, Alloc>::deque(std::initializer_list<T> il, const Alloc& alloc)
    //: deque(il.begin(), il.end(), alloc)
//{
//}
//// dtor
//template <class T, class Alloc>
//deque<T, Alloc>::~deque() noexcept
//{
    //for (size_t i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    //alloc_.deallocate(ptr_, capacity_);
//}
//// ==
//template <class T, class Alloc>
//deque<T, Alloc>&
//deque<T, Alloc>::operator=(const deque& x)
//{
    //deque(x).swap(*this);
    //return *this;
//}

//template <class T, class Alloc>
//deque<T, Alloc>&
//deque<T, Alloc>::operator=(deque&& x)
//{
    //deque(move(x)).swap(*this);
    //return *this;
//}

//template <class T, class Alloc>
//deque<T, Alloc>&
//deque<T, Alloc>::operator=(std::initializer_list<T> il)
//{
    //deque(il).swap(*this);
    //return *this;
//}
//// assign
//template <class T, class Alloc>
//template <class InputIterator>
//void
//deque<T, Alloc>::assign(
    //InputIterator first, InputIterator last,
    //typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
//{
    //clear();
    //for (; first != last; ++first) push_back(*first);
//}
//template <class T, class Alloc>
//void
//deque<T, Alloc>::assign(size_t n, const T& val)
//{
    //clear();
    //for (size_t i = 0; i < n; ++i) alloc_.construct(ptr_ + i, val);
    //size_ = n;
//}
//template <class T, class Alloc>
//void
//deque<T, Alloc>::assign(std::initializer_list<T> il)
//{
    //clear();
    //for (auto it = il.begin(); it != il.end(); ++it)
    //{
        //alloc_.construct(ptr_ + size_, *it);
        //++size_;
    //}
//}
//// get_allocator
//template <class T, class Alloc>
//Alloc
//deque<T, Alloc>::get_allocator() const noexcept
//{
    //return alloc_;
//}

////============================= element access ===============================//
//// at
//template <class T, class Alloc>
//T&
//deque<T, Alloc>::at(size_t n)
//{
    //if (n >= size_) throw std::out_of_range("out of range");
    //return ptr_[n];
//}
//template <class T, class Alloc>
//const T&
//deque<T, Alloc>::at(size_t n) const
//{
    //if (n >= size_) throw std::out_of_range("out of range");
    //return ptr_[n];
//}
//// []
//template <class T, class Alloc>
//T& deque<T, Alloc>::operator[](size_t n)
//{
    //return ptr_[n];
//}
//template <class T, class Alloc>
//const T& deque<T, Alloc>::operator[](size_t n) const
//{
    //return ptr_[n];
//}
//// front
//template <class T, class Alloc>
//T&
//deque<T, Alloc>::front()
//{
    //return ptr_[0];
//}
//template <class T, class Alloc>
//const T&
//deque<T, Alloc>::front() const
//{
    //return ptr_[0];
//}
//// back
//template <class T, class Alloc>
//T&
//deque<T, Alloc>::back()
//{
    //return ptr_[size_ - 1];
//}
//template <class T, class Alloc>
//const T&
//deque<T, Alloc>::back() const
//{
    //return ptr_[size_ - 1];
//}

////================================ iterators =================================//
//template <class T, class Alloc>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::begin() noexcept
//{
    //return ptr_;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::end() noexcept
//{
    //return ptr_ + size_;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_iterator
//deque<T, Alloc>::begin() const noexcept
//{
    //return ptr_;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_iterator
//deque<T, Alloc>::end() const noexcept
//{
    //return ptr_ + size_;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_iterator
//deque<T, Alloc>::cbegin() const noexcept
//{
    //return ptr_;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_iterator
//deque<T, Alloc>::cend() const noexcept
//{
    //return ptr_ + size_;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::reverse_iterator
//deque<T, Alloc>::rbegin() noexcept
//{
    //return reverse_iterator(end());
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::reverse_iterator
//deque<T, Alloc>::rend() noexcept
//{
    //return reverse_iterator(begin());
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_reverse_iterator
//deque<T, Alloc>::rbegin() const noexcept
//{
    //return const_reverse_iterator(end());
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_reverse_iterator
//deque<T, Alloc>::rend() const noexcept
//{
    //return const_reverse_iterator(begin());
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_reverse_iterator
//deque<T, Alloc>::crbegin() const noexcept
//{
    //return const_reverse_iterator(end());
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::const_reverse_iterator
//deque<T, Alloc>::crend() const noexcept
//{
    //return const_reverse_iterator(begin());
//}

////================================ capacity ==================================//
//template <class T, class Alloc>
//bool
//deque<T, Alloc>::empty() const noexcept
//{
    //return size_ == 0;
//}
//template <class T, class Alloc>
//size_t
//deque<T, Alloc>::size() const noexcept
//{
    //return size_;
//}
//template <class T, class Alloc>
//size_t
//deque<T, Alloc>::max_size() const
//{
    //return std::numeric_limits<size_t>::max();
//}
//template <class T, class Alloc>
//void
//deque<T, Alloc>::shrink_to_fit()
//{
    //if (size_ == 0)
        //alloc_.deallocate(ptr_, capacity_);
    //else
        //realloc(size_);
//}

////================================ modifiers =================================//
//// clear
//template <class T, class Alloc>
//void
//deque<T, Alloc>::clear()
//{
    //for (size_t i = 0; i < size_; ++i) alloc_.destroy(ptr_ + i);
    //size_ = 0;
//}
//// insert
//template <class T, class Alloc>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::insert(const_iterator position, const T& val)
//{
    //difference_type diff = position - ptr_;
    //++size_;
    //T* pos = ptr_ + diff;
    //memmove((void*)(pos + 1), (const void*)(pos),
            //(size_ - (pos - ptr_ + 1)) * sizeof(T));
    //alloc_.construct(pos, val);
    //return pos;
//}
//template <class T, class Alloc>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::insert(const_iterator position, T&& val)
//{
    //difference_type diff = position - ptr_;
    //++size_;
    //T* pos = ptr_ + diff;
    //memmove((void*)(pos + 1), (const void*)(pos),
            //(size_ - (pos - ptr_ + 1)) * sizeof(T));
    //alloc_.construct(pos, move(val));
    //return pos;
//}
//template <class T, class Alloc>
//void
//deque<T, Alloc>::insert(const_iterator position, size_t n, const T& val)
//{
    //difference_type diff = position - ptr_;
    //size_ += n;
    //T* pos = ptr_ + diff;
    //memmove((void*)(pos + n), (const void*)(pos),
            //(size_ - (pos - ptr_ + 1)) * sizeof(T));
    //for (size_t i = 0; i < n; ++i) alloc_.construct(pos + i, val);
//}
//template <class T, class Alloc>
//template <class InputIterator>
//void
//deque<T, Alloc>::insert(
    //const_iterator position, InputIterator first, InputIterator last,
    //typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
//{
    //iterator pos = ptr_ + (position - ptr_);
    //for (; first != last; ++first) pos = insert(pos, *first) + 1;
//}
//// erase
//template <class T, class Alloc>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::erase(const_iterator position)
//{
    //alloc_.destroy(position);
    //memmove((void*)position, (const void*)(position + 1),
            //(size_ - (position - ptr_ + 1)) * sizeof(T));
    //--size_;
    //return ptr_ + (position - ptr_);
//}

//template <class T, class Alloc>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::erase(const_iterator first, const_iterator last)
//{
    //for (auto iter = first; iter != last; ++iter) alloc_.destroy(iter);
    //memmove((void*)first, (const void*)last,
            //(size_ - (last - ptr_)) * sizeof(T));
    //size_ -= last - first;
    //return ptr_ + (first - ptr_);
//}
//// push_back
//template <class T, class Alloc>
//void
//deque<T, Alloc>::push_back(const T& val)
//{
    //if (capacity_ <= size_) realloc(size_ + 1);
    //alloc_.construct(ptr_ + size_, val);
    //++size_;
//}
//template <class T, class Alloc>
//void
//deque<T, Alloc>::push_back(T&& val)
//{
    //if (capacity_ <= size_) realloc(size_ + 1);
    //alloc_.construct(ptr_ + size_, move(val));
    //++size_;
//}
//// emplace_back
//template <class T, class Alloc>
//template <class... Args>
//typename deque<T, Alloc>::iterator
//deque<T, Alloc>::emplace(const_iterator position, Args&&... args)
//{
    //difference_type diff = position - ptr_;
    //++size_;
    //T* pos = ptr_ + diff;
    //memmove((void*)(pos + 1), (const void*)(pos),
            //(size_ - (pos - ptr_ + 1)) * sizeof(T));
    //alloc_.construct(pos, forward<Args>(args)...);
    //return pos;
//}
//template <class T, class Alloc>
//template <class... Args>
//void
//deque<T, Alloc>::emplace_back(Args&&... args)
//{
    //++size_;
    //alloc_.construct(ptr_ + size_ - 1, forward<Args>(args)...);
//}
//// pop_back
//template <class T, class Alloc>
//void
//deque<T, Alloc>::pop_back()
//{
    //--size_;
    //alloc_.destroy(ptr_ + size_);
//}
//// resize
//template <class T, class Alloc>
//void deque<T, Alloc>::resize(size_t, T)
//{
//}
//// swap
//template <class T, class Alloc>
//void
//deque<T, Alloc>::swap(deque& x)
//{
    //mrsuyi::swap(ptr_, x.ptr_);
    //mrsuyi::swap(size_, x.size_);
    //mrsuyi::swap(capacity_, x.capacity_);
//}

////=========================== non-member functions ===========================//
//template <class T, class Alloc>
//void
//swap(deque<T, Alloc>& x, deque<T, Alloc>& y)
//{
    //x.swap(y);
//}
//template <class T, class Alloc>
//bool
//operator==(const deque<T, Alloc>& lhs, const deque<T>& rhs)
//{
    //return lhs.size() == rhs.size() &&
           //equal(lhs.begin(), lhs.end(), rhs.begin());
//}
//template <class T, class Alloc>
//bool
//operator!=(const deque<T, Alloc>& lhs, const deque<T>& rhs)
//{
    //return !(lhs == rhs);
//}
//template <class T, class Alloc>
//bool
//operator<(const deque<T, Alloc>& lhs, const deque<T>& rhs)
//{
    //return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                   //rhs.end());
//}
//template <class T, class Alloc>
//bool
//operator<=(const deque<T, Alloc>& lhs, const deque<T>& rhs)
//{
    //return !(rhs < lhs);
//}
//template <class T, class Alloc>
//bool
//operator>(const deque<T, Alloc>& lhs, const deque<T>& rhs)
//{
    //return rhs < lhs;
//}
//template <class T, class Alloc>
//bool
//operator>=(const deque<T, Alloc>& lhs, const deque<T>& rhs)
//{
    //return !(lhs < rhs);
/*}*/
}
