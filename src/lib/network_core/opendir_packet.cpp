#include "opendir_packet.h"

OpendirPacket::OpendirPacket(char* filepath) 
 : Packet(0, OPENDIR, strlen(filepath) + 1),
 filepath(filepath) {

  char* base = (char*)(((uint8_t*)packet) + Packet::dataSize());

  memcpy(base, filepath, strlen(filepath) + 1);
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
