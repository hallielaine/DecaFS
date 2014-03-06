#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <gtest/gtest.h>

#include <espresso_storage.h>
#include <espresso_state.h>

// MUST be greater than 4096 for some tests.
const int TEST_FILE_SIZE = 50 * 1024;

class StorageTest : public ::testing::Test {
  private:
    char temp_name[32];
  protected:
    virtual void SetUp() {
      memcpy(temp_name, "espresso_storage_unittest.XXXXX", 32);
      espresso_global_data_init(mkstemp(temp_name), TEST_FILE_SIZE);
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

TEST_F (StorageTest, GrowChunkOnRewrite) {
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

TEST_F (StorageTest, GrowChunkOnAppend) {
  char buf[] = "a long test chunk that can be progressively grown";

  EXPECT_EQ(10, write_chunk(0, 1, 2, 3, 0, buf, 10));
  EXPECT_EQ(10, write_chunk(0, 1, 2, 3, 10, buf+10, 10));
  EXPECT_EQ(10, write_chunk(0, 1, 2, 3, 20, buf+20, 10));
  EXPECT_EQ(10, write_chunk(0, 1, 2, 3, 30, buf+30, 10));
  EXPECT_EQ(10, write_chunk(0, 1, 2, 3, 40, buf+40, 10));

  memset(buf, 0, 50);

  EXPECT_EQ(50, read_chunk(0, 1, 2, 3, 0, buf, 50));
  EXPECT_STREQ("a long test chunk that can be progressively grown", buf);

  EXPECT_EQ(1, espresso_global_data.metadata.size());
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
}

TEST_F (StorageTest, RewriteMiddle) {
  char buf[] = "test buffer";
  EXPECT_EQ(12, write_chunk(0, 21, 7, 1, 0, buf, 12));
  EXPECT_EQ(4, write_chunk(0, 21, 7, 1, 5, (void*)"hard", 4));
  EXPECT_EQ(12, read_chunk(0, 21, 7, 1, 0, buf, 12));
  EXPECT_STREQ("test harder", buf);
}

TEST_F (StorageTest, RewriteMiddleWithExtra) {
  char buf[38] = "test buffer";
  EXPECT_EQ(12, write_chunk(0, 21, 7, 1, 0, buf, 12));
  EXPECT_EQ(33, write_chunk(0, 21, 7, 1, 5,
        (void*)"harder, better, faster, stronger", 33));
  EXPECT_EQ(38, read_chunk(0, 21, 7, 1, 0, buf, 38));
  EXPECT_STREQ("test harder, better, faster, stronger", buf);
}

TEST_F (StorageTest, WriteUnallocatedOffset) {
  char buf[] = "test harder, better, faster, stronger";
  EXPECT_EQ(9, write_chunk(0, 5, 4, 3, 29, buf+29, 9));
  EXPECT_EQ(9, read_chunk(0, 5, 4, 3, 29, buf+29, 9));
  EXPECT_STREQ("test harder, better, faster, stronger", buf);
  EXPECT_EQ(29, write_chunk(0, 5, 4, 3, 0, buf, 29));
  EXPECT_EQ(38, read_chunk(0, 5, 4, 3, 0, buf, 38));
  EXPECT_STREQ("test harder, better, faster, stronger", buf);
}

TEST_F (StorageTest, ReadNonexistentChunk) {
  char buf[50];
  EXPECT_EQ(-1, read_chunk(0, 1, 2, 3, 0, buf, 50));
}

TEST_F (StorageTest, DeleteNonexistentChunk) {
  EXPECT_EQ(-1, delete_chunk(0, 1, 2, 3));
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

TEST_F (StorageTest, ReadTooMuch) {
  char buf[9] = "test str";
  EXPECT_EQ(9, write_chunk(0, 0, 0, 0, 0, buf, 9));
  EXPECT_EQ(-1, read_chunk(0, 0, 0, 0, 0, buf, 10));
}

TEST_F (StorageTest, ReadTooFar) {
  char buf[9] = "test str";
  EXPECT_EQ(9, write_chunk(0, 0, 0, 0, 0, buf, 9));
  EXPECT_EQ(-1, read_chunk(0, 0, 0, 0, 1, buf, 9));
}

TEST_F (StorageTest, DeleteChunk) {
  char buf[] = "test buffer";
  EXPECT_EQ(12, write_chunk(0, 2, 4, 6, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.metadata.size());
  EXPECT_EQ(12, read_chunk(0, 2, 4, 6, 0, buf, 12));

  EXPECT_EQ(0, delete_chunk(0, 2, 4, 6));
  EXPECT_EQ(0, espresso_global_data.metadata.size());
  EXPECT_EQ(-1, read_chunk(0, 2, 4, 6, 0, buf, 12));
}

TEST_F (StorageTest, MixedWritesDeletes) {
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

  EXPECT_EQ(0, delete_chunk(0, 0, 2, 0));
  EXPECT_EQ(1, espresso_global_data.metadata.size());

  EXPECT_EQ(strlen(buf3)+1, write_chunk(0, 1, 1, 0, 0, buf3, strlen(buf3)+1));
  EXPECT_EQ(2, espresso_global_data.metadata.size());
  EXPECT_STREQ("very gtest", buf3);

  EXPECT_EQ(strlen(buf4)+1, write_chunk(0, 1, 2, 0, 0, buf4, strlen(buf4)+1));
  EXPECT_EQ(3, espresso_global_data.metadata.size());
  EXPECT_STREQ("so buffer", buf4);

  EXPECT_EQ(strlen(buf5)+1, write_chunk(0, 2, 1, 0, 0, buf5, strlen(buf5)+1));
  EXPECT_EQ(4, espresso_global_data.metadata.size());
  EXPECT_STREQ("much wow", buf5);

  EXPECT_EQ(0, delete_chunk(0, 2, 1, 0));
  EXPECT_EQ(3, espresso_global_data.metadata.size());

  EXPECT_EQ(strlen(buf6)+1, write_chunk(0, 2, 2, 0, 0, buf6, strlen(buf6)+1));
  EXPECT_EQ(4, espresso_global_data.metadata.size());
  EXPECT_STREQ("lorem ipsum", buf6);

  EXPECT_EQ(strlen(buf1)+1, read_chunk(0, 0, 1, 0, 0, buf1, strlen(buf1)+1));
  EXPECT_EQ(-1, read_chunk(0, 0, 2, 0, 0, buf2, strlen(buf2)+1));
  EXPECT_EQ(strlen(buf3)+1, read_chunk(0, 1, 1, 0, 0, buf3, strlen(buf3)+1));
  EXPECT_EQ(strlen(buf4)+1, read_chunk(0, 1, 2, 0, 0, buf4, strlen(buf4)+1));
  EXPECT_EQ(-1, read_chunk(0, 2, 1, 0, 0, buf5, strlen(buf5)+1));
  EXPECT_EQ(strlen(buf6)+1, read_chunk(0, 2, 2, 0, 0, buf6, strlen(buf6)+1));

  EXPECT_EQ(-1, delete_chunk(0, 2, 1, 0));
  EXPECT_EQ(-1, delete_chunk(0, 3, 1, 0));

  EXPECT_STREQ("first test str", buf1);
  EXPECT_STREQ("second test str", buf2);
  EXPECT_STREQ("very gtest", buf3);
  EXPECT_STREQ("so buffer", buf4);
  EXPECT_STREQ("much wow", buf5);
  EXPECT_STREQ("lorem ipsum", buf6);
}

TEST_F (StorageTest, FillDataFile) {
  char *buf = new char[TEST_FILE_SIZE - 4096];

  EXPECT_EQ(1024, write_chunk(0, 1, 1, 0, 0, buf, 1024));
  EXPECT_EQ(1024, write_chunk(0, 1, 2, 0, 0, buf, 1024));
  EXPECT_EQ(TEST_FILE_SIZE - 4096,
      write_chunk(0, 1, 3, 0, 0, buf, TEST_FILE_SIZE - 4096));
  EXPECT_EQ(1024, write_chunk(0, 1, 4, 0, 0, buf, 1024));
  EXPECT_EQ(1024, write_chunk(0, 1, 5, 0, 0, buf, 1024));
  EXPECT_EQ(5, espresso_global_data.metadata.size());
  EXPECT_EQ(0, espresso_global_data.free_extents.size());

  EXPECT_EQ(-1, write_chunk(0, 1, 6, 0, 0, buf, 1));
  EXPECT_EQ(5, espresso_global_data.metadata.size());

  EXPECT_EQ(0, delete_chunk(0, 1, 2, 0));
  EXPECT_EQ(4, espresso_global_data.metadata.size());
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(512, write_chunk(0, 1, 7, 0, 0, buf, 512));
  EXPECT_EQ(512, write_chunk(0, 1, 8, 0, 0, buf, 512));
  EXPECT_EQ(6, espresso_global_data.metadata.size());
  EXPECT_EQ(0, espresso_global_data.free_extents.size());

  EXPECT_EQ(-1, write_chunk(0, 1, 6, 0, 0, buf, 1));
  EXPECT_EQ(6, espresso_global_data.metadata.size());
  EXPECT_EQ(0, espresso_global_data.free_extents.size());

  EXPECT_EQ(0, delete_chunk(0, 1, 3, 0));
  EXPECT_EQ(5, espresso_global_data.metadata.size());
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  delete[] buf;
}

TEST_F (StorageTest, DeleteMergesPriorExtent) {
  char buf[] = "test buffer";
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(12, write_chunk(0, 0, 0, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 1, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 2, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(0, delete_chunk(0, 0, 0, 0));
  EXPECT_EQ(2, espresso_global_data.free_extents.size());
  EXPECT_EQ(0, delete_chunk(0, 0, 1, 0));
  EXPECT_EQ(2, espresso_global_data.free_extents.size());
}

TEST_F (StorageTest, DeleteMergesLaterExtent) {
  char buf[] = "test buffer";
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(12, write_chunk(0, 0, 0, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 1, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 2, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(0, delete_chunk(0, 0, 1, 0));
  EXPECT_EQ(2, espresso_global_data.free_extents.size());
  EXPECT_EQ(0, delete_chunk(0, 0, 0, 0));
  EXPECT_EQ(2, espresso_global_data.free_extents.size());
}

TEST_F (StorageTest, DeleteMergesBothSides) {
  char buf[] = "test buffer";
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(12, write_chunk(0, 0, 0, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 1, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 2, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
  EXPECT_EQ(12, write_chunk(0, 0, 3, 0, 0, buf, 12));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());

  EXPECT_EQ(0, delete_chunk(0, 0, 0, 0));
  EXPECT_EQ(2, espresso_global_data.free_extents.size());
  EXPECT_EQ(0, delete_chunk(0, 0, 2, 0));
  EXPECT_EQ(3, espresso_global_data.free_extents.size());

  EXPECT_EQ(0, delete_chunk(0, 0, 1, 0));
  EXPECT_EQ(2, espresso_global_data.free_extents.size());
  EXPECT_EQ(0, delete_chunk(0, 0, 3, 0));
  EXPECT_EQ(1, espresso_global_data.free_extents.size());
}

// TODO explicitly test underlying _data functions for bounds checking
