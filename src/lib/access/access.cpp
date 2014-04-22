#include "access.h"

ssize_t process_read_chunk (int fd, int file_id, int node_id, 
     int stripe_id, int chunk_num, int offset, 
     void *buf, int count) {
  // No buffering
  return network_read_chunk(fd, file_id, node_id, stripe_id, chunk_num,
    offset, count);
}

ssize_t process_write_chunk (int fd, int file_id, int node_id, 
 int stripe_id, int chunk_num, int offset, 
 void *buf, int count) {
  // No buffering
  return network_write_chunk(fd, file_id, node_id, stripe_id, chunk_num, 
    offset, buf, count);
}


ssize_t process_delete_chunk (int file_id, int node_id, int stripe_id,
 int chunk_num) {

  return network_delete_chunk(file_id, node_id, stripe_id, chunk_num);
}
