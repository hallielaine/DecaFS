#include "open_packet_response.h"

OpenPacketResponse::OpenPacketResponse(void* buf, ssize_t size) : Packet(buf, size) {

  uint32_t* base = (uint32_t*)(((uint8_t*)buf) + Packet::dataSize());
  response = base[0];
}

OpenPacketResponse::OpenPacketResponse(int response) : Packet(0, OPEN_RESPONSE, data_size),
  response(response) {

  uint32_t* base = (uint32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = response;
}

std::ostream& OpenPacketResponse::print(std::ostream& stream) const {

  stream << "OpenPacketRespones" << std::endl;
  stream << "\tresponse: " << response << std::endl;
  return Packet::print(stream); 
}

std::ostream& operator<<(std::ostream& stream, const OpenPacketResponse &p) {
  return p.print(stream); 
}
