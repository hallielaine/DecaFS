#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <utility>

#include "gtest/gtest.h"

#include "persistent_map.h"

using ::std::make_pair;

const char *testfile = "test.map";

class MapTest : public ::testing::Test {
  protected:
    PersistentMap<int, int> pm;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(MapTest, SimpleInsertFindErase) {
  pm.open(testfile);
  auto insert = pm.insert(make_pair(1, 2));
  EXPECT_TRUE(insert.second);
  EXPECT_EQ(1, insert.first->first);
  EXPECT_EQ(2, insert.first->second);
  auto find = pm.find(1);
  EXPECT_EQ(1, find->first);
  EXPECT_EQ(2, find->second);
  auto erase = pm.erase(find);
  EXPECT_EQ(pm.end(), erase);
  pm.clear();
  pm.close();
}

TEST_F(MapTest, ReopenInsert) {
  pm.open(testfile);
  auto insert = pm.insert(make_pair(1, 2));
  EXPECT_TRUE(insert.second);
  EXPECT_EQ(1, insert.first->first);
  EXPECT_EQ(2, insert.first->second);
  pm.close();
  pm.open(testfile);
  auto find = pm.find(1);
  EXPECT_EQ(1, find->first);
  EXPECT_EQ(2, find->second);
  auto erase = pm.erase(find);
  EXPECT_EQ(pm.end(), erase);
  pm.clear();
  pm.close();
}
