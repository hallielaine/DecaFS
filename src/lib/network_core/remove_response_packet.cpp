#include "remove_response_packet.h"

RemoveResponsePacket::RemoveResponsePacket(void* buf, ssize_t size) : Packet(buf, size) {

  uint32_t* base = (uint32_t*)(((uint8_t*)buf) + Packet::dataSize());
  result = base[0];
}

RemoveResponsePacket::RemoveResponsePacket(int result) : Packet(0, REMOVE_RESPONSE, data_size),
  result(result) {

  int32_t* base = (int32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = result;
}

std::ostream& RemoveResponsePacket::print(std::ostream& stream) const {

  stream << "RemoveResponsePacket" << std::endl;
  stream << "\tresult: " << result << std::endl;
  return Packet::print(stream); 
}

std::ostream& operator<<(std::ostream& stream, const RemoveResponsePacket &p) {
  return p.print(stream); 
}

