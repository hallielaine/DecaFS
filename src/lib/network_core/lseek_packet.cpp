#include "lseek_packet.h"

LseekPacket::LseekPacket(int32_t fd, int32_t offset, int32_t whence) 
 : Packet(0, LSEEK, dataSize),
 fd(fd), offset(offset), whence(whence) {

  int32_t* base = (int32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = fd; 
  base[1] = offset; 
  base[2] = whence; 
}

LseekPacket::LseekPacket(void* buf, ssize_t length) : Packet(buf, length) {

  uint32_t* base = (uint32_t*)(&(((char*)packet)[Packet::dataSize()]));

  fd = base[0];
  offset = base[2];
  whence = base[3];
}

std::ostream& LseekPacket::print(std::ostream &stream) const {

  stream << "LseekPacket" << std::endl;
  stream << "\tfd: " << fd << std::endl;
  stream << "\toffset: " << offset << std::endl;
  stream << "\twhence: " << whence << std::endl;
  stream << std::endl;

  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const LseekPacket &packet) {
  return packet.print(stream);
}
