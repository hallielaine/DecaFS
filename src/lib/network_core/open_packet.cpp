#include "open_packet.h"

OpenPacket::OpenPacket(int flags, char* filepath) 
 : Packet(OPEN, strlen(filepath) + sizeof(int)),
 open_flags(flags), filepath(filepath) {

  uint32_t* base = (uint32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = open_flags; 

  char* pathbase = ((char*)base) + sizeof(open_flags);
  strncpy(pathbase, filepath, strlen(filepath));
}

OpenPacket::OpenPacket(void* buf, ssize_t length) : Packet(buf, length) {

  uint32_t* base = (uint32_t*)(((uint8_t*)packet) + Packet::dataSize());
  open_flags = base[0];

  int pathlen = packet_size - Packet::dataSize() - sizeof(open_flags) + 1;
  filepath = (char*)malloc(pathlen);
  strncpy(filepath, (char*)base[1], pathlen-1);
  filepath[pathlen-1] = '\0';
}

std::ostream& OpenPacket::print(std::ostream &stream) const {

  stream << "OpenPacket" << std::endl;
  stream << "flags: " << open_flags << std::endl;
  stream << "filepath: " << filepath << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const OpenPacket &packet) {
  return packet.print(stream);
}