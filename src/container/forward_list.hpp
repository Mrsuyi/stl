#pragma once

#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <stdexcept>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"

namespace mrsuyi {
template <class T, class Alloc = allocator<T>>
class forward_list {
  struct node {
    node* nxt;
    T t;

    template <class... Args>
    node(Args&&... args) : nxt(this), t(forward<Args>(args)...) {}
  };

  template <class E>
  class iter;

  // typedefs
 public:
  using value_type = T;
  using allocator_type = typename Alloc::template other<node>;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  // iterators
  using iterator = iter<T>;
  using const_iterator = iter<const T>;

  // member-function
 public:
  // ctor & dtor
  // default
  forward_list();
  explicit forward_list(const allocator_type& alloc);
  // fill
  explicit forward_list(size_t n,
                        const allocator_type& alloc = allocator_type());
  forward_list(size_t n,
               const T& val,
               const allocator_type& alloc = allocator_type());
  // range
  template <class InputIterator>
  forward_list(
      InputIterator first,
      InputIterator last,
      const allocator_type& alloc = allocator_type(),
      typename std::enable_if<!std::is_integral<InputIterator>::value>::type* =
          0);
  // copy
  forward_list(const forward_list& x);
  forward_list(const forward_list& x, const allocator_type& alloc);
  // move
  forward_list(forward_list&& x);
  forward_list(forward_list&& x, const allocator_type& alloc);
  // forward_list
  forward_list(std::initializer_list<T> il,
               const allocator_type& alloc = allocator_type());
  // dtor
  ~forward_list() noexcept;
  // =
  forward_list& operator=(const forward_list& x);
  forward_list& operator=(forward_list&& x);
  forward_list& operator=(std::initializer_list<T> il);
  // assign
  template <class InputIterator>
  void assign(
      InputIterator first,
      InputIterator last,
      typename std::enable_if<!std::is_integral<InputIterator>::value>::type* =
          0);
  void assign(size_t n, const T& val);
  void assign(std::initializer_list<T> il);
  // alloc
  allocator_type get_allocator() const noexcept;

  // element access
  T& front();
  const T& front() const;

  // iterators
  iterator before_begin() noexcept;
  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator before_begin() const noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbefore_begin() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  // capacity
  bool empty() const noexcept;
  size_t max_size() const noexcept;

  // modifiers
  void clear();
  void resize(size_t size);
  void resize(size_t size, const T& val);
  void swap(forward_list& x);

  void push_front(const T& val);
  void push_front(T&& val);

  void pop_front();

  template <class... Args>
  iterator emplace_after(const_iterator pos, Args&&... args);
  template <class... Args>
  void emplace_front(Args&&... args);

  iterator insert_after(const_iterator pos, const T& val);
  iterator insert_after(const_iterator pos, T&& val);
  void insert_after(const_iterator pos, size_t n, const T& val);
  template <class InputIterator>
  void insert_after(
      const_iterator pos,
      InputIterator first,
      InputIterator last,
      typename std::enable_if<!std::is_integral<InputIterator>::value>::type* =
          0);
  iterator erase_after(const_iterator pos);
  iterator erase_after(const_iterator first, const_iterator last);

  // operations
  void merge(forward_list&&);
  template <class Compare>
  void merge(forward_list&&, Compare);

  void splice_after(const_iterator pos, forward_list& other);
  void splice_after(const_iterator pos, forward_list&& other);
  void splice_after(const_iterator pos, forward_list& other, const_iterator it);
  void splice_after(const_iterator pos,
                    forward_list&& other,
                    const_iterator it);
  void splice_after(const_iterator pos,
                    forward_list& other,
                    const_iterator first,
                    const_iterator last);
  void splice_after(const_iterator pos,
                    forward_list&& other,
                    const_iterator first,
                    const_iterator last);

  void remove(const T& val);
  template <class UnaryPredicate>
  void remove_if(UnaryPredicate p);

  void reverse();

  void unique();
  template <class BinaryPredicate>
  void unique(BinaryPredicate p);

  void sort();
  template <class Compare>
  void sort(Compare cmp);

 protected:
  // new/delete
  template <class... Args>
  node* new_node(Args... args);
  void del_node(node*);
  // insert [one | head-tail] into list
  node* insert_after(node* pos, node* one);
  node* insert_after(node* pos, node* head, node* tail);
  // extract [one | head-tail] from list
  node* yield_after(node* one);
  node* yield_after(node* head, node* tail);
  // erase
  node* erase_after(node* one);
  // get last node
  node* last();
  // get mid node
  node* mid();

 protected:
  allocator_type alloc_;
  node* joint_;
};

template <class T, class Alloc>
template <class E>
class forward_list<T, Alloc>::iter {
  friend class forward_list<T, Alloc>;

