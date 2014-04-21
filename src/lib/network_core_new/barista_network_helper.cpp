#include "barista_network_helper.h"

int network_read_chunk(ConnectionToClient* client, int fd, int file_id, int stripe_id, 
 int chunk_num, int offset, int count) {

  ReadChunkPacket readRequest(fd, file_id, stripe_id, chunk_num, offset, count);
  return client->sendToClient(readRequest.packet, readRequest.packet_size);
}

int network_write_chunk(ConnectionToClient* client, int fd, int file_id, int stripe_id, 
 int chunk_num, int offset, void* buf, int count) {

  return -1;
}

int network_delete_chunk(ConnectionToClient* client, int file_id, int stripe_id,
 int chunk_num) {

  return -1;
}
