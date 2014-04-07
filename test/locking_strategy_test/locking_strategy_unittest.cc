#include <cstdio>
#include <thread>
#include <future>
#include <chrono>
#include <ratio>

#include "gtest/gtest.h"

#include "locking_strategy.h"



TEST(LockingTest, HasExclusive) {
  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
}

TEST(LockingTest, HasShared) {
  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
}

TEST(LockingTest, MultiHasShared) {
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 2, 1));

  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 2, 1));

  EXPECT_EQ(0, get_shared_lock(1, 2, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 2, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 2, 1));

  EXPECT_EQ(0, release_lock(1, 2, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 2, 1));
}

TEST(LockingTest, ExclusiveDoubleRelease) {
  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(-1, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
}

TEST(LockingTest, MultiSharedDoubleRelease) {
  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(0, get_shared_lock(1, 2, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(-1, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 2, 1));

  EXPECT_EQ(0, release_lock(1, 2, 1));
  EXPECT_EQ(-1, release_lock(1, 2, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 2, 1));
}

TEST(LockingTest, SimpleExclusiveConflict) {
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(2, 2, 1));

  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(2, 2, 1));

  EXPECT_EQ(-1, get_exclusive_lock(2, 2, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(2, 2, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(2, 2, 1));

  EXPECT_EQ(0, get_exclusive_lock(2, 2, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(2, 2, 1));

  EXPECT_EQ(0, release_lock(2, 2, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(2, 2, 1));
}

TEST(LockingTest, ExclusiveSharedConflict) {
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(2, 2, 1));

  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(2, 2, 1));

  EXPECT_EQ(-1, get_shared_lock(2, 2, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(2, 2, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(2, 2, 1));

  EXPECT_EQ(0, get_shared_lock(2, 2, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(2, 2, 1));

  EXPECT_EQ(-1, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(2, 2, 1));

  EXPECT_EQ(0, release_lock(2, 2, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(2, 2, 1));
}

TEST(LockingTest, MultiUserSharedConflict) {
  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(-1, get_shared_lock(2, 2, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, get_shared_lock(2, 2, 1));
  EXPECT_EQ(0, release_lock(2, 2, 1));
}


TEST(LockingTest, ExclusiveMultiSharedConflict) {
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(2, 2, 1));
  EXPECT_EQ(0, has_shared_lock(2, 3, 1));
  EXPECT_EQ(0, has_shared_lock(2, 4, 1));

  EXPECT_EQ(0, get_shared_lock(2, 2, 1));
  EXPECT_EQ(0, get_shared_lock(2, 3, 1));
  EXPECT_EQ(0, get_shared_lock(2, 4, 1));

  EXPECT_EQ(-1, get_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(2, 2, 1));
  EXPECT_EQ(-1, get_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(2, 3, 1));
  EXPECT_EQ(-1, get_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(2, 4, 1));
  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
}

TEST(LockingTest, SameProcExclusiveConflict) {
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));

  EXPECT_EQ(-1, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
}

TEST(LockingTest, SameProcSharedConflict) {
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));

  EXPECT_EQ(-1, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
}

TEST(LockingTest, SameProcMultiSharedConflict) {
  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));
  EXPECT_EQ(-1, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, get_shared_lock(1, 2, 1));
  EXPECT_EQ(1, has_shared_lock(1, 2, 1));
  EXPECT_EQ(-1, get_shared_lock(1, 2, 1));
  EXPECT_EQ(1, has_shared_lock(1, 2, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, release_lock(1, 2, 1));
  EXPECT_EQ(0, has_shared_lock(1, 2, 1));
}

TEST(LockingTest, SameProcExclusiveSharedConflict) {
  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));

  EXPECT_EQ(-1, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));

  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
}

// Metadata Lock Tests

typedef std::chrono::duration<float, std::ratio<1, 10> > base_duration;

std::future<std::pair<int, int> >
locked_read(int *d, int *i, int u, int p, int f, int b, int a) {
  /*
   * destination
   * shared int
   * user_id
   * proc_id
   * file_id
   * before sleep duration
   * after sleep duration
   */
  return std::async(std::launch::async, [=]{
    std::pair<int, int> r;
    std::this_thread::sleep_for(base_duration(b));
    r.first = get_metadata_lock(u, p, f);
    *d = *i;
    std::this_thread::sleep_for(base_duration(a));
    r.second = release_metadata_lock(u, p, f);
    return r;
  });
}

