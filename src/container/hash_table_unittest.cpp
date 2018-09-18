#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "hash_table.hpp"

using namespace mrsuyi;
using namespace testing;

template <class T>
void insert_equal(hash_table<T>& ht, std::initializer_list<T> il) {
  for (auto it = il.begin(); it != il.end(); ++it)
    ht.emplace_equal(*it);
}
template <class T>
void insert_unique(hash_table<T>& ht, std::initializer_list<T> il) {
  for (auto it = il.begin(); it != il.end(); ++it)
    ht.emplace_unique(*it);
}

TEST(HashTableTest, Constructor) {
  // default
  hash_table<int> ht;
  ht.emplace_equal(1);
  ht.emplace_equal(1);
  ht.emplace_equal(2);
  ht.emplace_equal(2);
  EXPECT_THAT(ht, UnorderedElementsAre(1, 1, 2, 2));
  // copy
  auto cp(ht);
  EXPECT_THAT(cp, UnorderedElementsAre(1, 1, 2, 2));
  EXPECT_THAT(ht, UnorderedElementsAre(1, 1, 2, 2));
  // move
  auto mv(mrsuyi::move(ht));
  EXPECT_THAT(mv, UnorderedElementsAre(1, 1, 2, 2));
  assert(ht.empty());

  // swap
  mv.swap(ht);
  EXPECT_THAT(ht, UnorderedElementsAre(1, 1, 2, 2));
  assert(mv.empty());

  // = copy
  mv = ht;
  EXPECT_THAT(ht, UnorderedElementsAre(1, 1, 2, 2));
  EXPECT_THAT(mv, UnorderedElementsAre(1, 1, 2, 2));

  //// = move
  mv = mrsuyi::move(ht);
  EXPECT_THAT(mv, UnorderedElementsAre(1, 1, 2, 2));
  assert(ht.empty());
}

TEST(HashTableTest, Emplace) {
  hash_table<int> ht;
  assert(ht.bucket_count() == 5);
  ht.emplace_equal(1);
  ht.emplace_equal(1);
  ht.emplace_equal(2);
  ht.emplace_equal(2);
  ht.emplace_equal(3);
  ht.emplace_equal(3);
  assert(ht.bucket_count() == 11);
  EXPECT_THAT(ht, UnorderedElementsAre(1, 1, 2, 2, 3, 3));

  ht.clear();
  ht.emplace_unique(1);
  ht.emplace_unique(1);
  ht.emplace_unique(2);
  ht.emplace_unique(2);
  ht.emplace_unique(3);
  ht.emplace_unique(3);
  EXPECT_THAT(ht, UnorderedElementsAre(1, 2, 3));
}

TEST(HashTableTest, Insert) {
  hash_table<int> ht;
  insert_equal(ht, {1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9});
  EXPECT_THAT(ht, UnorderedElementsAre(1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9));
  ht.clear();
  insert_unique(ht, {1, 2, 2, 3, 4, 5, 6, 7, 7, 8, 9});
  EXPECT_THAT(ht, UnorderedElementsAre(1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST(HashTableTest, Erase) {
  // single
  hash_table<int> ht;
  insert_equal(ht, {1, 1, 2, 2, 3, 3, 4});
  *ht.erase(ht.begin());
  EXPECT_THAT(ht, UnorderedElementsAre(1, 2, 2, 3, 3, 4));

  // range
  ht.erase(ht.begin(), ++++++ht.begin());
  EXPECT_THAT(ht, UnorderedElementsAre(3, 3, 4));

  // key-equal
  assert(ht.erase_equal(3) == 2);
  EXPECT_THAT(ht, UnorderedElementsAre(4));
  assert(ht.erase_equal(10) == 0);
  EXPECT_THAT(ht, UnorderedElementsAre(4));

  // key-unique
  insert_equal(ht, {8, 8});
  assert(ht.erase_unique(4) == 1);
  EXPECT_THAT(ht, UnorderedElementsAre(8, 8));
  assert(ht.erase_unique(8) == 1);
  EXPECT_THAT(ht, UnorderedElementsAre(8));
  assert(ht.erase_unique(8) == 1);
  assert(ht.empty());
}

TEST(HashTableTest, Find) {
  hash_table<int> ht;
  insert_equal(ht, {1, 1, 2, 2, 3, 3});

  assert(ht.count_equal(1) == 2);

  assert(*ht.find(1) == 1);

  auto p = ht.equal_range(1);
  auto it = p.first;
  assert(*it++ == 1);
  assert(*it++ == 1);
  assert(it == p.second);

  p = ht.equal_range(3);
  it = p.first;
  assert(*it++ == 3);
  assert(*it++ == 3);
  assert(it == p.second);
}
