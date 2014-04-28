#include "close_packet.h"

ClosePacket::ClosePacket(void* buf, ssize_t size) : Packet(buf, size) {

  uint32_t* base = (uint32_t*)(((uint8_t*)buf) + Packet::dataSize());
  fd = base[0];
}

ClosePacket::ClosePacket(int fd) : Packet(0, CLOSE, data_size),
  fd(fd) {

  uint32_t* base = (uint32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = fd;
}

std::ostream& ClosePacket::print(std::ostream& stream) const {

  stream << "ClosePacketializationPacket" << std::endl;
  stream << "\tfd: " << fd << std::endl;
  return Packet::print(stream); 
}

std::ostream& operator<<(std::ostream& stream, const ClosePacket &p) {
  return p.print(stream); 
}

