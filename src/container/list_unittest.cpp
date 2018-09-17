#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "list.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(ListTest, Constructor) {
  // initializer_list
  list<int> l1 = {1, 2, 3};
  EXPECT_THAT(l1, ElementsAre(1, 2, 3));
  // fill
  list<int> l2(3);
  EXPECT_THAT(l2, ElementsAre(0, 0, 0));
  // fill val
  list<int> l3(3, 1);
  EXPECT_THAT(l3, ElementsAre(1, 1, 1));
  // iter
  int ary[] = {1, 2, 3};
  list<int> l4(ary, ary + size(ary));
  EXPECT_THAT(l4, ElementsAre(1, 2, 3));
  // copy
  list<int> l5(l4);
  EXPECT_THAT(l5, ElementsAre(1, 2, 3));
  // move
  list<int> l6(move(l5));
  EXPECT_THAT(l6, ElementsAre(1, 2, 3));
}

TEST(ListTest, Assign) {
  list<int> li;
  // fill val
  li.assign(3, 1);
  EXPECT_THAT(li, ElementsAre(1, 1, 1));
  // fill initializer_list
  li.assign({1, 2, 3});
  EXPECT_THAT(li, ElementsAre(1, 2, 3));
}

TEST(ListTest, AssignOperator) {
  list<int> a = {1, 2, 3};
  list<int> b = a;
  EXPECT_THAT(b, ElementsAre(1, 2, 3));
}

TEST(ListTest, ElementAccess) {
  list<int> li = {1, 2, 3};
  EXPECT_EQ(1, li.front());
  EXPECT_EQ(3, li.back());
}

TEST(ListTest, SizeEmpty) {
  list<int> a;
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(0, a.size());
  a = {1, 2};
  EXPECT_FALSE(a.empty());
  EXPECT_EQ(2, a.size());
}

TEST(ListTest, PushPop) {
  list<int> li;
  li.push_front(100);
  EXPECT_THAT(li, ElementsAre(100));
  li.push_back(999);
  EXPECT_THAT(li, ElementsAre(100, 999));
  li.pop_front();
  EXPECT_THAT(li, ElementsAre(999));
  li.pop_back();
  EXPECT_TRUE(li.empty());
}

TEST(ListTest, Insert) {
  list<int> li;
  li.insert(li.begin(), 1);
  li.insert(li.end(), 2, 88);
  EXPECT_THAT(li, ElementsAre(1, 88, 88));
  int ary[] = {555, 666};
  li.insert(++li.begin(), ary, ary + size(ary));
  EXPECT_THAT(li, ElementsAre(1, 555, 666, 88, 88));
}

TEST(ListTest, Erase) {
  list<int> li = {1, 2, 3, 4, 5};
  li.erase(++li.begin());
  li.erase(++++li.begin(), li.end());
  EXPECT_THAT(li, ElementsAre(1, 3));
}

TEST(ListTest, Merge) {
  list<int> l1 = {1, 3, 5};
  list<int> l2 = {2, 4, 6};
  l1.merge(move(l2));
  EXPECT_THAT(l1, ElementsAre(1, 2, 3, 4, 5, 6));
  EXPECT_THAT(l2, ElementsAre());

  l1 = {1, 2, 3};
  l2 = {};
  l1.merge(move(l2));
  EXPECT_THAT(l1, ElementsAre(1, 2, 3));
  EXPECT_THAT(l2, ElementsAre());
  l2.merge(move(l1));
  EXPECT_THAT(l2, ElementsAre(1, 2, 3));
  EXPECT_THAT(l1, ElementsAre());

  l1 = {1, 5};
  l2 = {2, 3, 4};
  l2.merge(move(l1));
  EXPECT_THAT(l2, ElementsAre(1, 2, 3, 4, 5));
  EXPECT_THAT(l1, ElementsAre());
}

TEST(ListTest, Splic) {
  list<int> l1 = {1, 2};
  list<int> l2 = {3, 4};
  l1.splice(++l1.begin(), l2);
  EXPECT_THAT(l1, ElementsAre(1, 3, 4, 2));
  EXPECT_THAT(l2, ElementsAre());

  l1 = {1, 2, 3};
  l2 = {4, 5, 6};
  l1.splice(l1.end(), l2, ++l2.begin());
  EXPECT_THAT(l1, ElementsAre(1, 2, 3, 5));
  EXPECT_THAT(l2, ElementsAre(4, 6));

  l1 = {1, 2, 3};
  l2 = {4, 5, 6};
  l1.splice(++l1.begin(), l2, ++l2.begin(), l2.end());
  EXPECT_THAT(l1, ElementsAre(1, 5, 6, 2, 3));
  EXPECT_THAT(l2, ElementsAre(4));
}

TEST(ListTest, Remove) {
  list<int> li = {1, 2, 3};
  li.remove(2);
  EXPECT_THAT(li, ElementsAre(1, 3));

  auto p = [](const int& num) { return num < 0; };
  li = {-1, 2, -2, 3};
  li.remove_if(p);
  EXPECT_THAT(li, ElementsAre(2, 3));
}

TEST(ListTest, Unique) {
  list<int> li = {};
  li.unique();
  EXPECT_THAT(li, ElementsAre());

  li = {1, 2, 2, 3, 3, 4};
  li.unique();
  EXPECT_THAT(li, ElementsAre(1, 2, 3, 4));

  li = {1, 2, -2, 3, 3, 4};
  auto p = [](const int& a, const int& b) { return a == b || a == -b; };
  li.unique(p);
  EXPECT_THAT(li, ElementsAre(1, 2, 3, 4));
}

TEST(ListTest, Sort) {
  list<int> li = {1, 5, 2, 4, 3};
  li.sort();
  EXPECT_THAT(li, ElementsAre(1, 2, 3, 4, 5));
}
