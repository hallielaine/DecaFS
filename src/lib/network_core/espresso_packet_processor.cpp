#include "espresso_packet_processor.h"

Packet* process_read_packet(const ReadChunkRequest& r) {

  ssize_t bytes_read;
  uint8_t buffer[65536];
 
  bytes_read = read_chunk(r.fd, r.file_id, r.stripe_id, r.chunk_num, r.offset,
   buffer, r.count);

  return new ReadChunkResponse(r.id, r.fd, r.file_id, r.stripe_id, r.chunk_num, r.offset,
   bytes_read, buffer); 
}

Packet* process_write_packet(const WriteChunkRequest& r) {

  ssize_t bytes_written = write_chunk(r.fd, r.file_id, r.stripe_id, r.chunk_num,
   r.offset, r.data_buffer, r.count);

  return new WriteChunkResponse(r.id, r.fd, r.file_id, r.stripe_id, r.chunk_num, r.offset,
   bytes_written);
}

Packet* process_delete_packet(const DeleteChunkRequest& r) {

  // TODO check return val of delete_chunk, -1 error 0 success
  delete_chunk(r.fd, r.file_id, r.stripe_id, r.chunk_num);
  return new DeleteChunkResponse(r.id, r.file_id, r.stripe_id, r.chunk_num);
}
