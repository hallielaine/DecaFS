#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <gtest/gtest.h>

#include <espresso_storage.h>
#include <espresso_state.h>

class StorageTest : public ::testing::Test {
  private:
    char temp_name[32];
  protected:
    virtual void SetUp() {
      memcpy(temp_name, "espresso_storage_unittest.XXXXX", 32);
      espresso_global_data_init(mkstemp(temp_name), 50 * 1024);
    }

    virtual void TearDown() {
      close(espresso_global_data.fd);
      unlink(temp_name);
      espresso_global_data.metadata.clear();
      espresso_global_data.free_extents.clear();
    }
};

TEST_F (StorageTest, SimpleWrite) {
  char buf[9] = "test str";

  EXPECT_EQ(9, write_chunk(0, 0, 0, 0, 0, buf, 9));
  memset(buf, 0, 9);

  EXPECT_EQ(9, read_chunk(0, 0, 0, 0, 0, buf, 9));
  EXPECT_STREQ("test str", buf);
}

TEST_F (StorageTest, MultipleWrites) {
  char buf1[] = "first test str";
  char buf2[] = "second test str";
  char buf3[] = "very gtest";
  char buf4[] = "so buffer";
  char buf5[] = "much wow";
  char buf6[] = "lorem ipsum";

  EXPECT_EQ(strlen(buf1)+1, write_chunk(0, 0, 1, 0, 0, buf1, strlen(buf1)+1));
  EXPECT_EQ(1, espresso_global_data.metadata.size());
  EXPECT_STREQ("first test str", buf1);

  EXPECT_EQ(strlen(buf2)+1, write_chunk(0, 0, 2, 0, 0, buf2, strlen(buf2)+1));
  EXPECT_EQ(2, espresso_global_data.metadata.size());
  EXPECT_STREQ("second test str", buf2);

  EXPECT_EQ(strlen(buf3)+1, write_chunk(0, 1, 1, 0, 0, buf3, strlen(buf3)+1));
  EXPECT_EQ(3, espresso_global_data.metadata.size());
  EXPECT_STREQ("very gtest", buf3);

  EXPECT_EQ(strlen(buf4)+1, write_chunk(0, 1, 2, 0, 0, buf4, strlen(buf4)+1));
  EXPECT_EQ(4, espresso_global_data.metadata.size());
  EXPECT_STREQ("so buffer", buf4);

  EXPECT_EQ(strlen(buf5)+1, write_chunk(0, 2, 1, 0, 0, buf5, strlen(buf5)+1));
  EXPECT_EQ(5, espresso_global_data.metadata.size());
  EXPECT_STREQ("much wow", buf5);

  EXPECT_EQ(strlen(buf6)+1, write_chunk(0, 2, 2, 0, 0, buf6, strlen(buf6)+1));
  EXPECT_EQ(6, espresso_global_data.metadata.size());
  EXPECT_STREQ("lorem ipsum", buf6);

  EXPECT_EQ(strlen(buf1)+1, read_chunk(0, 0, 1, 0, 0, buf1, strlen(buf1)+1));
  EXPECT_EQ(strlen(buf2)+1, read_chunk(0, 0, 2, 0, 0, buf2, strlen(buf2)+1));
  EXPECT_EQ(strlen(buf3)+1, read_chunk(0, 1, 1, 0, 0, buf3, strlen(buf3)+1));
  EXPECT_EQ(strlen(buf4)+1, read_chunk(0, 1, 2, 0, 0, buf4, strlen(buf4)+1));
  EXPECT_EQ(strlen(buf5)+1, read_chunk(0, 2, 1, 0, 0, buf5, strlen(buf5)+1));
  EXPECT_EQ(strlen(buf6)+1, read_chunk(0, 2, 2, 0, 0, buf6, strlen(buf6)+1));

  EXPECT_STREQ("first test str", buf1);
  EXPECT_STREQ("second test str", buf2);
  EXPECT_STREQ("very gtest", buf3);
  EXPECT_STREQ("so buffer", buf4);
  EXPECT_STREQ("much wow", buf5);
  EXPECT_STREQ("lorem ipsum", buf6);

}

TEST_F (StorageTest, GrowChunkOnWrite) {
  char buf[] = "a long test chunk that can be progressively grown";

  EXPECT_EQ(10, write_chunk(0, 1, 2, 3, 0, buf, 10));
  EXPECT_EQ(20, write_chunk(0, 1, 2, 3, 0, buf, 20));
  EXPECT_EQ(30, write_chunk(0, 1, 2, 3, 0, buf, 30));
  EXPECT_EQ(40, write_chunk(0, 1, 2, 3, 0, buf, 40));
  EXPECT_EQ(50, write_chunk(0, 1, 2, 3, 0, buf, 50));

  memset(buf, 0, 50);

  EXPECT_EQ(50, read_chunk(0, 1, 2, 3, 0, buf, 50));
  EXPECT_STREQ("a long test chunk that can be progressively grown", buf);

  EXPECT_EQ(1, espresso_global_data.metadata.size());
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
}

TEST_F (StorageTest, RewriteMiddle) {
}

TEST_F (StorageTest, RewriteWithExtra) {
}

TEST_F (StorageTest, ReadNonexistentChunk) {
  char buf[50];
  EXPECT_EQ(-1, read_chunk(0, 1, 2, 3, 0, buf, 50));
}

TEST_F (StorageTest, WriteNegativeSize) {
  char buf[] = "test buffer";
  EXPECT_EQ(-1, write_chunk(0, 1, 2, 3, 0, buf, -5));
}

TEST_F (StorageTest, ReadNegativeSize) {
  char buf[] = "test buffer";
  EXPECT_EQ(12, write_chunk(0, 1, 2, 3, 0, buf, 12));
  EXPECT_EQ(-1, write_chunk(0, 1, 2, 3, 0, buf, -8));
}
