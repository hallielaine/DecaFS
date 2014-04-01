#include "io_manager.h"

IO_Manager::IO_Manager() {

}

ssize_t IO_Manager::process_read_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, void *buf,
                                         size_t count) {
  uint32_t chunk_id = CHUNK_ID_INIT, bytes_read = 0, read_size = 0;
  
  assert (count <= get_stripe_size());
  while (bytes_read < count) {
    if (count - bytes_read > get_chunk_size()) {
      read_size = get_chunk_size();
    }
    else {
      read_size = count - bytes_read;
    }

    // use chunk_id
    // CALL READ IN ACCESS LAYER
    // read (buf + bytes_read, read_size)

    bytes_read += read_size;
    chunk_id++;
  }

  return bytes_read;
}

ssize_t IO_Manager::process_write_stripe (uint32_t file_id, char *pathname,
                                          uint32_t stripe_id, void *buf,
                                          size_t count) {
  uint32_t chunk_id = CHUNK_ID_INIT, bytes_written = 0, write_size = 0;

  assert (count <= get_stripe_size());
  while (bytes_written < count) {
    if (count - bytes_written > get_chunk_size()) {
      write_size = get_chunk_size();
    }
    else {
      write_size = count - bytes_written;
    }

    // use chunk_id
    // CALL WRITE IN ACCESS LAYER
    // write (buf + bytes_written, write_size)

    bytes_written += write_size;
    chunk_id++;
  }

  return bytes_written;
}

int IO_Manager::set_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num, uint32_t node_id) {
  if (node_exists (node_id)) {
    struct file_chunk chunk = {file_id, stripe_id, chunk_num};
    chunk_to_node[chunk] = node_id;
    return node_id;
  }
  return NODE_NOT_FOUND;
}

int IO_Manager::get_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num) {
  struct file_chunk chunk = {file_id, stripe_id, chunk_num};
  if (chunk_exists (chunk)) {
    return chunk_to_node[chunk];
  }
  return CHUNK_NOT_FOUND;
}

int IO_Manager::set_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                     uint32_t chunk_num, uint32_t node_id) {
  if (node_exists (node_id)) {
    struct file_chunk chunk = {file_id, stripe_id, chunk_num};
    chunk_to_replica_node[chunk] = node_id;
    return node_id;
  }
  return REPLICA_NODE_NOT_FOUND;
}

int IO_Manager::get_replica_node_id (uint32_t file_id, uint32_t stripe_id,
                                     uint32_t chunk_num) {
  struct file_chunk chunk = {file_id, stripe_id, chunk_num};
  if (chunk_replica_exists (chunk)) {
    return chunk_to_replica_node[chunk];
  }
  return REPLICA_CHUNK_NOT_FOUND;
}

int IO_Manager::stat_file_name (char *pathname, struct decafs_file_stat *buf) {
  return 0;
}

int IO_Manager::stat_file_id (uint32_t file_id, struct decafs_file_stat *buf) {
  return 0;
}

int IO_Manager::stat_replica_name (char *pathname, struct decafs_file_stat *buf) {
  return 0;
}

int IO_Manager::stat_replica_id (uint32_t file_id, struct decafs_file_stat *buf) {
  return 0;
}

bool IO_Manager::chunk_exists (struct file_chunk chunk) {
  return (chunk_to_node.find (chunk) != chunk_to_node.end());
}

bool IO_Manager::chunk_replica_exists (struct file_chunk chunk) {
  return (chunk_to_replica_node.find (chunk) != chunk_to_replica_node.end());
}
