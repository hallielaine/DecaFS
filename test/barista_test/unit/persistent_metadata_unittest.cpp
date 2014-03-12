#include "../../../src/app/barista_core/persistent_metadata.h"
#include "gtest/gtest.h"

#define STRIPE_SIZE 1024
#define CHUNK_SIZE 512
#define REPLICA_SIZE 512

const char *file_1 = "file 1";
const char *file_2 = "file 2";

const int file_1_id = 1;
const int file_2_id = 2;

TEST (Persistent_Metadata, AddFile) {
  Persistent_Metadata p_meta;
  struct timeval time;
  char *bad_file = (char *)malloc (MAX_FILENAME_LENGTH + 2);
  memset (bad_file, 'a', MAX_FILENAME_LENGTH + 1);
  bad_file[MAX_FILENAME_LENGTH + 1] = '\0';

  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (FILE_EXISTS, p_meta.add_file ((char *)file_1, file_1_id,
                                               STRIPE_SIZE, CHUNK_SIZE,
                                               REPLICA_SIZE, time));
  EXPECT_EQ (FILENAME_INVALID, p_meta.add_file ((char *)bad_file, file_1_id,
                                                    STRIPE_SIZE, CHUNK_SIZE,
                                                    REPLICA_SIZE, time));
}

TEST (Persistent_Metadata, GetNumFiles) {
  Persistent_Metadata p_meta;
  struct timeval time;
  
  EXPECT_EQ (0, p_meta.add_file ((char *)file_1, file_1_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (1, p_meta.get_num_files());
  EXPECT_EQ (0, p_meta.add_file ((char *)file_2, file_2_id, STRIPE_SIZE,
                                    CHUNK_SIZE, REPLICA_SIZE, time));
  EXPECT_EQ (2, p_meta.get_num_files());
}
