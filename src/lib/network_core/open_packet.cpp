#include "open_packet.h"

OpenPacket::OpenPacket(int flags, const char* filename) 
 : Packet(0, OPEN, strlen(filepath) + 1 + sizeof(int32_t)),
 open_flags(flags) {

  uint32_t* base = (uint32_t*)(((uint8_t*)packet) + Packet::dataSize());
  base[0] = open_flags; 

  filepath = ((char*)base) + sizeof(open_flags);
  memcpy(filepath, filename, strlen(filename));
  filepath[strlen(filename)] = '\0';
}

OpenPacket::OpenPacket(void* buf, ssize_t length) : Packet(buf, length) {

  uint32_t* base = (uint32_t*)(&(((char*)packet)[Packet::dataSize()]));
  open_flags = base[0];
  filepath = (char*)(&(((char*)packet)[Packet::dataSize() + sizeof(int32_t)]));
  filepath[packet_size - Packet::dataSize() - 5] = '\0';

  std::cout << "open packet deserialization:" << std::endl;
  std::cout << "\tpacket_size: " << packet_size << std::endl;
  std::cout << "\tdata_size: " << Packet::dataSize() << std::endl;
  std::cout << "\topen_flags: " << open_flags << std::endl;
  

  filepath[packet_size - Packet::dataSize() - 5] = '\0';

  //int pathlen = packet_size - Packet::dataSize() - sizeof(open_flags) + 1;
  //filepath = (char*)malloc(pathlen);
  //strncpy(filepath, (char*)base[1], pathlen-1);
  //filepath[pathlen-1] = '\0';
}

std::ostream& OpenPacket::print(std::ostream &stream) const {

  int length = packet_size - Packet::dataSize() - 5;

  stream << "OpenPacket" << std::endl;
  stream << "\tflags: " << open_flags << std::endl;
  stream << "\tactual length: " << length << std::endl;
  stream << "\tstrlen length: " << strlen(filepath) << std::endl;
  stream << "\tfilepath: ";
  stream.write(filepath, length);//strlen(filepath));
  stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const OpenPacket &packet) {
  return packet.print(stream);
}
