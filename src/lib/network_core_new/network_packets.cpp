#include "network_packets.h"

static int next_seq_num = 1;

// -------------------- NetworkPacket --------------------------

NetworkPacket::NetworkPacket(uint32_t flag) : flag(flag) {

  seq_num = next_seq_num++;
}

NetworkPacket::NetworkPacket(void* buf, ssize_t size) :
  packet_size(size), packet(buf) {

  seq_num = ((uint32_t*)buf)[0];
  flag = ((uint32_t*)buf)[1];
}

void NetworkPacket::allocatePacket() {

  packet_size = data_size + header_size;
  packet = (char*)malloc(packet_size);

  uint32_t* ptr = (uint32_t*)packet;
  ptr[0] = seq_num;
  ptr[1] = flag;
}

// ------------------------ ReadChunkPacket ------------------

ReadChunkPacket::ReadChunkPacket(void* buf, ssize_t packet_size) : NetworkPacket(buf, packet_size) {

  uint32_t* base = (uint32_t*)(((uint8_t*)buf) + header_size);

  fd = base[0];
  file_id = base[1];
  stripe_id = base[2];
  chunk_num = base[3];
  offset = base[4];
  count = base[5];
}

ReadChunkPacket::ReadChunkPacket(uint32_t fd, uint32_t file_id, uint32_t stripe_id, uint32_t chunk_num,
 uint32_t offset, uint32_t count) : NetworkPacket(READ_CHUNK) {

  data_size = 6*sizeof(uint32_t);

  allocatePacket();
 
  uint32_t* base = (uint32_t*)(((uint8_t*)packet) + header_size);
  base[0] = fd;
  base[1] = file_id;
  base[2] = stripe_id;
  base[3] = chunk_num;
  base[4] = offset;
  base[5] = count;
}

void ReadChunkPacket::printInfo() {

  printf("%d, %d, %d, %d, %d, %d\n", fd, file_id, stripe_id, chunk_num, offset, count);
}
