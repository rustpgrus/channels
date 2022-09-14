#include <gtest/gtest.h>

#include "../Queue.hpp"

TEST(Queue_test, IsEmpty) {
  core::container::Queue<int> queue;
  EXPECT_TRUE(queue.IsEmpty());
  queue.Put(1);
  EXPECT_FALSE(queue.IsEmpty());
  queue.Put(2);
  EXPECT_FALSE(queue.IsEmpty());
  queue.Take();
  EXPECT_FALSE(queue.IsEmpty());
  queue.Take();
  EXPECT_TRUE(queue.IsEmpty());
}

TEST(Queue_test, PutAndTake) {
  core::container::Queue<int> queue;
  queue.Put(1);
  EXPECT_EQ(queue.Take(), 1);
  queue.Put(5);
  EXPECT_EQ(queue.Take(), 5);
  queue.Put(6);
  queue.Put(7);
  EXPECT_EQ(queue.Take(), 6);
  EXPECT_EQ(queue.Take(), 7);
}