 public:
  using value_type = E;
  using difference_type = size_t;
  using reference = E&;
  using pointer = E*;
  using iterator_category = bidirectional_iterator_tag;

  iter() : node_(nullptr) {}
  iter(node* ptr) : node_(ptr) {}
  iter(const iter& it) : node_(it.node_) {}
  iter& operator=(const iter& it) {
    node_ = it.node_;
    return *this;
  }
  E& operator*() const { return node_->t; }
  E* operator->() const { return &(node_->t); }
  bool operator==(const iter& it) { return node_ == it.node_; }
  bool operator!=(const iter& it) { return node_ != it.node_; }
  iter& operator++() {
    node_ = node_->nxt;
    return *this;
  }
  iter operator++(int) {
    auto res = *this;
    ++*this;
    return res;
  }
  operator iter<const E>() const { return iter<const E>(node_); }

 private:
  node* node_;
};

//================================== protected ===============================//
// new/delete function
template <class T, class Alloc>
template <class... Args>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::new_node(
    Args... args) {
  node* ptr = alloc_.allocate(1);
  mrsuyi::construct(ptr, forward<Args>(args)...);
  return ptr;
}
template <class T, class Alloc>
void forward_list<T, Alloc>::del_node(node* ptr) {
  mrsuyi::destroy_at(ptr);
  alloc_.deallocate(ptr, 1);
}
// insert
template <class T, class Alloc>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::insert_after(
    node* pos,
    node* one) {
  return insert_after(pos, one, one);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::node*
forward_list<T, Alloc>::insert_after(node* pos, node* head, node* tail) {
  tail->nxt = pos->nxt;
  pos->nxt = head;
  return head;
}
// yield
template <class T, class Alloc>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::yield_after(
    node* one) {
  return yield_after(one, one->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::yield_after(
    node* pre_head,
    node* tail) {
  node* res = pre_head->nxt;
  pre_head->nxt = tail->nxt;
  return res;
}
// erase
template <class T, class Alloc>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::erase_after(
    node* one) {
  node* res = one->nxt->nxt;
  del_node(yield_after(one));
  return res;
}
// last
template <class T, class Alloc>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::last() {
  node* res = joint_;
  while (res->nxt != joint_)
    res = res->nxt;
  return res;
}
// last
template <class T, class Alloc>
typename forward_list<T, Alloc>::node* forward_list<T, Alloc>::mid() {
  node* fast = joint_;
  node* slow = fast;
  while (fast->nxt != joint_ && fast->nxt->nxt != joint_) {
    fast = fast->nxt->nxt;
    slow = slow->nxt;
  }
  return slow;
}

//==================================== basic =================================//
// default
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list() : forward_list(allocator_type()) {}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(const allocator_type& alloc)
    : joint_(new_node()), alloc_(alloc) {}
// fill
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(size_t n, const allocator_type& alloc)
    : forward_list(n, T(), alloc) {}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(size_t n,
                                     const T& val,
                                     const allocator_type& alloc)
    : forward_list(alloc) {
  for (size_t i = 0; i < n; ++i)
    push_front(val);
}
// range
template <class T, class Alloc>
template <class InputIterator>
forward_list<T, Alloc>::forward_list(
    InputIterator first,
    InputIterator last,
    const allocator_type& alloc,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*)
    : forward_list(alloc) {
  for (auto ins = before_begin(); first != last; ++first)
    ins = insert_after(ins, *first);
}
// copy
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(const forward_list& x)
    : forward_list(x, allocator_type()) {}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(const forward_list& x,
                                     const allocator_type& alloc)
    : forward_list(x.begin(), x.end(), alloc) {}
// move
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(forward_list&& x)
    : forward_list(move(x), allocator_type()) {}
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(forward_list&& x,
                                     const allocator_type& alloc)
    : joint_(x.joint_), alloc_(alloc) {
  x.joint_ = new_node();
}
// forward_list
template <class T, class Alloc>
forward_list<T, Alloc>::forward_list(std::initializer_list<T> il,
                                     const allocator_type& alloc)
    : forward_list(il.begin(), il.end(), alloc) {}
// dtor
template <class T, class Alloc>
forward_list<T, Alloc>::~forward_list() noexcept {
  node* it = joint_;
  do {
    node* tmp = it;
    it = it->nxt;
    del_node(tmp);
  } while (it != joint_);
}

// =
template <class T, class Alloc>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(
    const forward_list& x) {
  forward_list(x).swap(*this);
  return *this;
}
template <class T, class Alloc>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(forward_list&& x) {
  forward_list(move(x)).swap(*this);
  return *this;
}
template <class T, class Alloc>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(
    std::initializer_list<T> il) {
  forward_list(il).swap(*this);
  return *this;
}
// assign
template <class T, class Alloc>
template <class InputIterator>
void forward_list<T, Alloc>::assign(
    InputIterator first,
    InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*) {
  forward_list(first, last).swap(*this);
}
template <class T, class Alloc>
void forward_list<T, Alloc>::assign(size_t n, const T& val) {
  forward_list(n, val).swap(*this);
}
template <class T, class Alloc>
void forward_list<T, Alloc>::assign(std::initializer_list<T> il) {
  forward_list(il).swap(*this);
}
// alloc
template <class T, class Alloc>
typename forward_list<T, Alloc>::allocator_type
forward_list<T, Alloc>::get_allocator() const noexcept {
  return alloc_;
}

//=============================== element access =============================//
template <class T, class Alloc>
T& forward_list<T, Alloc>::front() {
  return *(this->begin());
}
template <class T, class Alloc>
const T& forward_list<T, Alloc>::front() const {
  return *(this->begin());
}

//================================== iterators ===============================//
// non-const before_begin/begin/end
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::before_begin() noexcept {
  return iterator(joint_);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::begin() noexcept {
  return iterator(joint_->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::end() noexcept {
  return iterator(joint_);
}
// const before_begin/begin/end
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::before_begin() const noexcept {
  return const_iterator(joint_);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::begin()
    const noexcept {
  return const_iterator(joint_->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::end()
    const noexcept {
  return const_iterator(joint_);
}
// cbefore_begin/cbegin/cend
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator
forward_list<T, Alloc>::cbefore_begin() const noexcept {
  return const_iterator(joint_);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::cbegin()
    const noexcept {
  return const_iterator(joint_->nxt);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::cend()
    const noexcept {
  return const_iterator(joint_);
}

//================================== capacity ================================//
template <class T, class Alloc>
bool forward_list<T, Alloc>::empty() const noexcept {
  return joint_->nxt == joint_;
}
template <class T, class Alloc>
size_t forward_list<T, Alloc>::max_size() const noexcept {
  return std::numeric_limits<size_t>::max();
}

//================================== modifiers ===============================//
template <class T, class Alloc>
void forward_list<T, Alloc>::clear() {
  resize(0);
}
template <class T, class Alloc>
void forward_list<T, Alloc>::resize(size_t size) {
  resize(size, T());
}
template <class T, class Alloc>
void forward_list<T, Alloc>::resize(size_t size, const T& val) {
  size_t cnt = 0;
  auto pre = before_begin(), cur = begin();
  for (; cnt < size && cur != end(); ++cnt, ++pre, ++cur)
    ;
  if (cnt == size)
    for (; cur != end(); cur = erase_after(pre))
      ;
  else
    for (; cnt < size; ++cnt, insert_after(pre, val))
      ;
}
template <class T, class Alloc>
void forward_list<T, Alloc>::swap(forward_list& x) {
  mrsuyi::swap(joint_, x.joint_);
  mrsuyi::swap(alloc_, x.alloc_);
}

// push/pop -- front/back
template <class T, class Alloc>
void forward_list<T, Alloc>::push_front(const T& val) {
  emplace_front(val);
}
template <class T, class Alloc>
void forward_list<T, Alloc>::push_front(T&& val) {
  emplace_front(move(val));
}
template <class T, class Alloc>
void forward_list<T, Alloc>::pop_front() {
  erase_after(before_begin());
}

// emplace front/back
template <class T, class Alloc>
template <class... Args>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::emplace_after(
    const_iterator pos,
    Args&&... args) {
  return iterator(insert_after(pos.node_, new_node(forward<Args>(args)...)));
}
template <class T, class Alloc>
template <class... Args>
void forward_list<T, Alloc>::emplace_front(Args&&... args) {
  insert_after(joint_, new_node(forward<Args>(args)...));
}

// insert/erase
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::insert_after(
    const_iterator pos,
    const T& val) {
  return emplace_after(pos, val);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::insert_after(
    const_iterator pos,
    T&& val) {
  return emplace_after(pos, move(val));
}
template <class T, class Alloc>
void forward_list<T, Alloc>::insert_after(const_iterator pos,
                                          size_t n,
                                          const T& val) {
  for (size_t i = 0; i < n; ++i)
    insert_after(pos, val);
}
template <class T, class Alloc>
template <class InputIterator>
void forward_list<T, Alloc>::insert_after(
    const_iterator pos,
    InputIterator first,
    InputIterator last,
    typename std::enable_if<!std::is_integral<InputIterator>::value>::type*) {
  for (; first != last; ++first)
    pos = insert_after(pos, *first);
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::erase_after(
    const_iterator pos) {
  return iterator(erase_after(pos.node_));
}
template <class T, class Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::erase_after(
    const_iterator first,
    const_iterator last) {
  auto it = first;
  ++it;
  for (; it != last; it = erase_after(first)) {
  }
  return iterator(last.node_);
}

//================================== operations ==============================//
// merge
template <class T, class Alloc>
void forward_list<T, Alloc>::merge(forward_list&& other) {
  merge(move(other), [](const T& lhs, const T& rhs) { return lhs < rhs; });
}
template <class T, class Alloc>
template <class Compare>
void forward_list<T, Alloc>::merge(forward_list&& other, Compare cmp) {
  // use three pointer to merge
  node* tail = joint_;
  node* pthis = joint_->nxt;
  node* pthat = other.joint_->nxt;
  while (pthis != joint_ && pthat != other.joint_) {
    if (cmp(pthis->t, pthat->t)) {
      tail->nxt = pthis;
      tail = pthis;
      pthis = pthis->nxt;
    } else {
      tail->nxt = pthat;
      tail = pthat;
      pthat = pthat->nxt;
    }
  }
  // append rest of other to this
  if (pthat != other.joint_) {
    tail->nxt = pthat;
    other.joint_->nxt = joint_->nxt;
    joint_->nxt = joint_;
    mrsuyi::swap(joint_, other.joint_);
  } else {
    tail->nxt = pthis;
    other.joint_->nxt = other.joint_;
  }
}
// splice_after
template <class T, class Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos,
                                          forward_list& other) {
  splice_after(pos, move(other));
}
template <class T, class Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos,
                                          forward_list&& other) {
  if (other.joint_->nxt == other.joint_)
    return;
  insert_after(pos.node_, other.joint_->nxt, other.last());
  other.joint_->nxt = other.joint_;
}
template <class T, class Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos,
                                          forward_list& other,
                                          const_iterator it) {
  splice_after(pos, move(other), it);
}
template <class T, class Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos,
                                          forward_list&& other,
                                          const_iterator it) {
  insert_after(pos.node_, other.yield_after(it.node_));
}
template <class T, class Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos,
                                          forward_list& other,
                                          const_iterator first,
                                          const_iterator last) {
  splice_after(pos, move(other), first, last);
}
template <class T, class Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos,
                                          forward_list&& other,
                                          const_iterator first,
                                          const_iterator last) {
  node* pre_head = first.node_;
  node* tail = pre_head;
  while (tail->nxt != last.node_)
    tail = tail->nxt;
  node* head = other.yield_after(pre_head, tail);
  insert_after(pos.node_, head, tail);
}
// remove
template <class T, class Alloc>
void forward_list<T, Alloc>::remove(const T& val) {
  remove_if([&val](const T& t) { return val == t; });
}
template <class T, class Alloc>
template <class UnaryPredicate>
void forward_list<T, Alloc>::remove_if(UnaryPredicate p) {
  for (auto pre = before_begin(), cur = begin(); cur != end();) {
    if (p(*cur))
      cur = erase_after(pre);
    else {
      ++pre;
      ++cur;
    }
  }
}
// unique
template <class T, class Alloc>
void forward_list<T, Alloc>::unique() {
  unique([](const T& a, const T& b) { return a == b; });
}
template <class T, class Alloc>
template <class BinaryPredicate>
void forward_list<T, Alloc>::unique(BinaryPredicate p) {
  for (auto pre = begin(), cur = ++begin(); cur != end();) {
    if (p(*pre, *cur))
      cur = erase_after(pre);
    else {
      ++pre;
      ++cur;
    }
  }
}

template <class T, class Alloc>
void forward_list<T, Alloc>::sort() {
  sort([](const T& a, const T& b) { return a < b; });
}
template <class T, class Alloc>
template <class Compare>
void forward_list<T, Alloc>::sort(Compare p) {
  node* m = mid();
  if (m == joint_)
    return;

  forward_list tmp;
  tmp.splice_after(tmp.before_begin(), *this, before_begin(), ++iterator(m));

  tmp.sort(p);
  sort();

  merge(move(tmp), p);
}

//=========================== non-member functions ===========================//
template <class T, class Alloc>
void swap(forward_list<T, Alloc>& lhs, forward_list<T, Alloc>& rhs) {
  lhs.swap(rhs);
}
template <class T, class Alloc>
bool operator==(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs) {
  return (distance(lhs.begin(), lhs.end()) ==
          distance(rhs.begin(), rhs.end())) &&
         equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <class T, class Alloc>
bool operator!=(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs) {
  return !(lhs == rhs);
}
template <class T, class Alloc>
bool operator<(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs) {
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                 rhs.end());
}
template <class T, class Alloc>
bool operator<=(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs) {
  return !(rhs < lhs);
}
template <class T, class Alloc>
bool operator>(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs) {
  return rhs < lhs;
}
template <class T, class Alloc>
bool operator>=(const forward_list<T, Alloc>& lhs, const forward_list<T>& rhs) {
  return !(lhs < rhs);
}
}  // namespace mrsuyi
