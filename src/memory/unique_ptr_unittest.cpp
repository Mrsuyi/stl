#include "gtest/gtest.h"

#include "unique_ptr.hpp"

using namespace mrsuyi;

TEST(UniquePtrTest, ConstructorAndAccess) {
  unique_ptr<int> default_ctor;

  unique_ptr<int> ptr(new int(123));
  EXPECT_TRUE(ptr);
  EXPECT_TRUE(ptr.get());
  EXPECT_EQ(123, *ptr);

  int* raw = ptr.release();
  EXPECT_EQ(123, *raw);
  EXPECT_FALSE(ptr);
  EXPECT_FALSE(ptr.get());

  ptr.reset(raw);
  EXPECT_TRUE(ptr);
  EXPECT_TRUE(ptr.get());
  EXPECT_EQ(123, *ptr);
}

TEST(UniquePtrTest, ConvertToBool) {
  unique_ptr<int> a;
  EXPECT_FALSE(a);
  a.reset(new int);
  EXPECT_TRUE(a);
}
