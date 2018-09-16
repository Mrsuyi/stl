#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "heap.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(HeapTest, PushHeap) {
  int a[] = {5, 1, 2, 4, 8};
  push_heap(a, a + 5);
  EXPECT_THAT(a, ElementsAreArray({8, 5, 2, 4, 1}));

  int b[] = {1};
  push_heap(b, b + 1);
  EXPECT_THAT(b, ElementsAreArray({1}));

  int c[] = {};
  push_heap(c, c);
}

TEST(HeapTest, PopHeap) {
  int a[] = {8, 5, 2, 4, 1};
  pop_heap(a, a + 5);
  EXPECT_THAT(a, ElementsAreArray({5, 4, 2, 1, 8}));

  int b[] = {1};
  pop_heap(b, b + 1);
  EXPECT_THAT(b, ElementsAreArray({1}));

  int c[] = {};
  pop_heap(c, c);
}

TEST(HeapTest, SortHeap) {
  int a[] = {8, 5, 2, 4, 1};
  sort_heap(a, a + 5);
  EXPECT_THAT(a, ElementsAreArray({1, 2, 4, 5, 8}));
}

/*TEST(HeapTest, MakeHeap) {*/
// int a[] = {1, 2, 3, 4, 5};
// make_heap(a, a + 5);
// EXPECT_THAT(a, ElementsAreArray({5, 4, 3, 1, 2}));

// int b[] = {1, 2, 3};
// make_heap(b, b + 3);
// EXPECT_THAT(b, ElementsAreArray({3, 2, 1}));

// int c[] = {};
// make_heap(c, c);
/*}*/

TEST(HeapTest, IsHeap) {
  int a[] = {8, 5, 2, 4, 1};
  EXPECT_TRUE(is_heap(a, a + 5));

  int b[] = {1, 2, 3, 4, 5};
  EXPECT_FALSE(is_heap(b, b + 5));
}

TEST(HeapTest, IsHeapUntil) {
  int a[] = {2, 1, 0};
  EXPECT_EQ(3, is_heap_until(a, a + 3) - a);

  int b[] = {5, 4, 3, 8};
  EXPECT_EQ(3, is_heap_until(b, b + 4) - b);

  int c[] = {5, 4, 3, 1, 9};
  EXPECT_EQ(4, is_heap_until(c, c + 5) - c);
}
