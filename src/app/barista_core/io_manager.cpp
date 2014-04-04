#include "io_manager.h"

IO_Manager::IO_Manager() {

}

ssize_t IO_Manager::process_read_stripe (uint32_t file_id, char *pathname,
                                         uint32_t stripe_id, void *buf,
                                         int offset, size_t count) {
  uint32_t chunk_id, bytes_read = 0, read_size = 0;
  int chunk_offset, node_id;
  
  assert ((count - offset) <= get_stripe_size());

  get_first_chunk (&chunk_id, &chunk_offset, offset);
  
  while (bytes_read < count) {
    struct file_chunk cur_chunk = {file_id, stripe_id, chunk_id};
    if (!chunk_exists (cur_chunk)) {
      // Current chunk does not exist. Report and error and stop the read.
      fprintf (stderr, "Could only read %d bytes (out of %d requested.\n",
                  (int)bytes_read, (int)count);
      break;
    }

    // The chunk exists, so set the node_id
    node_id = chunk_to_node[cur_chunk];
   
    // Determine how much data to read from the current chunk
    if (count - bytes_read > get_chunk_size() - chunk_offset) {
      read_size = get_chunk_size() - chunk_offset;
    }
    else {
      read_size = count - bytes_read;
    }

    // Send the read to the node
                   // ADD FD HERE
    process_read_chunk (0, file_id, node_id, stripe_id, chunk_id,
                        chunk_offset, (uint8_t *)buf + bytes_read, read_size);

    // update counters
    chunk_offset = 0;
    bytes_read += read_size;
    chunk_id++;
  }

  return bytes_read;
}

ssize_t IO_Manager::process_write_stripe (uint32_t file_id, char *pathname,
                                          uint32_t stripe_id, void *buf,
                                          int offset, size_t count) {
  uint32_t chunk_id, bytes_written = 0, write_size = 0;
  int chunk_offset, node_id, replica_node_id;

  assert ((count - offset) <= get_stripe_size());
  
  get_first_chunk (&chunk_id, &chunk_offset, offset);

  while (bytes_written < count) {
    struct file_chunk cur_chunk = {file_id, stripe_id, chunk_id};
    // If the chunk does not exists, create it
    if (!chunk_exists (cur_chunk)) {
      node_id = put_chunk (file_id, pathname, stripe_id, chunk_id);
      chunk_to_node[cur_chunk] = node_id;
    }

    // If the replica does not exist, create it
    if (!chunk_replica_exists (cur_chunk)) {
      replica_node_id = put_replica (file_id, pathname, stripe_id,
                                     chunk_id);
      chunk_to_replica_node[cur_chunk] = replica_node_id;
    }

    // Ensure that we have the proper node and replica id's to send data to
    node_id = chunk_to_node[cur_chunk];
    replica_node_id = chunk_to_replica_node[cur_chunk];

    // Determine the size of the write
    if (count - bytes_written > get_chunk_size() - chunk_offset) {
      write_size = get_chunk_size() - chunk_offset;
    }
    else {
      write_size = count - bytes_written;
    }

    // Send the write to the node
                        // ADD FD HERE
    process_write_chunk (0, file_id, node_id, stripe_id, chunk_id,
                         chunk_offset, (uint8_t *)buf + bytes_written, write_size);
    // Send the write to the replica node
                        // ADD FD HERE
    process_write_chunk (0, file_id, replica_node_id, stripe_id, chunk_id,
                         chunk_offset, (uint8_t *)buf + bytes_written, write_size);
    // update counters
    chunk_offset = 0;
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

// These 4 functions should be moved up to Barista Core
// this layer is stripe level not file level
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
    
void IO_Manager::get_first_chunk (uint32_t *id, int *chunk_offset, int offset) {
  *id = CHUNK_ID_INIT;
  while (offset > (int)get_chunk_size()) {
    (*id)++;
    offset -= get_chunk_size();
  }
  *chunk_offset = offset;
}
