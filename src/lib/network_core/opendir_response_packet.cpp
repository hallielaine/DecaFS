#include "opendir_response_packet.h"

OpendirResponsePacket::OpendirResponsePacket(int flags, char* filepath) 
 : Packet(0, OPENDIR_RESPONSE, strlen(filepath) + 1) {

 // char* base = (char*)(((uint8_t*)packet) + Packet::dataSize());

  //memcpy(base, filepath, strlen(filepath) + 1);
}

OpendirResponsePacket::OpendirResponsePacket(void* buf, ssize_t length) : Packet(buf, length) {

  //filepath = &((char*)packet)[Packet::dataSize()];
}

std::ostream& OpendirResponsePacket::print(std::ostream &stream) const {

  //stream << "OpendirResponsePacket" << std::endl;
  //stream << "\tfilepath: ";
  //stream.write(filepath, strlen(filepath));//strlen(filepath));
  //stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const OpendirResponsePacket &packet) {
  return packet.print(stream);
}
