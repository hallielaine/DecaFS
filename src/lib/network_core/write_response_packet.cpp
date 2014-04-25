#include "write_response_packet.h"

WriteResponsePacket::WriteResponsePacket(void* buf, ssize_t packet_size) 
 : FilePacket(buf, packet_size) {

}

WriteResponsePacket::WriteResponsePacket(uint32_t fd, uint32_t file_id, uint32_t stripe_id,
 uint32_t chunk_num, uint32_t offset, uint32_t count) 
 : FilePacket(WRITE_RESPONSE, 0, fd, file_id, stripe_id, chunk_num, offset, count) {

}

std::ostream& WriteResponsePacket::print(std::ostream& stream) const {

  stream << "WriteResponsePacket" << std::endl;
  return FilePacket::print(stream);
}

std::ostream& operator<<(std::ostream& stream, const WriteResponsePacket &req) {
  return req.print(stream);
}
