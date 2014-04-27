#include "read_response_packet.h"

ReadResponsePacket::ReadResponsePacket(void* buf, ssize_t packet_size) 
 : FileDataPacket(buf, packet_size) {

}

ReadResponsePacket::ReadResponsePacket(uint32_t fd, uint32_t count, uint8_t * buf) 
 : FileDataPacket(0, READ_RESPONSE, 0, fd, 0, 0, 0, 0, count, buf) {

}

std::ostream& ReadResponsePacket::print(std::ostream& stream) const {

  stream << "ReadResponsePacket" << std::endl;
  return FileDataPacket::print(stream);
}

std::ostream& operator<<(std::ostream& stream, const ReadResponsePacket &req) {
  return req.print(stream);
}
