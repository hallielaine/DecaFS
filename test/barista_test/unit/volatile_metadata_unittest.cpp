#include "../../../src/app/barista_core/volatile_metadata.h"
#include "../../../src/lib/ip_address/ip_address.h"

#include "gtest/gtest.h"

#define VALID_CHUNK_SIZE 2048
#define ALT_VALID_CHUNK_SIZE 4096
#define INVALID_CHUNK_SIZE 64 

#define VALID_STRIPE_SIZE 2048
#define ALT_VALID_STRIPE_SIZE 4096
#define INVALID_STRIPE_SIZE 64

#define ESPRESSO_1_NODE_NUM 1
#define ESPRESSO_2_NODE_NUM 2
#define INVALID_NODE_NUM 3

#define CURSOR_VAL 32

struct ip_address ip;
struct client client(ip, 1, NULL);
struct client bad_client(ip, 2, NULL);
struct file_instance file_inst = {client, 1, 0};
struct file_instance bad_inst = {client, 2, 0};

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

TEST (Volatile_Metadata, SetNodeUp) {
  Volatile_Metadata v_meta;
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up(ESPRESSO_1_NODE_NUM));
  EXPECT_EQ (NODE_ALREADY_UP, v_meta.set_node_up(ESPRESSO_1_NODE_NUM));
}

TEST (Volatile_Metadata, GetActiveNodeCount) {
  Volatile_Metadata v_meta;

  EXPECT_EQ (0, v_meta.get_active_node_count());
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_EQ (1, v_meta.get_active_node_count());
}

TEST (Volatile_Metadata, GetActiveNodes) {
  Volatile_Metadata v_meta;
  
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_2_NODE_NUM));

  struct active_nodes nodes = v_meta.get_active_nodes();

  EXPECT_EQ (2, nodes.active_node_count);
  EXPECT_EQ (ESPRESSO_1_NODE_NUM, nodes.node_numbers[0]);
  EXPECT_EQ (ESPRESSO_2_NODE_NUM, nodes.node_numbers[1]);
  EXPECT_EQ (0, nodes.node_numbers[2]);
}

TEST (Volatile_Metadata, NodeDown) {
  Volatile_Metadata v_meta;
  
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_2_NODE_NUM));

  EXPECT_EQ (2, v_meta.get_active_nodes());
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_down (ESPRESSO_1_NODE_NUM));

  EXPECT_EQ (1, v_meta.get_active_nodes());
  EXPECT_EQ (ESPRESSO_2_NODE_NUM, nodes.node_numbers[0]);
  
}

TEST (Volatile_Metadata, NodeDownDoesNotExist) {
  Volatile_Metadata v_meta;

  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_2_NODE_NUM));
  
  EXPECT_EQ (2, v_meta.get_active_nodes());
  EXPECT_EQ (NODE_NUMBER_NOT_FOUND, v_meta.set_node_down (INVALID_NODE_NUM));
  EXPECT_EQ (2, v_meta.get_active_nodes());
}

TEST (Volatile_Metadata, IsNodeUp) {
  Volatile_Metadata v_meta;
  
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_up (ESPRESSO_2_NODE_NUM));
  
  EXPECT_TRUE (v_meta.is_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_TRUE (v_meta.is_node_up (ESPRESSO_2_NODE_NUM));

  EXPECT_EQ (V_META_SUCCESS, v_meta.set_node_down (ESPRESSO_1_NODE_NUM));
  
  EXPECT_FALSE (v_meta.is_node_up (ESPRESSO_1_NODE_NUM));
  EXPECT_TRUE (v_meta.is_node_up (ESPRESSO_2_NODE_NUM));
}

TEST (Volatile_Metadata, FileCursorCreation) {
  Volatile_Metadata v_meta;

  EXPECT_EQ (1, v_meta.new_file_cursor (1, client));
  EXPECT_EQ (0, v_meta.get_file_cursor (1));
}

TEST (Volatile_Metadata, FileCursorBadCursor) {
  Volatile_Metadata v_meta;

  EXPECT_EQ (INSTANCE_NOT_FOUND, v_meta.get_file_cursor (1));
  EXPECT_EQ (1, v_meta.new_file_cursor (1, client));
  EXPECT_EQ (0, v_meta.get_file_cursor (1));
  EXPECT_EQ (INSTANCE_NOT_FOUND, v_meta.get_file_cursor (2));
}

TEST (Volatile_Metadata, FileCursorSet) {
  Volatile_Metadata v_meta;
  
  EXPECT_EQ (1, v_meta.new_file_cursor (1, client));
  EXPECT_EQ (CURSOR_VAL, v_meta.set_file_cursor (1, CURSOR_VAL, client));
  EXPECT_EQ (CURSOR_VAL, v_meta.get_file_cursor (1));
  EXPECT_EQ (WRONG_CLIENT, v_meta.set_file_cursor (1, 0, bad_client));
}

TEST (Volatile_Metadata, FileCursorDelete) {
  Volatile_Metadata v_meta;
  
  EXPECT_EQ (1, v_meta.new_file_cursor (1, client));
  EXPECT_EQ (0, v_meta.get_file_cursor (1));
  EXPECT_EQ (WRONG_CLIENT, v_meta.close_file_cursor (1, bad_client));
  EXPECT_EQ (INSTANCE_NOT_FOUND, v_meta.close_file_cursor (2, client));
  EXPECT_EQ (0, v_meta.get_file_cursor (1));
  EXPECT_EQ (1, v_meta.close_file_cursor (1, client));
  EXPECT_EQ (INSTANCE_NOT_FOUND, v_meta.get_file_cursor (1));
}

TEST (Volatile_Metadata, GetFileInfo) {
  Volatile_Metadata v_meta;
  struct file_instance inst;
  
  EXPECT_EQ (1, v_meta.new_file_cursor (1, client));
  EXPECT_EQ (file_inst, v_meta.get_file_info (1));
  EXPECT_EQ (inst, v_meta.get_file_info (2));
}
