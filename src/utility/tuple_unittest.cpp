#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "tuple.hpp"

using namespace mrsuyi;

namespace {
struct Shit {
  int num;
  char chr;
};
}  // namespace

TEST(TupleTest, Constructor) {
  tuple<int, float, double> constexpr_ctor;

  tuple<int, float, double> const_ref_ctor(1, 1.1, 2.2);

  Shit s = {1, 'c'};
  tuple<int, Shit> move_ctor(123, move(s));

  tuple<int, Shit> const_ref_copy_ctor(move_ctor);

  tuple<int, Shit> move_copy_ctor(move(const_ref_copy_ctor));
}

TEST(TupleTest, Get) {
  tuple<int, float, double> tp(1, 1.1, 2.2);
  EXPECT_EQ(1, get<0>(tp));
  EXPECT_FLOAT_EQ(1.1, get<1>(tp));
  EXPECT_DOUBLE_EQ(2.2, get<2>(tp));
}

TEST(TupleTest, OperatorAssign) {
  tuple<int, float, double> tp(1, 1.1, 2.2), tp2;
  tp2 = tp;
  EXPECT_EQ(1, get<0>(tp2));
  EXPECT_FLOAT_EQ(1.1, get<1>(tp2));
  EXPECT_DOUBLE_EQ(2.2, get<2>(tp2));
}

TEST(TupleTest, Swap) {
  tuple<int, float, double> tp(1, 1.1, 2.2), tp2;
  tp2.swap(tp);
  EXPECT_EQ(0, get<0>(tp));
  EXPECT_FLOAT_EQ(0, get<1>(tp));
  EXPECT_DOUBLE_EQ(0, get<2>(tp));
  EXPECT_EQ(1, get<0>(tp2));
  EXPECT_FLOAT_EQ(1.1, get<1>(tp2));
  EXPECT_DOUBLE_EQ(2.2, get<2>(tp2));
}
