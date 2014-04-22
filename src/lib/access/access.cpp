#include "access.h"

ssize_t process_read_chunk (int fd, int file_id, int node_id, 
     int stripe_id, int chunk_num, int offset, 
     void *buf, int count) {
  // No buffering
  return DATA_NOT_FOUND;
}

ssize_t process_write_chunk (int fd, int file_id, int node_id, 
 int stripe_id, int chunk_num, int offset, 
 void *buf, int count) {
  // No buffering
  return DATA_NOT_FOUND;
}
