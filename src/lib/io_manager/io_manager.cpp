#include "io_manager.h"

IO_Manager::IO_Manager() {

}

void IO_Manager::init(char *metadata_path) {
  std::string node_metadata_file = std::string(metadata_path) +
                                   std::string(node_metadata_filename);
  std::string replica_metadata_file = std::string(metadata_path) +
                                      std::string(replica_metadata_filename);
  chunk_to_node.open (node_metadata_file.c_str());
  chunk_to_replica_node.open (replica_metadata_file.c_str());
  
}

ssize_t IO_Manager::process_read_stripe (uint32_t request_id, uint32_t file_id,
                                         char *pathname, uint32_t stripe_id,
                                         uint32_t stripe_size, uint32_t chunk_size,
                                         const void *buf, int offset,
                                         size_t count) {
  uint32_t chunk_id, bytes_read = 0, read_size = 0;
  int chunk_offset, chunk_result, node_id;
  
  assert ((count - offset) <= stripe_size);
  
  printf ("\n(BARISTA) Process Read Stripe\n");

  get_first_chunk (&chunk_id, chunk_size, &chunk_offset, offset);
  
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

    // If the node isn't up, switch to the replica
    if (!is_node_up (node_id)) {
      assert (chunk_replica_exists (cur_chunk));
      node_id = chunk_to_replica_node[cur_chunk];
    }
   
    // Determine how much data to read from the current chunk
    if (count - bytes_read > chunk_size - chunk_offset) {
      read_size = chunk_size - chunk_offset;
    }
    else {
      read_size = count - bytes_read;
    }
    
    printf ("\tprocessing chunk %d (sending to node %d)\n", chunk_id, node_id);
    printf ("\t\toffset: %d, size: %d\n", chunk_offset, read_size);
    // Send the read to the node
                   // ADD FD HERE
    chunk_result = process_read_chunk (request_id, 0, file_id, node_id, stripe_id,
                                      chunk_id, chunk_offset, 
                                      (uint8_t *)buf + bytes_read,
                                      read_size);
    
    printf ("\t\treceived %d from network call.\n", chunk_result);
    // If the node cannot be read from
    if (chunk_result == NODE_FAILURE) {
      // Mark the node as "down"
      set_node_down (node_id);
    }
    // The read suceeded, so move on
    else {
      // update counters
      chunk_offset = 0;
      bytes_read += read_size;
      chunk_id++;
    }
  }
  return bytes_read;
}

ssize_t IO_Manager::process_write_stripe (uint32_t request_id, uint32_t file_id,
                                          char *pathname, uint32_t stripe_id,
                                          uint32_t stripe_size, uint32_t chunk_size,
                                          const void *buf, int offset,
                                          size_t count) {
  uint32_t chunk_id, bytes_written = 0, write_size = 0;
  int chunk_offset, node_id, replica_node_id, write_result;

  assert ((count - offset) <= stripe_size);
  printf ("\n(BARISTA) Process Write Stripe\n");
  
  get_first_chunk (&chunk_id, chunk_size, &chunk_offset, offset);

  while (bytes_written < count) {
    struct file_chunk cur_chunk = {file_id, stripe_id, chunk_id};
    
    // If the chunk does not exists, create it
    if (!chunk_exists (cur_chunk)) {
      node_id = put_chunk (file_id, pathname, stripe_id, chunk_id);
      printf ("\tchunk doesn't exist. Preparing to send chunk to node %d\n", node_id);
      chunk_to_node[cur_chunk] = node_id;
    }

    // If the replica does not exist, create it
    if (!chunk_replica_exists (cur_chunk)) {
      replica_node_id = put_replica (file_id, pathname, stripe_id,
                                     chunk_id);
      printf ("\tchunk replica doesn't exist. Preparing to send chunk replica to node %d\n", 
                 replica_node_id);
      chunk_to_replica_node[cur_chunk] = replica_node_id;
    }

    // Ensure that we have the proper node and replica id's to send data to
    node_id = chunk_to_node[cur_chunk];
    replica_node_id = chunk_to_replica_node[cur_chunk];

    // Determine the size of the write
    if (count - bytes_written > chunk_size - chunk_offset) {
      write_size = chunk_size - chunk_offset;
    }
    else {
      write_size = count - bytes_written;
    }

    // Send the write to the node
                        // ADD FD HERE
    printf ("\tprocessing chunk %d (sending to node %d)\n", chunk_id, node_id);
    write_result = process_write_chunk (request_id, 0, file_id, node_id, stripe_id,
                                        chunk_id, chunk_offset, (uint8_t *)buf
                                        + bytes_written, write_size);
    printf ("\t\treceived %d from network call.\n", write_result);
    // If the write failed
    if (write_result == NODE_FAILURE) {
      // Set the node to "down" and try again
      set_node_down (node_id);
    }
    else {
      // Send the write to the replica node
                          // ADD FD HERE
      printf ("\tprocessing chunk replica %d (sending to node %d)\n", chunk_id, 
                 replica_node_id);
      write_result = process_write_chunk (request_id, 0, file_id, replica_node_id, stripe_id,
                                          chunk_id, chunk_offset, (uint8_t *)buf
                                          + bytes_written, write_size);
      // if the replica write failed
      if (write_result == NODE_FAILURE) {
        // Set the node to "down"
        set_node_down (replica_node_id);
        // Choose a different replica
        replica_node_id = put_replica (file_id, pathname, stripe_id,
                                       chunk_id);
        // Re-write the data
        process_write_chunk (request_id, 0, file_id, replica_node_id, stripe_id,
                             chunk_id, chunk_offset, (uint8_t *)buf
                             + bytes_written, write_size);
      }
      // update counters
      chunk_offset = 0;
      bytes_written += write_size;
      chunk_id++;
    }
  }

  return bytes_written;
}

