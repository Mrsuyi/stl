#include "gtest/gtest.h"

#include "mem_fn.hpp"

using namespace mrsuyi;

struct Class {
  void nop() {}
  int add(int& num) { return ++num; }
};

TEST(MemFnTest, LRef) {
  Class c;

  auto nop = mem_fn(&Class::nop);
  nop(c);

  auto add = mem_fn(&Class::add);
  int num = 0;
  EXPECT_EQ(1, add(c, num));
  EXPECT_EQ(1, num);
}

TEST(MemFnTest, RRef) {
  Class c;

  auto nop = mem_fn(&Class::nop);
  nop(move(c));

  auto add = mem_fn(&Class::add);
  int num = 0;
  EXPECT_EQ(1, add(move(c), num));
  EXPECT_EQ(1, num);
}

TEST(MemFnTest, Pointer) {
  Class c;

  auto nop = mem_fn(&Class::nop);
  nop(&c);

  auto add = mem_fn(&Class::add);
  int num = 0;
  EXPECT_EQ(1, add(&c, num));
  EXPECT_EQ(1, num);
}
