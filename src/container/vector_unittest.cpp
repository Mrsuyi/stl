#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "vector.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(VectorTest, Ctor) {
  // default
  vector<int> dft;
  // fill
  vector<int> fill_dft(1);
  EXPECT_THAT(fill_dft, ElementsAre(0));
  vector<int> fill(3, 1);
  EXPECT_THAT(fill, ElementsAre(1, 1, 1));
  // range
  int data[] = {1, 2, 3};
  vector<int> iter(data, data + 3);
  EXPECT_THAT(iter, ElementsAre(1, 2, 3));
  // copy
  vector<int> cp(iter);
  EXPECT_THAT(cp, ElementsAre(1, 2, 3));
  //// move
  vector<int> mv(move(iter));
  EXPECT_THAT(mv, ElementsAre(1, 2, 3));
  vector<int> mv_from_rvalue(vector<int>({1, 2, 3}));
  EXPECT_THAT(mv_from_rvalue, ElementsAre(1, 2, 3));
  //// = init
  vector<int> il = {1, 2, 3};
  EXPECT_THAT(il, ElementsAre(1, 2, 3));
}

TEST(VectorTest, Assign) {
  vector<int> ary = {1};
  // size, val
  ary.assign(3, 1);
  EXPECT_THAT(ary, ElementsAre(1, 1, 1));
  // [first, last)
  int data[] = {1, 2, 3};
  ary.assign(data, data + 3);
  EXPECT_THAT(ary, ElementsAre(1, 2, 3));
  // {initializer_list}
  ary.assign({1, 2, 3});
  EXPECT_THAT(ary, ElementsAre(1, 2, 3));
}

TEST(VectorTest, SizeAndCapacity) {
  vector<int> a;
  EXPECT_EQ(0, a.capacity());
  EXPECT_EQ(0, a.size());
  a.reserve(4);
  EXPECT_EQ(4, a.capacity());
  EXPECT_EQ(0, a.size());
  a.assign(4, 1);
  EXPECT_EQ(4, a.capacity());
  EXPECT_EQ(4, a.size());
  a.push_back(1);
  EXPECT_EQ(8, a.capacity());
  EXPECT_EQ(5, a.size());
  a.reserve(13);
  EXPECT_EQ(16, a.capacity());
  EXPECT_EQ(5, a.size());
  // resize
  a.resize(10, 1);
  EXPECT_EQ(16, a.capacity());
  EXPECT_EQ(10, a.size());
  a.resize(5, 1);
  EXPECT_EQ(16, a.capacity());
  EXPECT_EQ(5, a.size());
  // shrink
  a.shrink_to_fit();
  EXPECT_EQ(8, a.capacity());
  EXPECT_EQ(5, a.size());
}

TEST(VectorTest, PushEmplace) {
  vector<int> ary = {1, 2};
  ary.push_back(3);
  ary.push_back(4);
  ary.emplace_back(5);
  EXPECT_THAT(ary, ElementsAre(1, 2, 3, 4, 5));
}

TEST(VectorTest, Insert) {
  vector<int> ary(2, 1);
  ary.insert(ary.begin(), 1, 100);
  EXPECT_THAT(ary, ElementsAre(100, 1, 1));
  assert(ary.capacity() == 4);
  ary.insert(ary.begin() + 2, 2, 100);
  EXPECT_THAT(ary, ElementsAre(100, 1, 100, 100, 1));
  ary = {1, 4, 5};
  int il[2] = {2, 3};
  ary.insert(ary.begin() + 1, il, il + 2);
  EXPECT_THAT(ary, ElementsAre(1, 2, 3, 4, 5));
}

TEST(VectorTest, Erase) {
  vector<int> ary = {1, 2, 3, 4, 5};
  auto it = ary.erase(ary.begin());
  EXPECT_THAT(ary, ElementsAre(2, 3, 4, 5));
  it = ary.erase(it + 1, it + 3);
  EXPECT_THAT(ary, ElementsAre(2, 5));
}

void swap() {
  vector<int> a = {1, 2, 3};
  vector<int> b = {4, 5, 6};
  a.swap(b);
  EXPECT_THAT(a, ElementsAre(4, 5, 6));
  EXPECT_THAT(b, ElementsAre(1, 2, 3));
}
