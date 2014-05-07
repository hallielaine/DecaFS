#include "lseek_response_packet.h"

LseekResponsePacket::LseekResponsePacket(void* buf, ssize_t size) : Packet(buf, size) {

  uint32_t* base = (uint32_t*)(((uint8_t*)buf) + Packet::dataSize());
  result = base[0];
}

LseekResponsePacket::LseekResponsePacket(int result) : Packet(0, LSEEK_RESPONSE, data_size),
  result(result) {

  int32_t* base = (int32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = result;
}

std::ostream& LseekResponsePacket::print(std::ostream& stream) const {

  stream << "LseekResponsePacket" << std::endl;
  stream << "\tresult: " << result << std::endl;
  return Packet::print(stream); 
}

std::ostream& operator<<(std::ostream& stream, const LseekResponsePacket &p) {
  return p.print(stream); 
}

