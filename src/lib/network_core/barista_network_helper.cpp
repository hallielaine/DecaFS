#include "barista_network_helper.h"

BaristaServer* b_srvr = NULL;

int network_read_chunk(int32_t id, int fd, int file_id, int node_id, int stripe_id, 
 int chunk_num, int offset, int count) {

  if (!b_srvr) {
    b_srvr = BaristaServer::get();
  }

  ReadChunkRequest readRequest(id, fd, file_id, stripe_id, chunk_num, offset, count);
  return b_srvr->sendToEspresso(node_id, readRequest);
}

int network_write_chunk(int32_t id, int fd, int file_id, int node_id, int stripe_id, 
 int chunk_num, int offset, void* buf, int count) {

  if (!b_srvr) {
    b_srvr = BaristaServer::get();
  }

  WriteChunkRequest writeRequest(id, fd, file_id, stripe_id, chunk_num, offset, count, (uint8_t*)buf);
  return b_srvr->sendToEspresso(node_id, writeRequest);
}

int network_delete_chunk(int32_t id, int file_id, int node_id, int stripe_id, int chunk_num) {

  if (!b_srvr) {
    b_srvr = BaristaServer::get();
  }

  DeleteChunkRequest deleteRequest(id, file_id, stripe_id, chunk_num);
  return b_srvr->sendToEspresso(node_id, deleteRequest);
}

int send_open_result(struct client c, int result) {

  OpenPacketResponse response(result);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_read_result(struct client c, int fd, ssize_t count, void* buf) {

  printf("read_result: called with: fd <%d>, and count <%d>\n", fd, count);
  ReadResponsePacket response(fd, count, (uint8_t*)buf);
  printf("read_result: read response packet created\n");
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_write_result(struct client c, int fd, ssize_t count) {

  WriteResponsePacket response(fd, count);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_close_result(struct client c, int result) {
  
  CloseResponsePacket response(result);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_seek_result(struct client c, int result) {

  LseekResponsePacket response(result);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_delete_result(struct client c, int fd, int result) {

  return -1;
}

int send_opendir_result(struct client c, decafs_dir* dirp) {

  OpendirResponsePacket response(dirp);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_file_storage_stat_result(struct client c, const char* data) {

  FileStorageStatResponsePacket response(data);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}

int send_mkdir_result(struct client c, int result) {

  MkdirResponsePacket response(result);
  return c.ctc->sendToClient(response.packet, response.packet_size);
}
