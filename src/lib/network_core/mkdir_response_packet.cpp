#include "mkdir_response_packet.h"

MkdirResponsePacket::MkdirResponsePacket(void* buf, ssize_t size) : Packet(buf, size) {

  uint32_t* base = (uint32_t*)(((uint8_t*)buf) + Packet::dataSize());
  result = base[0];
}

MkdirResponsePacket::MkdirResponsePacket(int result) : Packet(0, MKDIR_RESPONSE, data_size),
  result(result) {

  int32_t* base = (int32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = result;
}

std::ostream& MkdirResponsePacket::print(std::ostream& stream) const {

  stream << "MkdirResponsePacket" << std::endl;
  stream << "\tresult: " << result << std::endl;
  return Packet::print(stream); 
}

std::ostream& operator<<(std::ostream& stream, const MkdirResponsePacket &p) {
  return p.print(stream); 
}

