#include "write_response_packet.h"

WriteResponsePacket::WriteResponsePacket(void* buf, ssize_t packet_size) 
 : FilePacket(buf, packet_size) {

}

WriteResponsePacket::WriteResponsePacket(uint32_t fd, uint32_t count) 
 : FilePacket(0, WRITE_RESPONSE, 0, fd, 0, 0, 0, 0, count) {

}

std::ostream& WriteResponsePacket::print(std::ostream& stream) const {

  stream << "WriteResponsePacket" << std::endl;
  return FilePacket::print(stream);
}

std::ostream& operator<<(std::ostream& stream, const WriteResponsePacket &req) {
  return req.print(stream);
}
