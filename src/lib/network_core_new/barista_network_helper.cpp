#include "barista_network_helper.h"

int network_read_chunk(ConnectionToClient* client, int fd, int file_id, int stripe_id, 
 int chunk_num, int offset, int count) {

  ReadChunkRequest readRequest(fd, file_id, stripe_id, chunk_num, offset, count);
  return client->sendToClient(readRequest.packet, readRequest.packet_size);
}

int network_write_chunk(ConnectionToClient* client, int fd, int file_id, int stripe_id, 
 int chunk_num, int offset, void* buf, int count) {

  WriteChunkRequest writeRequest(fd, file_id, stripe_id, chunk_num, offset, count, (uint8_t*)buf);
  return client->sendToClient(writeRequest.packet, writeRequest.packet_size);
}

int network_delete_chunk(ConnectionToClient* client, int file_id, int stripe_id,
 int chunk_num) {

  DeleteChunkRequest deleteRequest(file_id, stripe_id, chunk_num);
  return client->sendToClient(deleteRequest.packet, deleteRequest.packet_size);
}
