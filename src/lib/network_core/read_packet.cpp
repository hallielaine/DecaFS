#include "read_packet.h"

ReadRequest::ReadRequest(void* buf, ssize_t packet_size) 
 : FilePacket(buf, packet_size) {

}

ReadRequest::ReadRequest(uint32_t fd, uint32_t count) 
 : FilePacket(READ, 0, fd, 0, 0, 0, 0, count) {

}

std::ostream& ReadRequest::print(std::ostream& stream) const {

  stream << "ReadRequest" << std::endl;
  return FilePacket::print(stream);
}

std::ostream& operator<<(std::ostream& stream, const ReadRequest &req) {
  return req.print(stream);
}
