#include "gtest/gtest.h"

#include "priority_queue.hpp"

using namespace mrsuyi;

namespace {
template <class T, size_t N>
void expect(const priority_queue<T>& que, const T (&x)[N]) {
  priority_queue<T> q(que);
  for (size_t i = 0; i < N; ++i) {
    EXPECT_EQ(x[i], q.top());
    q.pop();
  }
  EXPECT_TRUE(q.empty());
}
}  // namespace

TEST(PriorityQueueTest, Constructor) {
  priority_queue<int> default_ctor;
  priority_queue<int> cmp_container(less<int>(), vector<int>({1, 2, 3}));
  expect(cmp_container, {3, 2, 1});
  priority_queue<int> cp = cmp_container;
  expect(cp, {3, 2, 1});
  priority_queue<int> mv(move(cp));
  expect(mv, {3, 2, 1});
  int ary[] = {4, 5, 6};
  priority_queue<int> iter(ary, ary + size(ary));
  expect(iter, {6, 5, 4});
}

TEST(PriorityQueueTest, PushPop) {
  priority_queue<int> q(less<int>(), {1, 4, 6, 7, 5, 3, 8, 2, 9});
  q.push(10);
  q.push(14);
  q.push(15);
  q.push(12);
  q.push(13);
  q.push(11);
  expect(q, {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
}