std::future<std::pair<int, int> >
locked_write(int *d, int i, int u, int p, int f, int b, int a) {
  /*
   * destination
   * new int
   * user_id
   * proc_id
   * file_id
   * before sleep duration
   * after sleep duration
   */
  return std::async(std::launch::async, [=]{
    std::pair<int, int> r;
    std::this_thread::sleep_for(base_duration(b));
    r.first = get_metadata_lock(u, p, f);
    *d = i;
    std::this_thread::sleep_for(base_duration(a));
    r.second = release_metadata_lock(u, p, f);
    return r;
  });
}

TEST(LockingTest, SimpleRead) {
  int i = 7;
  int r = 0;;
  auto rsf = locked_read(&r, &i, 1, 1, 1, 0, 0);
  ASSERT_EQ(std::future_status::ready, rsf.wait_for(base_duration(1)));
  auto rsr = rsf.get();
  EXPECT_EQ(0, rsr.first);
  EXPECT_EQ(0, rsr.second);
  EXPECT_EQ(i, r);
}

TEST(LockingTest, SimpleWrite) {
  const int w = 7;
  int i = 0;
  int r = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto wsf = locked_write(&i, w, 1, 1, 1, 0, 2);
  auto rsf = locked_read(&r, &i, 2, 1, 1, 1, 0);
  ASSERT_EQ(std::future_status::ready, wsf.wait_until(start + base_duration(3)));
  ASSERT_EQ(std::future_status::ready, rsf.wait_until(start + base_duration(3)));
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_GT(end - start, base_duration(1));
  auto wsr = wsf.get();
  auto rsr = rsf.get();
  EXPECT_EQ(0, wsr.first);
  EXPECT_EQ(0, wsr.second);
  EXPECT_EQ(0, rsr.first);
  EXPECT_EQ(0, rsr.second);
  EXPECT_EQ(w, i);
  EXPECT_EQ(i, r);
}

TEST(LockingTest, MultiRead) {
  int i = 7;
  int r1 = 0;
  int r2 = 0;
  int r3 = 0;
  int r4 = 0;
  auto start = std::chrono::high_resolution_clock::now();
  auto rs1f = locked_read(&r1, &i, 1, 1, 1,  0, 1);
  auto rs2f = locked_read(&r2, &i, 1, 2, 1,  0, 1);
  auto rs3f = locked_read(&r3, &i, 1, 3, 1,  0, 1);
  auto rs4f = locked_read(&r4, &i, 1, 4, 1,  0, 1);
  ASSERT_EQ(std::future_status::ready, rs1f.wait_until(start + base_duration(5)));
  ASSERT_EQ(std::future_status::ready, rs2f.wait_until(start + base_duration(5)));
  ASSERT_EQ(std::future_status::ready, rs3f.wait_until(start + base_duration(5)));
  ASSERT_EQ(std::future_status::ready, rs4f.wait_until(start + base_duration(5)));
  auto end = std::chrono::high_resolution_clock::now();
  EXPECT_GT(end - start, base_duration(3));
  auto rs1r = rs1f.get();
  auto rs2r = rs2f.get();
  auto rs3r = rs3f.get();
  auto rs4r = rs4f.get();
  EXPECT_EQ(0, rs1r.first);
  EXPECT_EQ(0, rs1r.second);
  EXPECT_EQ(0, rs2r.first);
  EXPECT_EQ(0, rs2r.second);
  EXPECT_EQ(0, rs3r.first);
  EXPECT_EQ(0, rs3r.second);
  EXPECT_EQ(0, rs4r.first);
  EXPECT_EQ(0, rs4r.second);
  EXPECT_EQ(i, r1);
  EXPECT_EQ(i, r2);
  EXPECT_EQ(i, r3);
  EXPECT_EQ(i, r4);
}

TEST(LockingTest, HasMetadataTest) {
  int i = 7;
  int r = 0;
  auto rsf = locked_read(&r, &i, 1, 1, 1, 0, 2);
  std::this_thread::sleep_for(base_duration(1));
  EXPECT_EQ(1, has_metadata_lock(1, 1, 1));
  EXPECT_EQ(0, has_metadata_lock(2, 2, 1));
  ASSERT_EQ(std::future_status::ready, rsf.wait_for(base_duration(2)));
  EXPECT_EQ(0, has_metadata_lock(1, 1, 1));
  auto rsr = rsf.get();
  EXPECT_EQ(0, rsr.first);
  EXPECT_EQ(0, rsr.second);
  EXPECT_EQ(i, r);
}
