#include "mkdir_packet.h"

MkdirPacket::MkdirPacket(const char* filename, mode_t mode) 
 : Packet(0, MKDIR, strlen(filename) + 1 + dataSize), mode(mode) {

  char* base = (char*)(((uint8_t*)packet) + Packet::dataSize());
  
  ((mode_t*)base)[0] = mode;
  filepath = base + dataSize; 

  memcpy(filepath, filename, strlen(filename) + 1);
}

MkdirPacket::MkdirPacket(void* buf, ssize_t length) : Packet(buf, length) {

  char* base = (char*)packet + Packet::dataSize();

  mode = ((mode_t*)base)[0];
  filepath = base + dataSize; 
}

std::ostream& MkdirPacket::print(std::ostream &stream) const {

  stream << "MkdirPacket" << std::endl;
  stream << "\tmode: " << mode << std::endl;
  stream << "\tfilepath: ";
  stream.write(filepath, strlen(filepath));//strlen(filepath));
  stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const MkdirPacket &packet) {
  return packet.print(stream);
}
