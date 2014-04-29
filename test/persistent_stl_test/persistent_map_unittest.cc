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
  EXPECT_EQ(0, pm.size());
  pm.close();
}

TEST_F(MapTest, ReopenInsertFindErase) {
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
  EXPECT_EQ(0, pm.size());
  pm.close();
}

TEST_F(MapTest, SimpleArrayAccess) {
  pm.open(testfile);
  pm[1] = 2;
  EXPECT_EQ(2, pm[1]);
  pm[1] = 3;
  EXPECT_EQ(3, pm[1]);
  pm[7] = 9;
  EXPECT_EQ(9, pm[7]);
  pm.clear();
  EXPECT_EQ(0, pm.size());
  pm.close();
}

TEST_F(MapTest, ReopenArrayAccess) {
  pm.open(testfile);
  pm[1] = 2;
  EXPECT_EQ(2, pm[1]);
  pm[1] = 3;
  EXPECT_EQ(3, pm[1]);
  pm[7] = 9;
  EXPECT_EQ(9, pm[7]);

  pm.close();
  pm.open(testfile);

  EXPECT_EQ(3, pm[1]);
  EXPECT_EQ(9, pm[7]);

  pm.clear();
  EXPECT_EQ(0, pm.size());
  pm.close();
}