void IO_Manager::process_delete_file (uint32_t request_id, uint32_t file_id) {
  std::vector<struct file_chunk> chunks = get_all_chunks (file_id); 

  for (std::vector<struct file_chunk>::iterator it = chunks.begin();
       it != chunks.end(); it++) {
    if (chunk_exists (*it)) {
      int chunk_node = get_node_id (file_id, (*it).stripe_id, (*it).chunk_num);
      process_delete_chunk (request_id, file_id, chunk_node, (*it).stripe_id, (*it).chunk_num);
      chunk_to_node.erase (*it);
    }
    if (chunk_replica_exists (*it)) {
      int chunk_node = get_replica_node_id (file_id, (*it).stripe_id,
                                            (*it).chunk_num);
      process_delete_chunk (request_id, file_id, chunk_node, (*it).stripe_id, (*it).chunk_num);
      chunk_to_replica_node.erase (*it);
    }
  }
}

int IO_Manager::set_node_id (uint32_t file_id, uint32_t stripe_id,
                             uint32_t chunk_num, uint32_t node_id) {
  struct file_chunk chunk = {file_id, stripe_id, chunk_num};
  chunk_to_node[chunk] = node_id;
  return node_id;
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
  struct file_chunk chunk = {file_id, stripe_id, chunk_num};
  chunk_to_replica_node[chunk] = node_id;
  return node_id;
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

void IO_Manager::sync () {
  // Intentionally left blank, in this implementation, all file data is written
  // to disk when "write" completes.
}

bool IO_Manager::chunk_exists (struct file_chunk chunk) {
  return (chunk_to_node.find (chunk) != chunk_to_node.end());
}

bool IO_Manager::chunk_replica_exists (struct file_chunk chunk) {
  return (chunk_to_replica_node.find (chunk) != chunk_to_replica_node.end());
}

std::vector<struct file_chunk> IO_Manager::get_all_chunks (uint32_t file_id) {
  std::vector <struct file_chunk> chunks;
  
  for (PersistentMap<struct file_chunk, int>::iterator it = chunk_to_node.begin();
         it != chunk_to_node.end(); it++) {
    struct file_chunk cur = it->first;
    if (cur.file_id == file_id) {
      chunks.push_back (cur);
    }
  }
  return chunks;
}

void IO_Manager::get_first_chunk (uint32_t *id, uint32_t chunk_size, int *chunk_offset, int offset) {
  *id = CHUNK_ID_INIT;
  while (offset > (int)chunk_size) {
    (*id)++;
    offset -= chunk_size;
  }
  *chunk_offset = offset;
}
