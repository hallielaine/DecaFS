#include "barista_network_helper.h"

BaristaServer* barista_server = NULL;

int network_read_chunk(int fd, int file_id, int node_id, int stripe_id, 
 int chunk_num, int offset, int count) {

  if (!barista_server) {
    barista_server = BaristaServer::get();
  }

  ReadChunkRequest readRequest(fd, file_id, stripe_id, chunk_num, offset, count);
  //return client->sendToClient(readRequest.packet, readRequest.packet_size);
  return barista_server->sendToEspresso(node_id, readRequest);
}

int network_write_chunk(int fd, int file_id, int node_id, int stripe_id, 
 int chunk_num, int offset, void* buf, int count) {

  if (!barista_server) {
    barista_server = BaristaServer::get();
  }

  WriteChunkRequest writeRequest(fd, file_id, stripe_id, chunk_num, offset, count, (uint8_t*)buf);
  //return client->sendToClient(writeRequest.packet, writeRequest.packet_size);
  return barista_server->sendToEspresso(node_id, writeRequest);
}

int network_delete_chunk(int file_id, int node_id, int stripe_id, int chunk_num) {

  if (!barista_server) {
    barista_server = BaristaServer::get();
  }

  DeleteChunkRequest deleteRequest(file_id, stripe_id, chunk_num);
  //return client->sendToClient(deleteRequest.packet, deleteRequest.packet_size);
  return barista_server->sendToEspresso(node_id, deleteRequest);
}
