#include "write_packet.h"

WritePacket::WritePacket(void* buf, ssize_t packet_size) 
 : FileDataPacket(buf, packet_size) {

}

WritePacket::WritePacket(uint32_t fd, uint32_t count, uint8_t * buf) 
 : FileDataPacket(WRITE, 0, fd, 0, 0, 0, 0, count, buf) {

}

std::ostream& WritePacket::print(std::ostream& stream) const {

  stream << "WritePacket" << std::endl;
  return FileDataPacket::print(stream);
}

std::ostream& operator<<(std::ostream& stream, const WritePacket &req) {
  return req.print(stream);
}
