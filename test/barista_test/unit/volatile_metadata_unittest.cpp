#include "../../../src/app/barista_core/barista_core.h"
#include "gtest/gtest.h"

#define VALID_CHUNK_SIZE 2048
#define ALT_VALID_CHUNK_SIZE 4096
#define INVALID_CHUNK_SIZE 64 

#define VALID_STRIPE_SIZE 2048
#define ALT_VALID_STRIPE_SIZE 4096
#define INVALID_STRIPE_SIZE 64

#define BARISTA "192.168.1.100"
#define ESPRESSO_1 "192.168.1.101"
#define ESPRESSO_2 "192.168.1.102"

#define BARISTA_NODE_NUM 1
#define ESPRESSO_1_NODE_NUM 2
#define ESPRESSO_2_NODE_NUM 3

TEST (Volatile_Metadata, DefaultConstructor) {
  Volatile_Metadata v_meta;

  EXPECT_EQ (0, v_meta.get_chunk_size());
  EXPECT_EQ (0, v_meta.get_stripe_size());
}

TEST (Volatile_Metadata, SetChunkSizeValid) {
  Volatile_Metadata v_meta;

  v_meta.set_chunk_size (VALID_CHUNK_SIZE);
  EXPECT_EQ (VALID_CHUNK_SIZE, v_meta.get_chunk_size());
}

TEST (Volatile_Metadata, SetChunkSizeInvalid) {
  Volatile_Metadata v_meta;

  v_meta.set_chunk_size (INVALID_CHUNK_SIZE);
  EXPECT_EQ (0, v_meta.get_chunk_size());
}

TEST (Volatile_Metadata, SetStripeSizeValid) {
  Volatile_Metadata v_meta;

  v_meta.set_stripe_size (VALID_STRIPE_SIZE);
  EXPECT_EQ (VALID_STRIPE_SIZE, v_meta.get_stripe_size());
}

TEST (Volatile_Metadata, SetStripeSizeInvalid) {
  Volatile_Metadata v_meta;

  v_meta.set_stripe_size (INVALID_STRIPE_SIZE);
  EXPECT_EQ (0, v_meta.get_stripe_size());
}

TEST (Volatile_Metadata, ResetChunkSize) {
  Volatile_Metadata v_meta;

  v_meta.set_chunk_size (VALID_CHUNK_SIZE);
  EXPECT_EQ (VALID_CHUNK_SIZE, v_meta.get_chunk_size());
  v_meta.set_chunk_size (ALT_VALID_CHUNK_SIZE);
  EXPECT_EQ (VALID_CHUNK_SIZE, v_meta.get_chunk_size());
}

TEST (Volatile_Metadata, ResetStripeSize) {
  Volatile_Metadata v_meta;

  v_meta.set_stripe_size (VALID_STRIPE_SIZE);
  EXPECT_EQ (VALID_STRIPE_SIZE, v_meta.get_stripe_size());
  v_meta.set_stripe_size (ALT_VALID_STRIPE_SIZE);
  EXPECT_EQ (VALID_STRIPE_SIZE, v_meta.get_stripe_size());
}

TEST (Volatile_Metadata, AddNodes) {
  Volatile_Metadata v_meta;
}
