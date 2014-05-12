#include "opendir_packet.h"

OpendirPacket::OpendirPacket(const char* filename) 
 : Packet(0, OPENDIR, strlen(filename) + 1) {

  filepath = (char*)(((uint8_t*)packet) + Packet::dataSize());

  memcpy(filepath, filename, strlen(filename) + 1);
}

OpendirPacket::OpendirPacket(void* buf, ssize_t length) : Packet(buf, length) {

  filepath = &((char*)packet)[Packet::dataSize()];
}

std::ostream& OpendirPacket::print(std::ostream &stream) const {

  stream << "OpendirPacket" << std::endl;
  stream << "\tfilepath: ";
  stream.write(filepath, strlen(filepath));//strlen(filepath));
  stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const OpendirPacket &packet) {
  return packet.print(stream);
}
