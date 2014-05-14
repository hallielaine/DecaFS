#include "remove_packet.h"

RemovePacket::RemovePacket(const char* filename) 
 : Packet(0, REMOVE, strlen(filename) + 1) {

  filepath = (char*)(((uint8_t*)packet) + Packet::dataSize());

  memcpy(filepath, filename, strlen(filename) + 1);
}

RemovePacket::RemovePacket(void* buf, ssize_t length) : Packet(buf, length) {

  filepath = &((char*)packet)[Packet::dataSize()];
}

std::ostream& RemovePacket::print(std::ostream &stream) const {

  stream << "RemovePacket" << std::endl;
  stream << "\tfilepath: ";
  stream.write(filepath, strlen(filepath));//strlen(filepath));
  stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const RemovePacket &packet) {
  return packet.print(stream);
}
