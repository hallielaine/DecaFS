#include <cstdio>
#include <thread>
#include <future>
#include <chrono>
#include <ratio>

#include "gtest/gtest.h"

#include "locking_strategy.h"


typedef std::chrono::duration<float, std::ratio<1, 10> > BASE_DURATION;

std::future<int> locked_read(int *i, int u, int p, int f, int t, int b, int a) {
  return std::async(std::launch::async, [=]{
    int r;
    std::this_thread::sleep_for(BASE_DURATION(b));
    get_shared_lock(u, p, f, t);
    r = *i;
    std::this_thread::sleep_for(BASE_DURATION(a));
    release_lock(u, p, f);
    return r;
  });
}

std::thread locked_write(int *d, int i, int u, int p, int f, int t, int b, int a) {
  std::thread w([=]{
    std::this_thread::sleep_for(BASE_DURATION(b));
    get_exclusive_lock(u, p, f, t);
    *d = i;
    std::this_thread::sleep_for(BASE_DURATION(a));
    release_lock(u, p, f);
  });
  return w;
}

TEST(LockingTest, SimpleRead) {
  int i = 7;
  auto start = std::chrono::high_resolution_clock::now();
  auto r1 = locked_read(&i, 1, 1, 1, -1, 1, 1);
  int r1r = r1.get();
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_EQ(i, r1r);
  EXPECT_GT(end - start, std::chrono::milliseconds(150));
}

TEST(LockingTest, SimpleWrite) {
  int i = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto w1 = locked_write(&i, 7, 1, 1, 1, -1, 0, 2);
  auto r1 = locked_read(&i, 1, 2, 1, -1, 1, 0);
  int r1r = r1.get();
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_GT(end - start, BASE_DURATION(1.5));
  EXPECT_LT(end - start, BASE_DURATION(2.5));
  EXPECT_EQ(7, r1r);
  w1.join();
}

TEST(LockingTest, MultiRead) {
  int i = 7;
  auto start = std::chrono::high_resolution_clock::now();
  auto r1 = locked_read(&i, 1, 1, 1, -1, 0, 1);
  auto r2 = locked_read(&i, 1, 2, 1, -1, 0, 1);
  auto r3 = locked_read(&i, 1, 3, 1, -1, 0, 1);
  auto r4 = locked_read(&i, 1, 4, 1, -1, 0, 1);
  int r1r = r1.get();
  int r2r = r2.get();
  int r3r = r3.get();
  int r4r = r4.get();
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_GT(end - start, BASE_DURATION(0.5));
  EXPECT_LT(end - start, BASE_DURATION(1.5));
  EXPECT_EQ(7, r1r);
  EXPECT_EQ(7, r2r);
  EXPECT_EQ(7, r3r);
  EXPECT_EQ(7, r4r);
}

TEST(LockingTest, MultiUserRead) {
  int i = 7;
  auto start = std::chrono::high_resolution_clock::now();
  auto r1 = locked_read(&i, 1, 1, 1, -1, 0, 2);
  auto r2 = locked_read(&i, 1, 2, 1, -1, 0, 2);
  auto r3 = locked_read(&i, 2, 1, 1, -1, 1, 1);
  auto r4 = locked_read(&i, 2, 2, 1, -1, 1, 1);
  int r1r = r1.get();
  int r2r = r2.get();
  auto end1 = std::chrono::high_resolution_clock::now();
  int r3r = r3.get();
  int r4r = r4.get();
  auto end2 = std::chrono::high_resolution_clock::now();
  EXPECT_GT(end1 - start, BASE_DURATION(1.5));
  EXPECT_LT(end1 - start, BASE_DURATION(2.5));
  EXPECT_GT(end2 - start, BASE_DURATION(2.5));
  EXPECT_LT(end2 - start, BASE_DURATION(3.5));
  EXPECT_EQ(7, r1r);
  EXPECT_EQ(7, r2r);
  EXPECT_EQ(7, r3r);
  EXPECT_EQ(7, r4r);
}
