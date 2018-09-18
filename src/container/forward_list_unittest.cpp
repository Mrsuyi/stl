#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "forward_list.hpp"

using namespace mrsuyi;
using namespace testing;

TEST(ForwardListTest, Constructor) {
  // default
  forward_list<int> default_ctor;
  // fill
  forward_list<int> fill_default(3);
  EXPECT_THAT(fill_default, ElementsAre(0, 0, 0));
  forward_list<int> fill_val(3, 1);
  EXPECT_THAT(fill_val, ElementsAre(1, 1, 1));
  // range
  int vec[] = {1, 2, 3};
  forward_list<int> range(vec, vec + 3);
  EXPECT_THAT(range, ElementsAre(1, 2, 3));
  // copy
  forward_list<int> cp(range);
  EXPECT_THAT(cp, ElementsAre(1, 2, 3));
  // move
  forward_list<int> mv(cp);
  EXPECT_THAT(mv, ElementsAre(1, 2, 3));
}

TEST(ForwardListTest, Assign) {
  forward_list<int> raw = {1, 2, 3};

  // =
  forward_list<int> fli;
  fli = raw;
  EXPECT_THAT(fli, ElementsAre(1, 2, 3));
  fli = move(raw);
  EXPECT_THAT(fli, ElementsAre(1, 2, 3));
  fli = {4, 5, 6};
  EXPECT_THAT(fli, ElementsAre(4, 5, 6));

  // assign
  int nums[] = {7, 8, 9};
  fli.assign(nums, nums + 3);
  EXPECT_THAT(fli, ElementsAre(7, 8, 9));
  fli.assign(3, 0);
  EXPECT_THAT(fli, ElementsAre(0, 0, 0));
  fli.assign({10, 11, 12});
  EXPECT_THAT(fli, ElementsAre(10, 11, 12));
}

TEST(ForwardListTest, Front) {
  forward_list<int> fli = {1, 2, 3};
  EXPECT_EQ(1, fli.front());
}

TEST(ForwardListTest, EmptyAndResize) {
  forward_list<int> fli;
  assert(fli.empty());
  fli = {1, 2, 3};
  assert(!fli.empty());
  fli.resize(2);
  EXPECT_THAT(fli, ElementsAre(1, 2));
  fli.resize(5, 0);
  EXPECT_THAT(fli, ElementsAre(1, 2, 0, 0, 0));
}

TEST(ForwardListTest, Insert) {
  forward_list<int> fli;
  fli.insert_after(fli.before_begin(), 1);
  EXPECT_THAT(fli, ElementsAre(1));
  int nums[] = {2, 3, 4};
  fli.insert_after(fli.begin(), nums, nums + 3);
  EXPECT_THAT(fli, ElementsAre(1, 2, 3, 4));
  fli.insert_after(fli.begin(), 3, 0);
  EXPECT_THAT(fli, ElementsAre(1, 0, 0, 0, 2, 3, 4));
}

TEST(ForwardListTest, Erase) {
  forward_list<int> fli = {1, 2, 3, 4, 5};
  fli.erase_after(fli.before_begin());
  EXPECT_THAT(fli, ElementsAre(2, 3, 4, 5));
  fli.erase_after(fli.begin(), fli.end());
  EXPECT_THAT(fli, ElementsAre(2));
}

TEST(ForwardListTest, Merge) {
  forward_list<int> l1 = {1, 3, 5};
  forward_list<int> l2 = {2, 4, 6};
  l1.merge(move(l2));
  EXPECT_THAT(l1, ElementsAre(1, 2, 3, 4, 5, 6));
  assert(l2.empty());

  l1 = {1, 2, 3};
  l2 = {};
  l1.merge(move(l2));
  EXPECT_THAT(l1, ElementsAre(1, 2, 3));
  assert(l2.empty());
  l2.merge(move(l1));
  EXPECT_THAT(l2, ElementsAre(1, 2, 3));
  assert(l1.empty());

  l1 = {1, 5};
  l2 = {2, 3, 4};
  l2.merge(move(l1));
  EXPECT_THAT(l2, ElementsAre(1, 2, 3, 4, 5));
  assert(l1.empty());
}

TEST(ForwardListTest, Splice) {
  forward_list<int> l1 = {1, 2};
  forward_list<int> l2 = {3, 4};
  l1.splice_after(l1.begin(), l2);
  EXPECT_THAT(l1, ElementsAre(1, 3, 4, 2));
  assert(l2.empty());

  l1 = {1, 2, 3};
  l2 = {4, 5, 6};
  l1.splice_after(++l1.begin(), l2, l2.begin());
  EXPECT_THAT(l1, ElementsAre(1, 2, 5, 3));
  EXPECT_THAT(l2, ElementsAre(4, 6));

  l1 = {1, 2, 3};
  l2 = {4, 5, 6};
  l1.splice_after(++++l1.begin(), l2, l2.begin(), ++++l2.begin());
  EXPECT_THAT(l1, ElementsAre(1, 2, 3, 5));
  EXPECT_THAT(l2, ElementsAre(4, 6));
}

TEST(ForwardListTest, Remove) {
  forward_list<int> li = {1, 2, 3};
  li.remove(2);
  EXPECT_THAT(li, ElementsAre(1, 3));

  auto p = [](const int& num) { return num < 0; };
  li = {-1, 2, -2, 3};
  li.remove_if(p);
  EXPECT_THAT(li, ElementsAre(2, 3));
}

TEST(ForwardListTest, Unique) {
  forward_list<int> li = {};
  li.unique();
  assert(li.empty());

  li = {1, 2, 2, 3, 3, 4};
  li.unique();
  EXPECT_THAT(li, ElementsAre(1, 2, 3, 4));

  li = {1, 2, -2, 3, 3, 4};
  auto p = [](const int& a, const int& b) { return a == b || a == -b; };
  li.unique(p);
  EXPECT_THAT(li, ElementsAre(1, 2, 3, 4));
}

TEST(ForwardListTest, Sort) {
  forward_list<int> li = {1, 5, 2, 4, 3};
  li.sort();
  assert(forward_list<int>({1, 2, 3, 4, 5}) == li);
}
