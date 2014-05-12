#include "file_storage_stat_packet.h"

FileStorageStatPacket::FileStorageStatPacket(const char* filepath) 
 : Packet(0, OPENDIR, strlen(filepath) + 1) {

  filename = (char*)(((uint8_t*)packet) + Packet::dataSize());

  memcpy(filename, filepath, strlen(filepath) + 1);
}

FileStorageStatPacket::FileStorageStatPacket(void* buf, ssize_t length) : Packet(buf, length) {

  filename = &((char*)packet)[Packet::dataSize()];
}

std::ostream& FileStorageStatPacket::print(std::ostream &stream) const {

  stream << "FileStorageStatPacket" << std::endl;
  stream << "\tfilename: ";
  stream.write(filename, strlen(filename));
  stream << std::endl;
  return Packet::print(stream);
}

std::ostream& operator <<(std::ostream& stream, const FileStorageStatPacket &packet) {
  return packet.print(stream);
}
